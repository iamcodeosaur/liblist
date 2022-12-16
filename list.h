#ifndef _LIST_H
#define _LIST_H

#include <stdlib.h>

struct list_entry {
    struct list_metadata {
        struct list_entry *head;
        struct list_entry *tail;
        size_t length;
        void (*free)(void*);
    } *metadata;

    struct list_entry *prev;
    struct list_entry *next;
};

#define list_foreach(entry, list)                                       \
    for(typeof(list) entry = list;                                      \
        entry;                                                          \
        entry = __list_get_entry(entry) ?                               \
                    __list_get_data(__list_get_entry(entry)->next) :    \
                    NULL)

void list_set_allocator(void* (*alloc_fn)(size_t));

struct list_entry *__list_get_entry(void *data);
void *__list_get_data(struct list_entry *entry);


#define list_head(list) ((typeof(list))__list_head(__list_get_entry(list)))
void *__list_head(struct list_entry *list);
#define list_tail(list) ((typeof(list))__list_tail(__list_get_entry(list)))
void *__list_tail(struct list_entry *list);
#define list_prev(list) ((typeof(list))__list_prev(__list_get_entry(list)))
void *__list_prev(struct list_entry *list);
#define list_next(list) ((typeof(list))__list_next(__list_get_entry(list)))
void *__list_next(struct list_entry *list);
#define list_length(list) __list_length(__list_get_entry(list))
size_t __list_length(struct list_entry *list);

#define list_free(entry) __list_free(__list_get_entry(entry))
void __list_free(struct list_entry *entry);
#define list_destroy(entry) __list_destroy(__list_get_entry(list_head(entry)));
void __list_destroy(struct list_entry *entry);


#define list_insert(list) __list_insert(__list_get_entry(list), sizeof(*list))
void *__list_insert(struct list_entry *list, size_t size);
#define list_append(list) __list_append(__list_get_entry(list), sizeof(*list))
void *__list_append(struct list_entry *list, size_t size);

#define list_idx(list, idx) ((typeof(list))__list_idx(__list_get_entry(list), idx))
void *__list_idx(struct list_entry *list, ssize_t idx);

#endif // _LIST_H
