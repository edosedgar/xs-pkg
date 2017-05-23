#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>

#define NODE_FLAG_EMPTY  (0U)
#define NODE_FLAG_LOADED (1U << 0)

#define TPOOL_ONGOING  (1U)
#define TPOOL_SHUTDOWN (0U)

/* NOTE:
 * Regardless of title queue in the code, actually, thread_pool is based on
 * ring buffer(HINT: take a look at thread_create)
 */
typedef int (*worker_t)(void* data);
/*
 *
 */
struct thread_node {
        struct thread_node* prev;
        struct thread_node* next;
        worker_t worker;
        void* args;
        /* It's supposed to be a future feature */
        unsigned flag;
};
/*
 *
 */
typedef struct thread_node thread_node_t;
/*
 *
 */
typedef struct {
        thread_node_t* first;
        thread_node_t* last;
        thread_node_t* out;
        thread_node_t* in;
        size_t queue_size;
} thread_queue_t;
/*
 *
 */
typedef struct {
        thread_queue_t* thr_queue;
        size_t max_workers;
        size_t max_queue_size;
        int finalize;
        pthread_mutex_t lock;
        pthread_cond_t notify;
        pthread_mutex_t pulse_lock;
        pthread_cond_t pulse;
        pthread_t* threads;
} thread_pool_t;
/*
 *
 */
thread_pool_t* thread_pool_create(size_t max_queue_size, size_t max_workers);
/*
 *
 */
int thread_pool_add(thread_pool_t* thr_pool, worker_t worker, void* data);
/*
 *
 */
void* thread_pool_exec(void* args);
/*
 *
 */
int thread_pool_destroy(thread_pool_t* thr_pool);
/*
 *
 */
int thread_pool_wait_all(thread_pool_t* thr_pool);

#endif
