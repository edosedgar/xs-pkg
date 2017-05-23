#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "thread_pool.h"

/*
 *
 */
thread_pool_t* thread_pool_create(size_t max_queue_size,
                                  size_t max_workers)
{
        int i;
        /*
         * Allocate memory and set to default values
         */
        if (max_queue_size == 0 || max_workers == 0) {
                errno = EINVAL;
                return NULL;
        }

        thread_pool_t* thr_pool = (thread_pool_t*)malloc(
                                   sizeof(thread_pool_t));
        if (thr_pool == NULL) {
                errno = ENOMEM;
                return NULL;
        }
        thr_pool->max_workers = max_workers;
        thr_pool->max_queue_size = max_queue_size;
        thr_pool->finalize = TPOOL_ONGOING;
        if (pthread_mutex_init(&(thr_pool->lock), NULL) != 0 ||
            pthread_cond_init(&(thr_pool->notify), NULL) != 0 ||
            pthread_mutex_init(&(thr_pool->pulse_lock), NULL) != 0 ||
            pthread_cond_init(&(thr_pool->pulse), NULL) != 0) {
                free(thr_pool);
                return NULL;
        }

        thread_queue_t* thr_queue = (thread_queue_t*)malloc(
                                     sizeof(thread_queue_t));
        if (thr_queue == NULL) {
                errno = ENOMEM;
                free(thr_pool);
                return NULL;
        }
        thr_pool->thr_queue = thr_queue;
        thr_queue->queue_size = 0;
        thr_queue->in = NULL;
        thr_queue->out = NULL;
        thr_queue->first = NULL;
        thr_queue->last = NULL;

        thread_node_t* thr_buf = NULL;
        thread_node_t* thr_node = NULL;
        for (i = 0; i < max_queue_size; i++ ) {
                thr_node = (thread_node_t*) malloc(max_queue_size *
                                                   sizeof(thread_node_t));
                thr_node->prev = thr_buf;
                if (thr_buf != NULL)
                        thr_buf->next = thr_node;
                else
                        thr_pool->thr_queue->first = thr_node;
                thr_node->worker = NULL;
                thr_node->args = NULL;
                thr_node->flag = NODE_FLAG_EMPTY; 
                thr_buf = thr_node;
        } 
        thr_node->next = NULL;
        thr_pool->thr_queue->last = thr_node;
        thr_pool->thr_queue->last->next = thr_pool->thr_queue->first;
        thr_pool->thr_queue->first->prev = thr_pool->thr_queue->last;
        /*
         * Gather information about machine
         */
        unsigned processors = sysconf(_SC_NPROCESSORS_ONLN);
        if (max_workers > processors) {
                errno = ENOTSUP;
                free(thr_pool);
                free(thr_queue);
                return NULL;
        }
        /*
         * Create main workers using pthread
         */
        pthread_t* threads = (pthread_t*)malloc(max_workers *
                                                sizeof(*threads));
        pthread_attr_t attr;
        pthread_attr_init(&attr);

        for (i = 0; i < max_workers; i++) {
                pthread_create(&threads[i], &attr, thread_pool_exec,
                               thr_pool);
        }
        thr_pool->threads = threads;
        /*
         * Return created structure
         */
        return thr_pool;
}
/*
 *
 */
int thread_pool_add(thread_pool_t* thr_pool, worker_t worker, void* data)
{
        /*
         * Check out input
         */
        if (thr_pool == NULL || worker == NULL) {
                errno = EINVAL;
                return -1;
        }
        /*
         * Check for overflow
         */
        pthread_mutex_lock(&(thr_pool->lock));
        if (thr_pool->thr_queue->queue_size >= thr_pool->max_queue_size) {
                errno = EACCES;
                return -1;
        }
        /*
         * In case of accessing while destroying
         */
        if (thr_pool->finalize == TPOOL_SHUTDOWN)
                return -1;
        /*
         * Changing in and out pointers of ring buffer
         */
        if (thr_pool->thr_queue->in == NULL) {
                thr_pool->thr_queue->in = thr_pool->thr_queue->first;
                thr_pool->thr_queue->out = thr_pool->thr_queue->first;
        } else {
                thr_pool->thr_queue->in = thr_pool->thr_queue->in->prev;
        }
        thr_pool->thr_queue->queue_size++;

        thr_pool->thr_queue->in->worker = worker;
        thr_pool->thr_queue->in->flag = NODE_FLAG_EMPTY;
        thr_pool->thr_queue->in->args = data;
        /*
         * Wake up workers
         */
        pthread_cond_signal(&(thr_pool->notify));
        pthread_mutex_unlock(&(thr_pool->lock));
        return 0;
}
/*
 *
 */
