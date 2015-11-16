#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/msg.h>

/*
 * Prototypes
 */

int8_t create_children(int32_t number);

/*
 * Main
 */

int main(int argc, char* argv [])
{
    int64_t number = 0;
    char* endptr = NULL;	
    const uint8_t base = 10;
	/*
     * Handler input data
     */
	switch (argc) {
    case 1:
        fprintf(stderr, "No arguments\n");
        exit(EXIT_FAILURE);
        break;
    case 2:
        number = strtol(argv[1], &endptr, base);
        if (argv[1] + strlen(argv[1]) != endptr) {
            fprintf (stderr, "Uncorrect input data (found symbols).\n");
            exit(EXIT_FAILURE);
        }
        if (number < 1) {
            fprintf (stderr, "Uncorrect interval(only from 1).\n");
            exit(EXIT_FAILURE);
        }
        if ((errno == ERANGE && (number == LONG_MAX || number == LONG_MIN)) || \
           (number > INT_MAX || number < INT_MIN )) {
            fprintf(stderr, "Too long for me.\n");
            exit(EXIT_FAILURE);				
        }
        break;
    default:
        fprintf (stderr, "Many arguments for me.\n");
        exit(EXIT_FAILURE);
    }	
    create_children(number);
    return (EXIT_SUCCESS);    
}

/*
 * create_children function
 * This function creates <number> chilren and
 * they print their numbers
 */

int8_t create_children(int32_t number) {
    unlink("/tmp/ipcmsg");
    mknod("/tmp/ipcmsg", S_IFREG | 0666, 0);

    key_t key_ipc = ftok("/tmp/ipcmsg", getpid());
    uint32_t id = msgget(key_ipc, IPC_CREAT | 0666);
    msgctl(id, IPC_RMID, NULL);
    id = msgget(key_ipc, IPC_CREAT | 0666);

    /*
     * Message struct
     */
    typedef struct msgbuf_s{
        long mtype;
        uint64_t c_number;
    } msgbuf_t;
    msgbuf_t msgbuf;
    int DataLength = sizeof(msgbuf_t)-sizeof(msgbuf.mtype);

    int i = 0;
    for (i = 1; i <= number; i++) {
        int childpid = fork();
        if (childpid == 0) {  
            if (msgrcv(id, &msgbuf, DataLength, i, 0) == -1)
                exit(EXIT_FAILURE);
            /*
             * Critical area start
             */
            fprintf(stdout, "%d\n", i);
            fflush(stdout);
            /*
             * Critical area end
             */
            msgbuf.mtype = i + 1;
            if (msgsnd(id, &msgbuf, DataLength, 0) == -1)
                exit(EXIT_FAILURE);
            exit(EXIT_SUCCESS);           
        }  
        if (childpid == -1) {
            perror("");
            msgctl(id, IPC_RMID, NULL);
            exit(EXIT_FAILURE);
        }
    }
    msgbuf.mtype = 1;
    msgsnd(id, &msgbuf, DataLength, 0);
    msgrcv(id, &msgbuf, DataLength, number + 1, 0);

    msgctl(id, IPC_RMID, NULL);
    return 0;
}
