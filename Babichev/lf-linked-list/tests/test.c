#include <stdio.h>

#include "linked_list.h"
#include "thread_pool.h"

#define THREAD_POOL_SIZE 100
#define WORKERS_NUM 4
#define TASKS_NUM 50

int
search_task(void *data)
{
        node_t *list_head = (node_t *)data;
        int key = 3;
        search_res_t buf = linked_list_search(list_head, key);
        (void) linked_list_delete(list_head, 0);
        if (buf.error)
                fprintf(stderr, "Element with key %d is not found\n", key);
        else
                fprintf(stderr, "Key: %d Element: %ld\n", key, buf.elem);

        return 0;
}

int
delete_task(void *data)
{
        node_t *list_head = (node_t *)data;
        (void) linked_list_insert(list_head, 3, 10);
        return 0;
}

int
main(int argc, char *argv[])
{
        int i;
        node_t *list_head;
        thread_pool_t* th_pool;

        list_head = linked_list_create();
        th_pool = thread_pool_create(THREAD_POOL_SIZE, WORKERS_NUM);

        for (i = 0; i < TASKS_NUM; i++)
                switch (i) {
                case 6:
                        thread_pool_add(th_pool, delete_task,
                                        (void *)list_head);
                        break;
                default:
                        thread_pool_add(th_pool, search_task,
                                        (void *)list_head);
                        break;
                }
        thread_pool_wait_all(th_pool);
        thread_pool_destroy(th_pool);
        return 0;
}
