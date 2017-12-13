#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "gc.h"
#include "linked_list.h"

node_t *
linked_list_create()
{
        node_t *head = malloc(sizeof(node_t));
        node_t *tail = malloc(sizeof(node_t));
        node_t *elem = malloc(sizeof(node_t));

        head->key = INT64_MIN;
        head->next = elem;
        head->back_link = NULL;
        elem->key = 0;
        elem->next = tail;
        elem->back_link = NULL;
        tail->key = INT64_MAX;
        tail->next = NULL;
        tail->back_link = NULL;

        return head;
}

/*
 *  The HelpMarked routine attempts to physically delete the marked
 *  node del_node.
 */
static void
HelpMarked(node_t *prev_node, node_t *del_node)
{
        node_t *next_node = P(del_node)->next;

        (void) _cas(&(P(prev_node)->next), M0F1(del_node), M0F0(next_node));
}

/*
 * The TryFlag routine attempts to flag the predecessor of prev_node
 */
static node_res_t
TryFlag(node_t *prev_node, node_t *target_node)
{
        node_res_t ret_val = {NULL, 0};
        node_range_t search_ret = {NULL, NULL};
        uint64_t result;

        for (;;) {
                /*
                 * Predecessor is already flagged. Report the failure,
                 * return a pointer to prev_node
                 */
                if ((uint64_t)(P(prev_node)->next) == M0F1(target_node)) {
                        ret_val.node = prev_node;
                        ret_val.result = 0;
                        return ret_val;
                }
                /*
                 * Flagging attempt
                 */
                result = (uint64_t) _cas(&(P(prev_node)->next),
                                         M0F0(target_node),
                                         M0F1(target_node));
                /*
                 * Successful flagging. Report the success, return
                 * a pointer to prev_node.
                 */
                if (result == M0F0(target_node)) {
                        ret_val.node = prev_node;
                        ret_val.result = 1;
                        return ret_val;
                }
                /*
                 * Failure due to flagging by a concurrent operation.
                 * Report the failure, return a pointer to prev_node.
                 */
                if (result == M0F1(target_node)) {
                        ret_val.node = prev_node;
                        ret_val.result = 0;
                        return ret_val;
                }
                /*
                 * Possibly a failure due to marking. Traverse a chain
                 * of backinks to reach an unmarked node.
                 */
                while (GET_MARK(P(prev_node)) == 1)
                        prev_node = P(prev_node)->back_link;
                search_ret = linked_list_search_from(P(target_node)->key - 1,
                                                     prev_node);
                /*
                 * target_node got deleted. Report the failure, return
                 * no pointer
                 */
                if (P(search_ret.right) != P(target_node)) {
                        ret_val.node = NULL;
                        ret_val.result = 0;
                        return ret_val;
                }
        }
}

static void HelpFlagged(node_t *prev_node, node_t *del_node);

/*
 * The TryMark routine attempts to mark the node del_node
 */
static void
TryMark(node_t *del_node)
{
        node_t *next_node, *result;

        do {
                next_node = P(del_node)->next;
                result = (node_t *) _cas(&(P(del_node)->next),
                                         M0F0(next_node), M1F0(next_node));
                if ((((uint64_t)result) & 0x3) == 1)
                        HelpFlagged(del_node, P(result)->next);

        } while (GET_MARK(P(del_node)) == 0);
}

/*
 *  The HelpFlagged routine attempts to mark and physically delete the
 *  successor of the flagged node prev_node.
 */
static void
HelpFlagged(node_t *prev_node, node_t *del_node)
{
        P(del_node)->back_link = P(prev_node);
        if (GET_MARK(P(del_node)) == 0)
                TryMark(del_node);
        HelpMarked(prev_node, del_node);
}

/*
 * ==========================================================================
 * ==========================================================================
 */

