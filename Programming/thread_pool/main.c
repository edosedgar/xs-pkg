#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <math.h>

#define err_exit(error)     \
    do {                    \
        errno = error;      \
        perror("");         \
        exit(EXIT_FAILURE); \
    } while (0)

/*
 * Main program
 */
int main(int argc, char* argv[]) {
    /*
     * Initial routines
     */
    unsigned n_threads = 6;
    //unsigned processors = 0;
    if (argc != 2)
        err_exit(EINVAL);
    else if (sscanf(argv[1], "%u", &n_threads) == 0)
        err_exit(EINVAL);
    /*
     * Print number of online processors
     */
    exit(EXIT_SUCCESS);
}
