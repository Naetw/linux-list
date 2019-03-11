#include <assert.h>
#include <stdlib.h>
#include "list.h"

#include "common.h"

static void list_insert_sorted(struct listitem *entry, struct list_head *head)
{
    struct listitem *item = NULL;

    if (list_empty(head)) {
        list_add(&entry->list, head);
        return;
    }

    list_for_each_entry (item, head, list) {
        if (cmpint(&entry->i, &item->i) < 0) {
            list_add_tail(&entry->list, &item->list);
            return;
        }
    }

    list_add_tail(&entry->list, head);
}

static void list_insertsort(struct list_head *head)
{
    struct list_head list_unsorted;
    struct listitem *item = NULL, *is = NULL;

    INIT_LIST_HEAD(&list_unsorted);
    list_splice_init(head, &list_unsorted);

    list_for_each_entry_safe (item, is, &list_unsorted, list) {
        list_del(&item->list);
        list_insert_sorted(item, head);
    }
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
    list_insertsort(&testlist);
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
