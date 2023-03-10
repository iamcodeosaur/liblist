#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#include "list.h"

static inline void*
xcalloc(size_t size)
{
    return calloc(size, 1);
}

static void* (*__user_allocator)(size_t) = xcalloc;
static void  (*__user_free)(void*) = free;

static inline struct list_metadata*
meta_alloc()
{
    size_t size = sizeof(struct list_metadata);

    return memset(__user_allocator(size), 0, size);
}

static inline struct list_entry*
entry_alloc(size_t size)
{
    size_t entry_sz = sizeof(struct list_entry);

    return memset(__user_allocator(entry_sz + size), 0, entry_sz);
}

void*
list_alloc(size_t size)
{
    struct list_entry *entry = entry_alloc(size);

    entry->metadata = meta_alloc();
    entry->metadata->length = 1;
    entry->metadata->head = entry;
    entry->metadata->tail = entry;

    return __list_get_data(entry);
}

void
list_dealloc(void *mem)
{
    list_free(mem);
}

void
list_set_allocator(void* (*alloc_fn)(size_t))
{
    __user_allocator = alloc_fn ? alloc_fn : xcalloc;
}

void
list_set_freefn(void (*free_fn)(void*))
{
    __user_free = free_fn ? free_fn : free;
}

void
list_set_cleanup_fn(struct list_entry *entry, void (*cleanup_fn)(void*))
{
    if (!entry)
        return;

    entry->metadata->cleanup = cleanup_fn;
}

struct list_entry*
__list_get_entry(void *data)
{
    return !data ? NULL : (void*)((uint8_t*)data - sizeof(struct list_entry));
}

void*
__list_get_data(struct list_entry *entry)
{
    return !entry ? NULL : (uint8_t*)entry + sizeof(struct list_entry);
}

void*
__list_head(struct list_entry *list)
{
    return !list ? NULL : __list_get_data(list->metadata->head);
}

void*
__list_tail(struct list_entry *list)
{
    return !list ? NULL : __list_get_data(list->metadata->tail);
}

void*
__list_prev(struct list_entry *list)
{
    return !list ? NULL : __list_get_data(list->prev);
}

void*
__list_next(struct list_entry *list)
{
    return !list ? NULL : __list_get_data(list->next);
}

size_t
__list_length(struct list_entry *list)
{
    return !list ? 0 : list->metadata->length;
}

void
__list_free(struct list_entry *entry)
{
    if (!entry)
        return;

    if (entry->prev)
        entry->prev->next = entry->next;
    else
        entry->metadata->head = entry->next;

    if (entry->next)
        entry->next->prev = entry->prev;
    else
        entry->metadata->tail = entry->prev;

    if (entry->metadata->cleanup)
        entry->metadata->cleanup(__list_get_data(entry));

    if (!entry->prev && !entry->next)
        __user_free(entry->metadata);
    else
        entry->metadata->length -= 1;

    __user_free(entry);
}

void
__list_destroy(struct list_entry *entry)
{
    struct list_entry *next;
    void (*cleanup_fn)(void*);

    if (!entry)
        return;

    cleanup_fn = entry->metadata->cleanup;
    __user_free(entry->metadata);

    for (; entry; entry = next) {
        next = entry->next;

        if (cleanup_fn)
            cleanup_fn(__list_get_data(entry));

        __user_free(entry);
    }
}

void*
__list_insert(struct list_entry *list, size_t size)
{
    struct list_entry *entry = entry_alloc(size);

    if (!list)
        entry->metadata = meta_alloc();
    else
        entry->metadata = list->metadata;

    entry->metadata->length += 1;
    entry->next = list;

    if (entry->next) {
        entry->prev = entry->next->prev;
        entry->next->prev = entry;
    }
    else
        entry->metadata->tail = entry;

    if (entry->prev)
        entry->prev->next = entry;
    else
        entry->metadata->head = entry;

    return __list_get_data(entry);
}

void*
__list_append(struct list_entry *list, size_t size)
{
    struct list_entry *entry = entry_alloc(size);

    if (!list)
        entry->metadata = meta_alloc();
    else
        entry->metadata = list->metadata;

    entry->metadata->length += 1;
    entry->prev = list;

    if (entry->prev) {
        entry->next = entry->prev->next;
        entry->prev->next = entry;
    }
    else
        entry->metadata->head = entry;

    if (entry->next)
        entry->next->prev = entry;
    else
        entry->metadata->tail = entry;

    return __list_get_data(entry);
}

void*
__list_idx(struct list_entry *list, ssize_t idx)
{
    if (idx < 0) {
        for (; idx && list; ++idx, list = list->prev)
            ;
    }
    else {
        for (; idx && list; --idx, list = list->next)
            ;
    }

    return __list_get_data(list);
}