search_res_t
linked_list_search(node_t *head, int64_t key)
{
        node_range_t pair = {NULL, NULL};
        search_res_t ret_val;

        ret_val.error = 0;
        pair = linked_list_search_from(key, head);
        if (pair.left->key == key)
                ret_val.elem = pair.left->element;
        else
                ret_val.error = 1;

        return ret_val;
}

node_range_t
linked_list_search_from(int64_t key, node_t *curr_node)
{
        node_range_t ret_val = {NULL, NULL};
        node_t *next_node = P(curr_node)->next;

        while (P(next_node)->key <= key) {
                /*
                 * Ensure that either next node is unmarked, or both
                 * curr_node and next_node are marked and curr_node
                 * was marked earlier.
                 */
                while (GET_MARK(P(next_node)) == 1 &&
                       (GET_MARK(P(curr_node)) == 0 ||
                        P(curr_node)->next != P(next_node))) {
                        if (P(curr_node)->next == P(next_node))
                                HelpMarked(curr_node, next_node);
                        next_node = P(curr_node)->next;
                }
                if (P(next_node)->key <= key) {
                        curr_node = next_node;
                        next_node = P(curr_node)->next;
                }
        }
        ret_val.left = P(curr_node);
        ret_val.right = P(next_node);
        return ret_val;
}

node_t *
linked_list_delete(node_t *head, int64_t key)
{
        /*
         * Return if HEAD or TAIL
         */
        if (key == INT64_MIN || key == INT64_MAX)
                return NULL;
        node_range_t ret_val = {NULL, NULL};
        node_res_t try_flag_res = {NULL, 0};

        ret_val = linked_list_search_from(key - 1, head);
        /*
         * key is not found in the list
         */
        if (P(ret_val.right)->key != key)
                return NULL;
        try_flag_res = TryFlag(ret_val.left, ret_val.right);
        if (try_flag_res.node)
                HelpFlagged(try_flag_res.node, ret_val.right);
        if (!try_flag_res.result)
                return NULL;
        return P(ret_val.right);
}

int
linked_list_insert(node_t *head, int64_t key, data_t elem)
{
        node_range_t ret_val = {NULL, NULL};
        node_t *new_node, *prev_succ;
        uint64_t result;
         /*
         * Return if HEAD or TAIL
         */
        if (key == INT64_MIN || key == INT64_MAX)
                return -1;
        /*
         * prev_node.key ≤ k < next_node.key
         */
        ret_val = linked_list_search_from(key, head);
        if (P(ret_val.left)->key == key)
                return -1;
        new_node = malloc(sizeof(node_t));
        new_node->key = key;
        new_node->element = elem;
loop:
        prev_succ = P(ret_val.left)->next;
        /*
         * If the predecessor is flagged, help
         * he corresponding deletion to complete.
         */
        if (GET_FLAG(P(prev_succ)) == 1)
                HelpFlagged(ret_val.left, P(prev_succ)->next);
        else {
                new_node->next = (node_t *)M0F0(ret_val.right);
                /*
                 * Insertion attempt
                 */
                result = (uint64_t) _cas(&(P(ret_val.left)->next),
                                         M0F0(ret_val.right),
                                         M0F0(new_node));
                /*
                 * Successful insertion
                 */
                if (result == M0F0(ret_val.right))
                        return 0;
                else {
                        /*
                         * Failure due to flagging.
                         * Help complete the corresponding deletion.
                         */
                        if ((result & 0x3) == 1)
                                HelpFlagged(ret_val.left, P(result)->next);
                        /*
                         * Possibly a failure due to marking. Traverse
                         * a chain of backlinks to reach an unmarked
                         * node.
                         */
                        while (GET_MARK(P(ret_val.left)) == 1)
                                ret_val.left = P(ret_val.left)->back_link;
                }
        }
        /*
         * prev_node.key ≤ k < next_node.key
         */
        ret_val = linked_list_search_from(key, head);
        if (P(ret_val.left)->key == key) {
                free(new_node);
                return -1;
        }
        goto loop;
}