void* thread_pool_exec(void* args)
{
        thread_pool_t* thr_pool = (thread_pool_t*)args;
        worker_t worker = NULL;
        void* w_args = NULL;

        for (;;) {
                pthread_mutex_lock(&(thr_pool->lock));
                /*
                 * In case empty queue, we have to be blocked
                 */
                while (thr_pool->thr_queue->queue_size == 0 &&
                       thr_pool->finalize == TPOOL_ONGOING) {
                        pthread_cond_wait(&(thr_pool->notify),
                                          &(thr_pool->lock));
                }
                /*
                 * It is used while destroying
                 */
                if (thr_pool->finalize == TPOOL_SHUTDOWN) {
                        break;
                }
                /*
                 * Pop thread from the top
                 */
                if (thr_pool->thr_queue->queue_size >= 1) {
                        worker = thr_pool->thr_queue->out->worker;
                        w_args = thr_pool->thr_queue->out->args;
                        if (thr_pool->thr_queue->queue_size == 1) {
                                thr_pool->thr_queue->out = NULL;
                                thr_pool->thr_queue->in = NULL;
                        } else {
                                thr_pool->thr_queue->out = 
                                        thr_pool->thr_queue->out->prev;
                        }
                        thr_pool->thr_queue->queue_size -= 1;
                }
                pthread_mutex_unlock(&(thr_pool->lock));
                /*
                 * Execute subroutine and send signal
                 */
                (*(worker))(w_args);
                pthread_mutex_lock(&(thr_pool->pulse_lock));
                pthread_cond_signal(&(thr_pool->pulse));
                pthread_mutex_unlock(&(thr_pool->pulse_lock));
        }
        pthread_mutex_unlock(&(thr_pool->lock));
        pthread_exit(NULL);
        return NULL;
}
/*
 *
 */
int thread_pool_destroy(thread_pool_t* thr_pool)
{
        int i;
        /*
         * Check input
         */
        if (thr_pool == NULL) {
                errno = EINVAL;
                return -1;
        }

        pthread_mutex_lock(&(thr_pool->lock));
        /*
         * destroy has been already launched
         */
        if (thr_pool->finalize == TPOOL_SHUTDOWN) {
                errno = EACCES;
                return -1;
        }
        /*
         * Wake up all
         */
        thr_pool->finalize = TPOOL_SHUTDOWN;
        pthread_cond_broadcast(&(thr_pool->notify));
        pthread_mutex_unlock(&(thr_pool->lock));

        for (i = 0; i < thr_pool->max_workers; i++) {
                pthread_join(thr_pool->threads[i], NULL);
        }
        /*
         * Free all structures
         */
        thread_node_t* cur_node = thr_pool->thr_queue->first;
        thread_node_t* next_node = NULL;
        if (cur_node != NULL)
                next_node = cur_node->next;
        for (cur_node = cur_node;
             cur_node != thr_pool->thr_queue->last;
             cur_node = next_node) {
                next_node = cur_node->next;
                free(cur_node);
        }
        free(thr_pool->thr_queue->last);
        free(thr_pool->thr_queue);
        pthread_mutex_destroy(&(thr_pool->lock));
        pthread_cond_destroy(&(thr_pool->notify));
        free(thr_pool);
        return 0;
}
/*
 *
 */
int thread_pool_wait_all(thread_pool_t* thr_pool)
{
        pthread_mutex_lock(&(thr_pool->pulse_lock));
        while (thr_pool->thr_queue->queue_size != 0) {
                pthread_cond_wait(&(thr_pool->pulse), 
                                  &(thr_pool->pulse_lock));
        }
        pthread_mutex_unlock(&(thr_pool->pulse_lock));
        return 0;
}
