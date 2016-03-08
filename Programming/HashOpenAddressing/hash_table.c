#include "hash_table.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * @file   hash_table.c
 * @author Edgar kaziahmedov
 * @date   unkown
 * @brief  Library(source) for hash table with open addressing.
 */

/*****************************************************************************/

unsigned hash_char(data_t data, unsigned size) {
    data = data;
    return 1U%size;
}

/*****************************************************************************/
hash_t* hash_create(unsigned size, unsigned (*hash_f)(data_t, unsigned)) {
    if (size == 0 || hash_f == NULL) {
        errno = EINVAL;
        return NULL;
    }
    hash_t* temp_hash = (hash_t*) calloc(1, sizeof(*temp_hash));
    if (temp_hash == NULL)
        return NULL;
    temp_hash->size = size;
    temp_hash->hash_f = hash_f;
    hash_element* temp_elem = (hash_element*) calloc(size, sizeof(*temp_elem));
    if (temp_elem == NULL) {
        free(temp_hash); 
        return NULL;
    }
    temp_hash->hash_data = temp_elem; 
    return temp_hash;
}

/*****************************************************************************/
int hash_insert(hash_t* object, data_t data) {
    if (object == NULL) {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    unsigned index = object->hash_f(data, object->size);
    if (object->hash_data[index].status == FREE ||
        object->hash_data[index].status == DELETED) {
        object->hash_data[index].data = data;
        object->hash_data[index].status = BUSY;
        return EXIT_SUCCESS;
    } else {
        unsigned cycle_var = index;
        while (object->hash_data[cycle_var].status == BUSY) {
            cycle_var++;
            cycle_var %= object->size;
            if (cycle_var == index)
                break;
        }
        if (cycle_var == index) {
            errno = EOVERFLOW;
            return EXIT_FAILURE;
        } else {
            object->hash_data[cycle_var].status = BUSY;
            object->hash_data[cycle_var].data = data;
        }
    }
    return EXIT_SUCCESS;
}

/*****************************************************************************/
data_t hash_search(hash_t* object, data_t data) {
    if (object == NULL) {
        errno = EINVAL;
        return (data_t)NULL;
    }
    data = data;
    return (data_t)NULL;
}

/*****************************************************************************/
int hash_delete(hash_t* object, data_t data) {
    if (object == NULL) {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    unsigned index = object->hash_f(data, object->size);
    if (object->hash_data[index].status == FREE)
        return EXIT_FAILURE;

    unsigned cycle_var = index;
    while (object->hash_data[cycle_var].status != FREE) {
        if (object->hash_data[cycle_var].status == BUSY &&
            memcmp(&(object->hash_data[cycle_var].data), &data, \
            sizeof(data)) == 0) {

            object->hash_data[cycle_var].data = (data_t)(NULL);
            object->hash_data[cycle_var].status = DELETED;
            return EXIT_SUCCESS;
        }
        cycle_var++;
        cycle_var %= object->size;
        if (cycle_var == index)
            break;
    }         
    return EXIT_FAILURE;
}

/*****************************************************************************/
int hash_destroy(hash_t* object) {
    if (object == NULL) {
        errno = EINVAL;
        return EXIT_SUCCESS;
    }
    object->size = 0;
    object->hash_f = NULL;
    free(object->hash_data);
    free(object);
    return 0;
}

/*****************************************************************************/
data_t hash_iter_current(hash_t* object) {
    if (object == NULL) {
        errno = EINVAL;
        return (data_t)NULL;
    }
    return object->hash_data[object->current_iter_pos].data; 
}

/*****************************************************************************/
int hash_iter_next(hash_t* object) {
    if (object == NULL) {
        errno = EINVAL;
        return EXIT_FAILURE;
    } 
    object->current_iter_pos++;
    return EXIT_SUCCESS;

}

/*****************************************************************************/
int hash_iter_reset(hash_t* object) {
    if (object == NULL) {
        errno = EINVAL;
        return EXIT_FAILURE;
    }    
    object->current_iter_pos = 0;
    return EXIT_SUCCESS;
}

/*****************************************************************************/
