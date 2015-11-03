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

#define RX 1
#define TX 2

const int16_t buffer_size = 4094;
const int16_t head_size = 2;

int OptHandler (int argc, char* argv[], char** FileName);
int receiver_module(void);
int transmitter_module(char* FileName);

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

int main (int argc, char* argv[]) {

	char* FileName = NULL;
	int WorkMode = 0;

	if ( (WorkMode = OptHandler(argc, argv, &FileName)) == -1 ) 
		exit(EXIT_FAILURE);

	if ( WorkMode == RX )
		if (receiver_module() == -1 )
			exit(EXIT_FAILURE);

	if ( WorkMode == TX ) 
		if (transmitter_module(FileName) == -1 ) 
			exit(EXIT_FAILURE);
	
	exit(EXIT_SUCCESS);
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

int OptHandler (int argc, char* argv[], char** FileName) {
	int opt = 0;
	opterr = 0;
	int WorkMode = 0;
	while ( (opt = getopt(argc, argv, "i::rth")) != -1 ) 
	{
		switch ( opt ) 
		{
		case 'h': {
			printf ("\n\n********** ReceiveTransmitterTerminal v1 help: ************\n");
			printf ("*                                                         *\n");                               
			printf ("* rtterm [-r|-t] [-i /filepath]                           *\n");
			printf ("*                                                         *\n");                               
			printf ("* -r ---> Receive mode (to stdout)                        *\n");
			printf ("*                                                         *\n");
			printf ("* -t ---> Transmitter mode                                *\n");
			printf ("*                                                         *\n");
			printf ("* -i ---> Transmitted content (file PATH)                 *\n");
			printf ("*                                                         *\n");
			printf ("\033[94m***********************************************************\n");
			printf ("*                                                         *\n");
			printf ("* Required program for working: none                      *\n");
			printf ("*                                                         *\n");
			printf ("***********************************************************\n");
			printf ("\033[91m*                                                         *\n");
			printf ("* Program is written on C by EdOS at 25.10.2015           *\n");
			printf ("*                                                         *\n");
			printf ("***********************************************************\033[0m\n\n");
			exit(EXIT_SUCCESS);
			break;
		}
		case 'r': { 
			if ( WorkMode == 0 ) 
				WorkMode = RX;
			else {
				fprintf(stderr, "Please, select one working mode.\n");
			   	return -1;	
			}
			break;
		}
		case 't': { 
			if ( WorkMode == 0 ) 
				WorkMode = TX;
			else {
				fprintf(stderr, "Please, select one working mode.\n");
			   	return -1;	
			}
			break;
		}
		case 'i': {
			if ( argv[optind] == NULL || strlen(argv[optind]) == 0 || \
				 argv[optind][0] == '-' ) {
				fprintf(stderr, "Missing filepath after '-i'.\n" );	
				return -1;
			} else 
				*FileName = argv[optind];			
			break;
		}
		default: {
			fprintf(stderr, "Unrecognized command line option.\n");
			return -1;
		}			
		}
	}
	if ( WorkMode == 0 ) {
		fprintf(stderr, "Don't set a work mode. Use help for info.\n");
		return -1;
	}
	if ( WorkMode == TX && *FileName == NULL ) {
		fprintf(stderr, "Mode transmitter, but no input file.\n");
		return -1;
	}
	if ( WorkMode == RX && *FileName != NULL ) {
		fprintf(stderr, "In receiver mode path on file miss.\n");
	}
	return WorkMode;
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

int receiver_module(void){
    key_t key = ftok("/", 'b');
    int m_rcv = semget(key, 1, IPC_CREAT | 0666);

    key_t key2 = ftok("/", 'c');
    int sh_mem = shmget(key2, buffer_size + head_size, IPC_CREAT | 0666);
    char* buf = shmat(sh_mem, (void*)0, 0); 

    key_t key3 = ftok("/", 'd');
    int m_mutx  = semget(key3, 1, IPC_CREAT | 0666);
    key_t key4 = ftok("/", 'e');
    int m_full = semget(key4, 1, IPC_CREAT | 0666);
    key_t key5 = ftok("/", 'f');
    int m_empt = semget(key5, 1, IPC_CREAT | 0666);
 
    struct sembuf sops[2];
    sops[0].sem_num = 0;
    sops[0].sem_op = 0;
    sops[0].sem_flg = SEM_UNDO | IPC_NOWAIT;
    sops[1].sem_num = 0;
    sops[1].sem_op = 1;
    sops[1].sem_flg = SEM_UNDO;
    if (semop(m_rcv, sops, 2) == -1) {
        fprintf(stderr, "Copy of program already started.\n");
        return -1;
    }
    /*
     * Start critical area
     */
    int16_t cur_size = buffer_size;
    while (cur_size  != 0) {
        sops[0].sem_num = 0;
        sops[0].sem_op = -1;
        sops[0].sem_flg = SEM_UNDO;
        semop(m_full, sops, 1);
        
        sops[0].sem_num = 0;
        sops[0].sem_op = 0;
        sops[0].sem_flg = SEM_UNDO;
        sops[1].sem_num = 0;
        sops[1].sem_op = 1;
        sops[1].sem_flg = SEM_UNDO;
        semop(m_mutx, sops, 2);
        
        cur_size = ((uint8_t)buf[0] + (((uint8_t)buf[1]) << 8));
        write(STDOUT_FILENO, buf + head_size, cur_size);
        fprintf(stderr, "Block %dB read.\n", cur_size);
        usleep(100000);
           
        sops[0].sem_num = 0;
        sops[0].sem_op = -1;
        sops[0].sem_flg = SEM_UNDO;
        semop(m_mutx, sops, 1);
        
        semop(m_empt, sops, 1); 
    }
    /*
     * End critical area
     */
    sops[0].sem_num = 0;
    sops[0].sem_op = -1;
    sops[0].sem_flg = SEM_UNDO;
    semop(m_rcv, sops, 1);

    shmdt(buf);
    shmctl(sh_mem, IPC_RMID, NULL);
	return 0;
}

int transmitter_module(char* FileName){
    key_t key = ftok("/", 'a');
    int m_tran = semget(key, 1, IPC_CREAT | 0666);
   
    key_t key2 = ftok("/", 'c');
    int sh_mem = shmget(key2, buffer_size + head_size, IPC_CREAT | 0666);
    char* buf = shmat(sh_mem, NULL, 0); 

    key_t key3 = ftok("/", 'd');
    int m_mutx = semget(key3, 1, IPC_CREAT | 0666);
    key_t key4 = ftok("/", 'e');
    int m_full = semget(key4, 1, IPC_CREAT | 0666);
    key_t key5 = ftok("/", 'f');
    int m_empt = semget(key5, 1, IPC_CREAT | 0666);

    struct sembuf sops[2];
    sops[0].sem_num = 0;
    sops[0].sem_op = 0;
    sops[0].sem_flg = SEM_UNDO | IPC_NOWAIT;
    sops[1].sem_num = 0;
    sops[1].sem_op = 1;
    sops[1].sem_flg = SEM_UNDO;
    if (semop(m_tran, sops, 2) == -1) {
        fprintf(stderr, "Copy of program already started.\n");
        return -1;
    }
    /*
     * Start critical area
     */
    int file_d = open(FileName, O_RDONLY);
    int16_t cur_size = 0;
    char* prebuf = (char*)calloc(buffer_size, sizeof(*prebuf));
    int end4K_file_flag = 0;

    while ((cur_size = read(file_d, prebuf, buffer_size)) > 0 || \
            end4K_file_flag == 0) {
        if (cur_size == 0 && end4K_file_flag == 0)
            end4K_file_flag = 1;
        sops[0].sem_num = 0;
        sops[0].sem_op = 0;
        sops[0].sem_flg = SEM_UNDO;
        sops[1].sem_num = 0;
        sops[1].sem_op = 1;
        sops[1].sem_flg = SEM_UNDO;
        semop(m_empt, sops, 2);
        
        semop(m_mutx, sops, 2);

        buf[0] = (char)(cur_size & 0x00FF);
        buf[1] = (char)((uint16_t)(cur_size & 0xFF00) >> 8);
        memcpy(&(buf[2]), prebuf, cur_size); 
        fprintf(stderr, "Block %dB sent.\n", cur_size);
        
        sops[0].sem_num = 0;
        sops[0].sem_op = -1;
        sops[0].sem_flg = SEM_UNDO;
        semop(m_mutx, sops, 1);
        
        sops[0].sem_num = 0;
        sops[0].sem_op = 1;
        sops[0].sem_flg = SEM_UNDO;
        semop(m_full, sops, 1);
    }
    sops[0].sem_num = 0;
    sops[0].sem_op = 0;
    sops[0].sem_flg = SEM_UNDO;
    semop(m_empt, sops, 1);

    free(prebuf);
    close(file_d);
    /*
     * End critical area
     */
    sops[0].sem_num = 0;
    sops[0].sem_op = -1;
    sops[0].sem_flg = SEM_UNDO;
    semop(m_tran, sops, 1);

	return 0;
}
