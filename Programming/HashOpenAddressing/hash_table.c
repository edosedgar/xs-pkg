#include "hash_table.h"
#include <stdlib.h>

/**
 * @file   hash_table.c
 * @author Edgar kaziahmedov
 * @date   unkown
 * @brief  Library(source) for hash table with open addressing.
 */

/*****************************************************************************/

unsigned hash_char(data_t data) {   
    return 0;
}

hash_t* hash_create(unsigned size, unsigned (*hash_f)(data_t)) {
    return NULL;
}

int hash_insert(hash_t* object, data_t data) {
    return 0;
}

data_t hash_search(hash_t* object, data_t data) {
    return (data_t)NULL;
}

int hash_delete(hash_t* object, data_t data) {
    return 0;
}

int hash_destroy(hash_t* object) {
    return 0;
}

/*****************************************************************************/
