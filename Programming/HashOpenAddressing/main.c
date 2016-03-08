#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include "limits.h"
#include "sys/resource.h"
#include "sys/time.h"

/*
 * My hash dump, whicn uses iterator
 */
int hash_dump(hash_t* object);

int main(int argc, char* argv[]) {
    const unsigned H_SIZE = 5;    
    char test[15] = {"hello, world"};
    char test2[15] = {"semiconductors"};
    struct rlimit rlim;
    /* FIrst test(Invalid argument)
     * try to create zero-size hash table
     * try to create huge table
     * try to send incorrect parameters to all methods
     */
    getrlimit(RLIMIT_AS, &rlim);
    rlim.rlim_cur = 0;
    setrlimit(RLIMIT_AS, &rlim);
    hash_t* hash_table = hash_create(H_SIZE, hash_char);
    rlim.rlim_cur = RLIM_INFINITY;
    setrlimit(RLIMIT_AS, &rlim);

    hash_table = hash_create(0, NULL);
    hash_table = hash_create(INT_MAX, hash_char);
    hash_destroy(hash_table);
    hash_search(NULL, NULL);
    hash_insert(NULL, NULL);
    hash_delete(NULL, NULL);
    hash_iter_current(NULL);
    hash_iter_next(NULL);
    hash_iter_reset(NULL);

    /*
     * Second test(Normal mode)
     * with checking on overflowing of hash table
     * try to delete from empty table
     * try to delete from table with only 'deleted' status
     */
    hash_table = hash_create(H_SIZE, hash_char);
    
    hash_delete(hash_table, test2);
    hash_insert(hash_table, test);
    hash_insert(hash_table, test);
    hash_insert(hash_table, test2);
    hash_insert(hash_table, test);
    hash_insert(hash_table, test2);
    hash_insert(hash_table, test2); 

    hash_dump(hash_table);  

    hash_delete(hash_table, test2);

    hash_dump(hash_table);

    hash_delete(hash_table, test2);
    hash_delete(hash_table, test);
    hash_delete(hash_table, test);
    hash_delete(hash_table, test);
    hash_delete(hash_table, test2);

    hash_dump(hash_table);

    hash_destroy(hash_table);
    
    exit(EXIT_SUCCESS);
}

int hash_dump(hash_t* object) {
    int i = 0;
    data_t buffer = NULL;
    
    fprintf(stderr, "********************************\n");
    hash_iter_reset(object);
    for (i = 0; i < object->size; i++) {
        buffer = hash_iter_current(object);
        if (buffer == NULL)
            fprintf(stderr, "%d: (NULL)\n", i);
        else
            fprintf(stderr, "%d: %s\n", i, buffer);
        hash_iter_next(object);
    }
    fprintf(stderr, "********************************\n");
    return EXIT_SUCCESS;
}
