#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include <sys/prctl.h>
#include <sys/types.h>

/*
 * This define check bit
 */
#define IS_BIT_SET(byte, bit) ((((byte) >> (bit)) & 0x01) == 1)  

/*
 * Global constant
 */
#define BIT7 7
#define BIT0 0

/*
 * Prototype
 */
void set_bit_handler(int signo); 
void clr_bit_handler(int signo);
void child_handler(int signo);

/*
 * Global variable
 */
volatile sig_atomic_t sig_flag = 0;
int cur_bit = -1;
int cur_byte = 0;

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

int main(int argc, char* argv[]) {
    /*
     * Check input arguments
     */
    if (argc != 2) {
        fprintf(stderr, "Invalid arguments!\n");    
        exit(EXIT_FAILURE);
    }
    /*
     * Open file and check
     */
    int file_d = open(argv[1], O_RDONLY);
    if (file_d == -1) {
        fprintf(stderr, "Cannot open file.\n");
        exit(EXIT_FAILURE);
    }
    /*
     * Initialize handlers
     */
    struct sigaction act, oact;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = set_bit_handler;
    sigaction(SIGUSR1, &act, &oact);

    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = clr_bit_handler;
    sigaction(SIGUSR2, &act, &oact);

    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = child_handler;
    sigaction(SIGCHLD, &act, &oact);
    /*
     * Block all signals
     */
    sigset_t newmask, oldmask;
    sigfillset(&newmask);
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);
    /*
     * Prepare waitmask
     */
    sigset_t waitmask;
    sigfillset(&waitmask);
    sigdelset(&waitmask, SIGUSR1);
    sigdelset(&waitmask, SIGUSR2);
    sigdelset(&waitmask, SIGCHLD);
    /*
     * FORK
     */ 
    int temp_byte = 0;
    int parent_pid = getpid();
    int child_pid = fork();
    
    if (child_pid == 0) {
     /*
      * CHILD process
      */
     prctl(PR_SET_PDEATHSIG, SIGKILL, 0, 0, 0);
     if (getppid() != parent_pid)
         exit(EXIT_FAILURE);
     int i = 0;
     while (read(file_d, &temp_byte, 1) > 0){
         for (i = BIT0; i <= BIT7; i++){
             if (IS_BIT_SET(temp_byte, i) == 1)
                 kill(parent_pid, SIGUSR1);
             else
                 kill(parent_pid, SIGUSR2);
             sigsuspend(&waitmask);
         }
     } 
     close(file_d);
     exit(EXIT_SUCCESS);
    } else {
     /*
      * PARENT process
      */
     while (1) {
        if (sig_flag == 0)
            sigsuspend(&waitmask);
        if (cur_bit == BIT7) {
            write(STDOUT_FILENO, &(cur_byte), 1);
            cur_byte = 0;
        }
        sig_flag = 0;
        kill(child_pid, SIGUSR1);
     }
    }
    exit(EXIT_SUCCESS);
}

/*
 * SIGCHLD handler
 */
void child_handler(int signo){
    exit(EXIT_FAILURE);
}
/*
 * SIGUSR1 handler
 */
void set_bit_handler(int signo){
    cur_bit++;
    if (cur_bit == (BIT7 + 1))
        cur_bit = 0;
    sig_flag = 1;
    cur_byte |= (1 << cur_bit);
}

/*
 * SIGUSR2 handler
 */
void clr_bit_handler(int signo){
    cur_bit++;
    if (cur_bit == (BIT7 + 1))
        cur_bit = 0;
    sig_flag = 1;
}

