#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#include "common.h"

/**
 * split_front_back() - Split list into half
 * @head: pointer to the head of list to be split
 * @front: pointer to the head of list which receives first half
 * @back: pointer to the head of list which receives second half
 *
 * At first, use fast/slow strategy to get the middle node. Cut it by
 * list_cut_position, and then use list_splice to transform the ownership.
 */
static void split_front_back(struct list_head *head,
                             struct list_head *front,
                             struct list_head *back)
{
    struct list_head *fast, *slow;

    slow = head->next;
    fast = slow->next;
    while (fast != head) {
        fast = fast->next;

        if (fast == head) {
            slow = slow->next;  // make slow ((n / 2) + 1)th element (1-based)
            break;
        }

        slow = slow->next;
        fast = fast->next;
    }

    list_cut_position(front, head, slow->prev);
    list_splice(head, back);
}

/**
 * merge() - Merge two lists.
 * @head: pointer to the head of list which receives sorted lists
 * @front: pointer to the head of list to be merged
 * @back: pointer to the head of list to be merged
 */
static void merge(struct list_head *head,
                  struct list_head *front,
                  struct list_head *back)
{
    struct listitem *left, *right;

    INIT_LIST_HEAD(head);

    left = list_first_entry(front, struct listitem, list);
    right = list_first_entry(back, struct listitem, list);

    while (!list_empty(front) && !list_empty(back)) {
        if (cmpint(&left->i, &right->i) < 0) {
            list_move_tail(&left->list, head);
            left = list_first_entry(front, struct listitem, list);
        } else {
            list_move_tail(&right->list, head);
            right = list_first_entry(back, struct listitem, list);
        }
    }
    list_splice_tail(!list_empty(front) ? front : back, head);
}

/**
 * list_mergesort() - Sort the list
 * @head: the list to sort
 *
 * This function implements "merge sort", which has O(nlog(n))
 * complexity.
 */
static void list_mergesort(struct list_head *head)
{
    if (head == NULL || list_empty(head) || list_is_singular(head)) {
        return;
    }

    struct list_head list_front, list_back;

    INIT_LIST_HEAD(&list_front);
    INIT_LIST_HEAD(&list_back);

    split_front_back(head, &list_front, &list_back);

    list_mergesort(&list_front);
    list_mergesort(&list_back);

    merge(head, &list_front, &list_back);
}

int main(int argc, const char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s input_size\n", argv[0]);
        return -1;
    }
    struct list_head testlist;
    struct listitem *item, *is = NULL;
    size_t i, array_size = strtol(argv[1], NULL, 10);
    uint16_t *values = (uint16_t *) malloc(sizeof(uint16_t) * array_size);

    random_shuffle_array(values, (uint16_t) array_size);

    INIT_LIST_HEAD(&testlist);

    assert(list_empty(&testlist));

    for (i = 0; i < array_size; i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        list_add_tail(&item->list, &testlist);
    }

    assert(!list_empty(&testlist));

    qsort(values, array_size, sizeof(values[0]), cmpint);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    list_mergesort(&testlist);
    clock_gettime(CLOCK_MONOTONIC, &end);

    printf("%lf\t", time_diff(&start, &end));

    i = 0;
    list_for_each_entry_safe (item, is, &testlist, list) {
        assert(item->i == values[i]);
        list_del(&item->list);
        free(item);
        i++;
    }

    assert(i == array_size);
    assert(list_empty(&testlist));

    free(values);

    return 0;
}
