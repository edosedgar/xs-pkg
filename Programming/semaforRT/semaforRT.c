#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h>

/*
 * Mode of working
 */
#define RX 1
#define TX 2

/*
 * Semaphores
 */
#define RCV_M   0
#define SND_M   1
#define MEM_M   2

/*
 * Other constants
 */
#define SEM_NUM         3
#define ACCESS_FLAGS    0666
#define MAX_OPER_NUM    4
#define DATA_SIZE       4094

/*
 * Semaphores functions
 */
#define ADD_OPER(num, sem, oper, flags) ({  \
    sem_ops[num].sem_num = (sem);           \
    sem_ops[num].sem_op = (oper);           \
    sem_ops[num].sem_flg = (flags);         \
})

#define DO_OPER(num)                        \
    (semop(semaphores, sem_ops, (num))) 

/*
 * Operations under semaphores
 */
#define V   +1
#define P   -1
#define ZW   0

/*
 * Definition data packet
 * size - current size
 * data - rx/tx data
 * sizeof = 4K
 */
typedef struct data_packet_s {
    uint16_t size;
    char data[DATA_SIZE];
} data_packet_t;

/*
* Create array of operations
*/ 
struct sembuf sem_ops[MAX_OPER_NUM];

/*
 * Prototypes of functions
 */
int opt_handler(int argc, char* argv[], char** file_name);
int receiver_module(int semaphores, data_packet_t* memory);
int transmitter_module(char* file_name, int semaphores, data_packet_t* memory);

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

