#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <math.h>
#include "thread_pool.h"

#define err_exit(error)     \
    do {                    \
        errno = error;      \
        perror("");         \
        exit(EXIT_FAILURE); \
    } while (0)

typedef struct {
        double int_from;
        double int_to;
        int order_num;
        int max_workers;
        double result;
        double step;
} process_data_t;

int task1(void* data)
{
        process_data_t* pdt = (process_data_t*)data;
        double from = (pdt->int_to - pdt->int_from)/pdt->max_workers;
        double to = from * (pdt->order_num + 1) + pdt->int_from;
        from = from * pdt->order_num + pdt->int_from;
        double val;
        double result = 0;
        double local_step = pdt->step;
        for (val = from; val < to; val += local_step) {
                result += (local_step / 6) * \
                          (val * val * val+ \
                           4 * (val + local_step / 2) * (val + local_step / 2) *
                           (val + local_step / 2) + (val + local_step) *
                           (val + local_step) * (val + local_step));
        }
        pdt->result = result;
        return 0;
}

/*
 * Main program
 */
int main(int argc, char* argv[])
{
        /*
         * Initial routines
         */
        unsigned n_threads = 0;
        unsigned n_task = 2;
        int i =0;
        //unsigned processors = 0;
        if (argc != 2)
            err_exit(EINVAL);
        else if (sscanf(argv[1], "%u", &n_threads) == 0)
            err_exit(EINVAL);
        /*
         * Create issues
         */
        process_data_t* pdt = (process_data_t*)malloc(n_task *
                                                      sizeof(process_data_t));
        for (i = 0; i < n_task; i++) {
                pdt[i].int_from = -10;
                pdt[i].int_to = 10;
                pdt[i].order_num = i;
                pdt[i].max_workers = n_task;
                pdt[i].step = 1/100000000.0L;
        }
        /*
         * Send tasks to execute
         */
        thread_pool_t* th_pool = thread_pool_create(10, n_threads);
        for (i = 0; i < n_task; i++)
                thread_pool_add(th_pool, task1, &(pdt[i]));
        thread_pool_wait_all(th_pool);
        thread_pool_destroy(th_pool);
        double result = pdt[0].result + pdt[1].result +
                        pdt[2].result + pdt[3].result;
        fprintf(stderr, "Result: %lg \n", result);
        free(pdt);
        return 0;
}
