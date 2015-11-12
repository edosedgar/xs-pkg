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
#define EMPTY_M 2
#define FULLY_M 3
#define MEM_M   4

/*
 * Other constants
 */
#define SEM_NUM 5
#define ACCESS_FLAGS 0666
#define OPER_NUM SEM_NUM
#define DATA_SIZE 4094

/*
 * Semaphores functions
 */
#define P(sem, flags) ({            \
    sem_ops[0].sem_num = (sem);     \
    sem_ops[0].sem_op = -1;         \
    sem_ops[0].sem_flg = (flags);   \
    semop(semaphores, sem_ops, 1);  \
})                                    

#define V(sem, flags) ({            \
    sem_ops[0].sem_num = (sem);     \
    sem_ops[0].sem_op = 1;          \
    sem_ops[0].sem_flg = (flags);   \
    semop(semaphores, sem_ops, 1);  \
})                                  

#define Vz(sem, flags) ({           \
    sem_ops[0].sem_num = (sem);     \
    sem_ops[0].sem_op = 0;          \
    sem_ops[0].sem_flg = 0;         \
    sem_ops[1].sem_num = (sem);     \
    sem_ops[1].sem_op = 1;          \
    sem_ops[1].sem_flg = (flags);   \
    semop(semaphores, sem_ops, 2);  \
})                                  

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
struct sembuf sem_ops[OPER_NUM];

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
    fprintf(stderr, "%d", semctl( semaphores, FULLY_M, GETVAL));
	if (work_mode == RX) {
		if (receiver_module(semaphores, buf) == -1 )
			exit(EXIT_FAILURE);
        shmdt(buf);
        shmctl(sh_mem, IPC_RMID, NULL);
    }

	if (work_mode == TX) {
		if (transmitter_module(file_name, semaphores, buf) == -1 ) 
			exit(EXIT_FAILURE);
        semctl(semaphores, 5, IPC_RMID, NULL);
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
     */
    sem_ops[0].sem_num = RCV_M;
    sem_ops[0].sem_op = 0;
    sem_ops[0].sem_flg = IPC_NOWAIT;
    sem_ops[1].sem_num = RCV_M;
    sem_ops[1].sem_op = 1;
    sem_ops[1].sem_flg = SEM_UNDO;
    if (semop(semaphores, sem_ops, 2) == -1) {
        fprintf(stderr, "Copy of program already started.\n");
        return -1;
    }
    /*
     * Start critical area
     */
    uint16_t cur_size = DATA_SIZE;
    int flag = 0;

    while (cur_size != 0) {
        if (flag == 0) 
            P(FULLY_M, SEM_UNDO);
        else
            P(FULLY_M, 0);
       
        Vz(MEM_M, SEM_UNDO); 
               
        cur_size = memory->size;
        write(STDOUT_FILENO, memory->data, cur_size);
        fprintf(stderr, "Block %dB read.\n", cur_size);
        usleep(100000);
           
        P(MEM_M, SEM_UNDO);

        if (flag == 0) 
            V(EMPTY_M, SEM_UNDO);
        else
            V(EMPTY_M, 0); 
        flag |= 1;
    }
    /*
     * End critical area
     */ 
    return 0;
}


int transmitter_module(char* file_name, int semaphores, data_packet_t* memory) { 
    /*
     * V(SND_M) if no zero then out
     */
    sem_ops[0].sem_num = SND_M;
    sem_ops[0].sem_op = 0;
    sem_ops[0].sem_flg = IPC_NOWAIT;
    sem_ops[1].sem_num = SND_M;
    sem_ops[1].sem_op = 1;
    sem_ops[1].sem_flg = SEM_UNDO;
    if (semop(semaphores, sem_ops, 2) == -1) {
        fprintf(stderr, "Copy of program already started.\n");
        return -1;
    }
    /*
     * Initialize EMPTY_M
     */
    V(EMPTY_M, SEM_UNDO);
    /*
     * Start critical area
     */
    int file_d = open(file_name, O_RDONLY);
    int16_t cur_size = DATA_SIZE;
    int flag = 0;

    while (cur_size > 0) {
        if (flag == 0) 
            P(EMPTY_M, SEM_UNDO); 
        else
            P(EMPTY_M, 0); 
        
        Vz(MEM_M, SEM_UNDO); 
        
        cur_size = read(file_d, memory->data, DATA_SIZE);
        memory->size = cur_size;
        fprintf(stderr, "Block %dB sent.\n", cur_size);
        
        P(MEM_M, SEM_UNDO);
        
        if (flag == 0)  
            V(FULLY_M, SEM_UNDO);
        else
            V(FULLY_M, 0);
        flag |= 1;
    }
    close(file_d);
    /*
     * End critical area
     */
	return 0;
}