int main(int argc, char* argv[]) {
    char* file_name = NULL;
    int work_mode = 0;
    /*
     * Choose mode of working
     */
    if ((work_mode = opt_handler(argc, argv, &file_name)) == -1) 
        exit(EXIT_FAILURE);
    /*
     * Generate key(it is same for two processes)
     */
    key_t key = ftok(argv[0], 0);
    /*
     * Create semaphores
     */
    int semaphores = semget(key, SEM_NUM, IPC_CREAT | ACCESS_FLAGS);
    /*
     * Create shared memory and attach it
     */
    int sh_mem = shmget(key, sizeof(data_packet_t), IPC_CREAT | ACCESS_FLAGS);
    data_packet_t* buf = (data_packet_t*) shmat(sh_mem, NULL, 0);
    /*
     * Choose module and execute it
     */
    if (work_mode == RX) {
        if (receiver_module(semaphores, buf) == -1)
            exit(EXIT_FAILURE);
        shmdt(buf);
        shmctl(sh_mem, IPC_RMID, NULL);
    }

    if (work_mode == TX) {
        if (transmitter_module(file_name, semaphores, buf) == -1 ) 
            exit(EXIT_FAILURE);
        semctl(semaphores, SEM_NUM, IPC_RMID, NULL);
    }
    
    exit(EXIT_SUCCESS);
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

int opt_handler (int argc, char* argv[], char** file_name) {
    int opt = 0;
    opterr = 0;
    int work_mode = 0;
    while ((opt = getopt(argc, argv, "i::rth")) != -1) {
    switch (opt) {
        case 'h': 
            printf("\n\n********** ReceiveTransmitterTerminal v1 help: ************\n");
            printf("*                                                         *\n");                               
            printf("* rtterm [-r|-t] [-i /filepath]                           *\n");
            printf("*                                                         *\n");                               
            printf("* -r ---> Receive mode (to stdout)                        *\n");
            printf("*                                                         *\n");
            printf("* -t ---> Transmitter mode                                *\n");
            printf("*                                                         *\n");
            printf("* -i ---> Transmitted content (file PATH)                 *\n");
            printf("*                                                         *\n");
            printf("\033[94m***********************************************************\n");
            printf("*                                                         *\n");
            printf("* Required program for working: none                      *\n");
            printf("*                                                         *\n");
            printf("***********************************************************\n");
            printf("\033[91m*                                                         *\n");
            printf("* Program is written on C by EdOS at 25.10.2015           *\n");
            printf("*                                                         *\n");
            printf("***********************************************************\033[0m\n\n");
            exit(EXIT_SUCCESS);
            break;
        case 'r':  
            if (work_mode == 0) 
			    work_mode = RX;
            else {
                fprintf(stderr, "Please, select one working mode.\n");
			    return -1;	
            }
            break;
        case 't': 
            if ( work_mode == 0 ) 
                work_mode = TX;
            else {
                fprintf(stderr, "Please, select one working mode.\n");
			    return -1;	
            }
            break;
        case 'i': 
            if ( argv[optind] == NULL || strlen(argv[optind]) == 0 || \
                argv[optind][0] == '-' ) {
                fprintf(stderr, "Missing filepath after '-i'.\n" );	
                return -1;
            } else 
                *file_name = argv[optind];			
            break;
        default: 
            fprintf(stderr, "Unrecognized command line option.\n");
            return -1;	
        }
    }
    if (work_mode == 0) {
        fprintf(stderr, "Don't set a work mode. Use help for info.\n");
        return -1;
    }
    if (work_mode == TX && *file_name == NULL) {
        fprintf(stderr, "Mode transmitter, but no input file.\n");
        return -1;
    }
    if (work_mode == RX && *file_name != NULL) {
        fprintf(stderr, "In receiver mode path on file miss.\n");
    }
    return work_mode;
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************


int receiver_module(int semaphores, data_packet_t* memory) {
    /*
     * V(RCV_M) if no zero then out
     * V(MEM_M) -> P(MEM_M) 
     */
    ADD_OPER(0, RCV_M, ZW, IPC_NOWAIT);
    ADD_OPER(1, RCV_M,  V, SEM_UNDO);
    ADD_OPER(2, MEM_M,  V, SEM_UNDO);
    ADD_OPER(3, MEM_M,  P, 0);

    if (DO_OPER(4) == -1) {
        fprintf(stderr, "Copy of program already started.\n");
        return -1;
    }
    uint16_t cur_size = DATA_SIZE;

    ADD_OPER(0, SND_M, P, 0);
    ADD_OPER(1, SND_M, V, 0);
    if (DO_OPER(2) == -1) {
        perror("");
        return -1;
    }

    while (cur_size != 0) {
        /*
         * Check death transmitter
         * Set MEM_M
         */
        ADD_OPER(0, SND_M, P, IPC_NOWAIT);
        ADD_OPER(1, SND_M, V, 0);
        ADD_OPER(2, MEM_M, P, 0);
        ADD_OPER(3, MEM_M, V, 0);

        if (DO_OPER(4) == -1) {
            perror("");
            return -1;
        }
        /*
         * Critical area start
         */
        cur_size = memory->size;
        write(STDOUT_FILENO, memory->data, cur_size);
        usleep(100000);
        fprintf(stderr,"hi"); 
        /*
         * Critical area end
         */
        /*
         * Check death transmitter
         * Reset MEM_M
         */
        ADD_OPER(0, SND_M, P, IPC_NOWAIT);
        ADD_OPER(1, SND_M, V, 0);
        ADD_OPER(2, MEM_M, P, 0);
        if (DO_OPER(3) == -1) {
            perror("");
            return -1;
        }
    }
    return 0;
}


int transmitter_module(char* file_name, int semaphores, data_packet_t* memory) { 
    /*
     * V(SND_M) if no zero then out
     * V(MEM_M) -> P(MEM_M) 
     */
    ADD_OPER(0, SND_M, ZW, IPC_NOWAIT);
    ADD_OPER(1, SND_M,  V, SEM_UNDO);
    ADD_OPER(2, MEM_M,  V, SEM_UNDO);
    ADD_OPER(3, MEM_M,  P, 0);

    if (DO_OPER(4) == -1) {
        fprintf(stderr, "Copy of program already started.\n");
        return -1;
    }

    ADD_OPER(0, RCV_M, P, 0);
    ADD_OPER(1, RCV_M, V, 0);
    if (DO_OPER(2) == -1) {
        perror("");
        return -1;
    }

    int file_d = open(file_name, O_RDONLY);
    int16_t cur_size = DATA_SIZE;

    while (cur_size > 0) {
        /*
         * Check death receiver
         * Wait zero MEM_M
         */
        ADD_OPER(0, RCV_M, P,  IPC_NOWAIT);
        ADD_OPER(1, RCV_M, V,  0);
        ADD_OPER(2, MEM_M, ZW, 0);

        if (DO_OPER(3) == -1) {
            perror("");
            return -1;
        }
        /*
         * Critical area start
         */
        cur_size = read(file_d, memory->data, DATA_SIZE);
        memory->size = cur_size;
        /*
         * Critical area end
         */
        ADD_OPER(0, MEM_M, V, 0);
        if (DO_OPER(1) == -1) {
            perror("");
            return -1;
        }
    }
    ADD_OPER(0, MEM_M, ZW, 0);
    if (DO_OPER(1) == -1) {
        perror("");
        return -1;
    }

    close(file_d);
	return 0;
}
