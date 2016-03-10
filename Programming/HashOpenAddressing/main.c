#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include "limits.h"
#include "sys/resource.h"
#include "sys/time.h"
#include "errno.h"
#include "string.h"

#define TEST_CAPTION(test_name)                                   \
    do {                                                          \
        fprintf(stderr, "\033[1m\033[31m->\033[0m %-80s:",        \
                (test_name));                                     \
    } while (0);

#define TEST_RESULT(condition)                                    \
    do {                                                          \
    if ((condition))                                              \
        fprintf(stderr, " \033[92m%s\033[0m\n", "[SUCCESS]");     \
    else                                                          \
        fprintf(stderr, " \033[91m[FAILURE]\033[0m _REASON: %s\n",\
                strerror(errno));                                 \
    } while (0);

/*
 * My hash dump, which uses iterator
 */
int hash_dump(hash_t* object);

int main(int argc, char* argv[]) {
    const unsigned H_SIZE = 5;    
    char text_1[15] = {"hello, world"};
    char text_2[15] = {"semiconductors"};
    struct rlimit rlim;
    hash_t* hash_table_1 = NULL;
    hash_t* hash_table_2 = NULL;
    /*
     * Test hash_create
     */
    TEST_CAPTION("hash_create(limit memory mode)");
    getrlimit(RLIMIT_AS, &rlim);
    rlim.rlim_cur = 0;
    setrlimit(RLIMIT_AS, &rlim);
    hash_table_2 = hash_create(H_SIZE, hash_char);
    rlim.rlim_cur = RLIM_INFINITY;
    setrlimit(RLIMIT_AS, &rlim);
    TEST_RESULT(hash_table_2 == NULL && errno == ENOMEM);

    TEST_CAPTION("hash_create(huge hash table(MAX_INT))");
    hash_table_2 = hash_create(INT_MAX, hash_char);
    TEST_RESULT(hash_table_2 == NULL && errno == ENOMEM)
    
    TEST_CAPTION("hash_create(zero size)");
    hash_table_2 = hash_create(0, hash_char);
    TEST_RESULT(hash_table_2 == NULL && errno == EINVAL)

    TEST_CAPTION("hash_create(invalid hash function)");
    hash_table_2 = hash_create(H_SIZE, NULL);
    TEST_RESULT(hash_table_2 == NULL && errno == EINVAL)

    TEST_CAPTION("hash_create(5 elements) and dump");
    hash_table_1 = hash_create(H_SIZE, hash_char);
    TEST_RESULT(hash_table_1 != NULL)
    hash_dump(hash_table_1);
    
    /*
     * Test hash_insert 
     */
    TEST_CAPTION("hash_insert(invalid object)");
    TEST_RESULT(hash_insert(NULL, text_1) == EXIT_FAILURE && errno == EINVAL)
   
    TEST_CAPTION("hash_insert(add element to empty hash) and dump");
    TEST_RESULT(hash_insert(hash_table_1, text_1) == EXIT_SUCCESS)
    hash_dump(hash_table_1);

    TEST_CAPTION("hash_insert(first collision) and dump"); 
    TEST_RESULT(hash_insert(hash_table_1, text_2) == EXIT_SUCCESS)
    hash_dump(hash_table_1);
    
    TEST_CAPTION("hash_insert(try to add element in overflowed hash table) and dump");
    hash_insert(hash_table_1, text_1);
    hash_insert(hash_table_1, text_1);
    hash_insert(hash_table_1, text_1);
    TEST_RESULT(hash_insert(hash_table_1, text_2) == EXIT_FAILURE && errno == EOVERFLOW)
    hash_dump(hash_table_1);

    /*
     * Test hash_delete 
     */
    TEST_CAPTION("hash_delete(invalid object)");
    TEST_RESULT(hash_delete(NULL, text_1) == EXIT_FAILURE && errno == EINVAL)
    
    TEST_CAPTION("hash_delete(usually deleting) and dump");
    TEST_RESULT(hash_delete(hash_table_1, text_2) == EXIT_SUCCESS)
    hash_dump(hash_table_1);

    TEST_CAPTION("hash_delete(noexist element) and dump");
    TEST_RESULT(hash_delete(hash_table_1, text_2) == EXIT_FAILURE)
    hash_dump(hash_table_1);

    TEST_CAPTION("hash_delete(try to delete from empty h_table) and dump");
    hash_table_2 = hash_create(H_SIZE, hash_char);
    TEST_RESULT(hash_delete(hash_table_2, text_2) == EXIT_FAILURE)
    hash_dump(hash_table_2);

    /*
     * Test hash_destroy 
     */
    TEST_CAPTION("hash_destroy(invalid arguments)");
    TEST_RESULT(hash_destroy(NULL) == EXIT_FAILURE && errno == EINVAL)

    TEST_CAPTION("hash_destroy(valid hash table)");
    TEST_RESULT(hash_destroy(hash_table_2) == EXIT_SUCCESS);
    
    /*
     * Test hash_iter_*
     */
    TEST_CAPTION("hash_iter_current(invalid args)");
    TEST_RESULT(hash_iter_current(NULL) == (data_t)NULL && errno == EINVAL);

    TEST_CAPTION("hash_iter_next(invalid args)");
    TEST_RESULT(hash_iter_next(NULL) == EXIT_FAILURE && errno == EINVAL);
    
    TEST_CAPTION("hash_iter_reset(invalid args)");
    TEST_RESULT(hash_iter_reset(NULL) == EXIT_FAILURE && errno == EINVAL);
 
    TEST_CAPTION("hash_iter_current(valid hash table)");
    TEST_RESULT(strcmp(hash_iter_current(hash_table_1), text_1) == 0);

    TEST_CAPTION("hash_iter_next(valid hash table)");
    TEST_RESULT(hash_iter_next(hash_table_1) == EXIT_SUCCESS && \
                strcmp(hash_iter_current(hash_table_1), text_1) == 0);

    TEST_CAPTION("hash_iter_reset(valid hash table)");
    TEST_RESULT(hash_iter_reset(hash_table_1) == EXIT_SUCCESS && \
                strcmp(hash_iter_current(hash_table_1), text_1) == 0);

    /*
     * Test hash_search
     */
    hash_destroy(hash_table_1);   
    hash_table_1 = hash_create(H_SIZE, hash_char);

    TEST_CAPTION("hash_search(invalid args)");
    TEST_RESULT(hash_search(NULL, text_1) == (data_t)NULL && errno == EINVAL);

    TEST_CAPTION("hash_search(clear stack without 'deleted' places)");
    TEST_RESULT(hash_search(hash_table_1, text_1) == (data_t)NULL && \
                errno == EAGAIN);
    
    TEST_CAPTION("hash_search(normal mode)");
    hash_insert(hash_table_1, text_1);
    TEST_RESULT(strcmp(hash_search(hash_table_1, text_1), text_1) == 0);

    TEST_CAPTION("hash_search(doesn't exist element)");
    TEST_RESULT(hash_search(hash_table_1, text_2) == (data_t)NULL && \
                errno == EAGAIN);

    TEST_CAPTION("hash_search(clear stack with all 'deleted' places)");
    hash_insert(hash_table_1, text_1);
    hash_insert(hash_table_1, text_1);
    hash_insert(hash_table_1, text_1);
    hash_insert(hash_table_1, text_1);
    hash_delete(hash_table_1, text_1);
    hash_delete(hash_table_1, text_1);
    hash_delete(hash_table_1, text_1);
    hash_delete(hash_table_1, text_1);
    hash_delete(hash_table_1, text_1);
    TEST_RESULT(hash_search(hash_table_1, text_1) == (data_t)NULL && \
                errno == EAGAIN);

    /* 
     * All tests passed
     */
    hash_destroy(hash_table_1);
    exit(EXIT_SUCCESS);
}

int hash_dump(hash_t* object) {
    int i = 0;
    data_t buffer = NULL;
    
    fprintf(stderr, "\033[034m**************\033[33mHASH DUMP\033[034m"
            "*************\033[0m\n");
    hash_iter_reset(object);
    for (i = 0; i < object->size; i++) {
        buffer = hash_iter_current(object);
        if (buffer == NULL)
            fprintf(stderr, "%d: (NULL)\n", i);
        else
            fprintf(stderr, "%d: %s\n", i, buffer);
        hash_iter_next(object);
    }
    fprintf(stderr, "\033[034m************************************\033[0m\n");
    return EXIT_SUCCESS;
}
