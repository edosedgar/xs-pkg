#ifndef HASH_TABLE
#define HASH_TABLE

/**
 * @file   hash_table.h
 * @author Edgar Kaziahmedov
 * @date   unknown
 * @brief  Library(header) for hash table with open addressing.
 */

/*****************************************************************************/
    
/**
 * @brief  Main data type, using in hash table.
 *         Now it is string with length no more than 255.
 */
typedef char* data_t;

/**
 * @brief  State of 'deleted' flag.
 */
typedef enum {
    FREE = 0,
    BUSY = 1,
    DELETED = 2
} deleted_t;

/** 
 * @struct hash_element 
 * @brief  Additional structure for storing elements of hash table, 
 *         including data and special 'deleted' flag, which uses for 
 *         algorithm of open addressing.
 */
typedef struct {
    deleted_t status;
    data_t data;
} hash_element;

/**
 * @struct hash_type
 * @brief  General structure contains size and point on hash elements. 
 *         This is a main object in library. 
 */
typedef struct {
    unsigned size;
    long int current_iter_pos;
    hash_element* hash_data;
    unsigned (*hash_f)(data_t, unsigned);
} hash_t;

/*****************************************************************************/

/**
 * @brief  Simple hash function, which summarizes each ascii code of symbol 
 *         and returns it.
 * @param  data Data, which you hash.
 * @param  size Size of hash table.
 * @return Integer number f(data).
 */
unsigned hash_char(data_t data, unsigned size);

/**
 * @brief  Hash constructor gets size and returns object. Constructor
 *         allocates memory for hash table. 
 * @param  size The size of hash table.
 * @param  hash_f Hash function.
 * @return The initialized object. 
 *    <br> EINVAL Uncorrect parameters.
 *    <br> ENOMEM No memory.
 */
hash_t* hash_create(unsigned size, unsigned (*hash_f)(data_t, unsigned));

/**
 * @brief  Hash method for inserting object to table. 
 * @param  object Your object.
 * @param  data   Data for inserting.
 * @return The EXIT_SUCCESS or EXIT_FAILURE status.
 *    <br> EOVERFLOW Hash table is oversize.
 *    <br> EINVAL    Uncorrect parameters.
 */
int hash_insert(hash_t* object, data_t data);

/**
 * @brief  Hash method for searching object in table. 
 * @param  object Your object.
 * @param  data   Data for searching.
 * @return Function returns (data_t)NULL or data.
 *    <br> EAGAIN Data didn't found
 *    <br> EINVAL Uncorrect parameters.
 */
data_t hash_search(hash_t* object, data_t data);

/**
 * @brief  Hash method for deleting object in table.
 * @param  object Your object.
 * @param  data   Data for deleting.
 * @return If data has already been deleted, then subroutines returns 
 *         EXIT_FAILURE, otherwise EXIT_SUCCESS.
 *    <br> EINVAL Uncorrect parameters. 
 */
int hash_delete(hash_t* object, data_t data);

/**
 * @brief  Hash method for destroying hash table.
 * @param  object Your object.
 * @return The EXIT_SUCCESS or EXIT_FAILURE status.
 *    <br> EINVAL Uncorrect parameters.
 */ 
int hash_destroy(hash_t* object);

/**
 * @brief  Hash iterator for accessing to each element's.
 *   <br>  reset   - rewind index to start
 *   <br>  next    - get next element
 *   <br>  current - get current element
 *   <br>  When point will reach a finish, pointer automatically will reset.
 * @param  object Your object.
 * @return The EXIT_SUCCESS, EXIT_FAILURE or element, (data_t)NULL.
 *    <br> EINVAL Uncorrect parameters.
 */
data_t hash_iter_current(hash_t* object);
int hash_iter_next(hash_t* object);
int hash_iter_reset(hash_t* object);

/*****************************************************************************/
#endif
