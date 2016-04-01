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

typedef struct {
    double start_lim;
    double end_lim;
    double result;
} thread_data;

/*
 * Global constants
 */
double step = 1/10000000.0L;
double start = -100;
double end = 100;

/*
 * Subroutine is executed in parallel
 */
void* calc_integral(void* args) {
    //fprintf(stderr, "ID: %lu, CPU: %u\n", pthread_self(), sched_getcpu());    
    thread_data* info = (thread_data*)args;
    double val = 0;
    double result = 0;
    double local_step = step;
    for (val = (info->start_lim); val < (info->end_lim); val += local_step) { 
        result += (local_step / 6) * \
                  (val * val + \
                   4 * (val + local_step / 2) * (val + local_step / 2) + \
                   (val + local_step) * (val + local_step));
    }
    info->result = result;
    //fprintf(stderr, "suka:%lg %lg %lg\n", info->start_lim, info->end_lim, result);
    return EXIT_SUCCESS;
}

/*
 * Main program
 */
int main(int argc, char* argv[]) {
    /*
     * Initial routines
     */
    unsigned n_threads = 0;
    unsigned processors = 0;
    unsigned i = 0;
    if (argc != 2) 
        err_exit(EINVAL);
    else if (sscanf(argv[1], "%u", &n_threads) == 0)
        err_exit(EINVAL);
    /*
     * Print number of online processors
     */
    processors = sysconf(_SC_NPROCESSORS_ONLN);
    fprintf(stderr, "Processors: %u\nThreads: %u\n", processors, n_threads);
    if (n_threads > processors)
        err_exit(ENOTSUP);
    /*
     * Bind to processor and start to calculate
     */
    pthread_t* threads = (pthread_t*)malloc(n_threads * sizeof(*threads));
    pthread_attr_t attr;
    cpu_set_t cpus;
    pthread_attr_init(&attr);
    thread_data* info = (thread_data*)malloc(n_threads * sizeof(*info));

    for (i = 0; i < n_threads; i++ ) {
        info[i].start_lim = ((end - start) / n_threads) * i + start;
        info[i].end_lim = ((end - start) / n_threads) * (i + 1) + start;
        info[i].result = 0;
    }

    for (i = 0; i < n_threads; i++) {
        CPU_ZERO(&cpus);
        switch (i) {
            case 0: CPU_SET(0, &cpus); break;
            case 1: CPU_SET(2, &cpus); break;
            case 2: CPU_SET(1, &cpus); break;
            case 3: CPU_SET(3, &cpus); break;
        }
        pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpus);
        pthread_create(&threads[i], &attr, calc_integral, &(info[i]));
    }
    /*
     * Wait and summarize result
     */
    for (i = 0; i < n_threads; i++) 
        pthread_join(threads[i], NULL);

    for (i = 1; i < n_threads; i++)
        info[0].result += info[i].result;

    fprintf(stderr, "Result: %lg\n", info[0].result);
    free(threads);
    free(info);
    exit(EXIT_SUCCESS);
}
