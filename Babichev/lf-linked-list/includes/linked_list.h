/*
 *  This implementation of lock-free singly linked list
 *            is based on the article entitled:
 *
 *          "LOCK-FREE LINKED LISTS AND SKIP LISTS"
 *                   MIKHAIL FOMITCHEV
 *
 * A thesis submitted to the Faculty of Graduate Studies in
 *         partial fulfilment of the requirements
 *                   for the degree of
 *                   Master of Science
 *
 *         Graduate Programme in Computer Science
 *                    York University
 *          North York, Ontario November, 2003
 *
 * Link: http://www.cse.yorku.ca/~ruppert/Mikhail.pdf
 *
 * Implemented by: Edgar Kaziakhmedov,
 *                 Moscow Institute of Physics Mathematics,
 *                 edos.edgar@gmail.com
 */

#ifndef _LINKED_LIST_
#define _LINKED_LIST_

#include <stdint.h>

typedef int64_t data_t;
#define _cas __sync_val_compare_and_swap

/*
 * succ_field is a successor field, which itself consists of three fields:
 * right, mark, and flag. Right is  a pointer to the next node in the list.
 * Mark and flag are both 1-bit fields, which reflect the state of the node.
 * If both of these fields are 0, the node is in a normal state. If flag = 1,
 * then the node is flagged, which means that its successor node is about to
 * get deleted from the list. If mark = 1, it means that the node is marked as
 * deleted. Both flag and mark cannot be equal to 1 at the same time.
 */

typedef union succ_field {
        uint64_t next;
        struct {
                uint64_t f:1;
                uint64_t m:1;
                uint64_t addr:62;
        };
} succ_field_t;

#define GET_MARK(node) ((((uint64_t)(node->next)) & 0x2) == 0x2)
#define GET_FLAG(node) ((((uint64_t)(node->next)) & 0x1) == 0x1)

#define M1F1(node) (((uint64_t)node) | 0x3)
#define M0F0(node) (((uint64_t)node) & ~((uint64_t)0x3))
#define M1F0(node) (M0F0(node) | 0x2)
#define M0F1(node) (M0F0(node) | 0x1)

/*
 * Each node in our data structure has the following fields
 * Struct size = 24-byte
 */
typedef struct node {
        int64_t key;
        data_t element;
        struct node *back_link;
        struct node *next;
} node_t;

/* Make first two bits to be zero before using */
#define P(node) ((node_t *)M0F0(node))

/*
 * Special pair of node_t for list_search_from
 */
typedef struct node_range {
        node_t *left;
        node_t *right;
} node_range_t;

/*
 * Special set of node_t and bool for TryMark
 */
typedef struct node_res {
        node_t *node;
        int result;
} node_res_t;

/*
 * For search
 */
typedef struct search_res {
        data_t elem;
        int error;
} search_res_t;

/*
 * Create linked list (should be done one time)
 */
node_t *linked_list_create();

/*
 * The Search routine searches for a node with the supplied key
 */
search_res_t linked_list_search(node_t *head, int64_t key);

/*
 * The SearchFrom routine finds two consecutive nodes such that: the first
 * has the key less or equal than k, and the second has the key strictly
 * greater than k.
 */
node_range_t linked_list_search_from(int64_t key, node_t *curr_node);

/*
 * The Delete routine attempts to delete a node with the supplied key
 */
node_t *linked_list_delete(node_t *head, int64_t key);

/*
 * The Insert routine attempts to insert a new node into the list
 */
int linked_list_insert(node_t *head, int64_t key, data_t elem);

#endif /* !_LINKED_LIST_ */
