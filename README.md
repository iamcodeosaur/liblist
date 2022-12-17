# Provided functions

- ``void list_set_allocator(void* (*)())`` -- set user's allocator;
- ``void list_set_freefn(void (*)(void*))`` -- set user's free function;
- ``void list_set_cleanupfn(struct list_entry*, void (*cleanup_fn)(void*))`` --
  set function that will be called for an entry of some list by ``list_free()``
  and ``list_destroy()``;
- ``void *list_alloc(size_t)`` -- this function allocate a valid list entry;
- ``void *list_dealloc(void*)`` -- this function free the memory allocated by
  ``list_alloc()``;
- ``void list_free(void*)`` -- delete an entry from the list;
- ``void list_destroy(void*)`` -- delete the whole list.

# Provided macros

- ``(typeof list) list_head(list)`` -- returns head of the list;
- ``(typeof list) list_tail(list)`` -- returns tail of the list;
- ``(typeof list) list_prev(list)`` -- returns previous entry of the list;
- ``(typeof list) list_next(list)`` -- returns next entry of the list;
- ``size_t list_length(list)`` -- returns length of the list;
- ``(typeof entry) list_insert(entry)`` -- allocate an entry before
  the provided entry or return an new entry if ``entry`` is NULL;
- ``(typeof entry) list_append(entry)`` -- allocate an entry after
  the provided entry or return an new entry if ``entry`` is NULL;
- ``(typeof entry) list_idx(entry, idx)`` -- return idx'th entry before
  the specified entry on negative ``idx`` or after on positive ``idx``.

# Example:

A small example is provided below.

```c
int
main(void)
{
    int i;
    struct point {size_t x, y;} *line = NULL;

    for (i = 0; i < 5; ++i) {
        line = list_append(line);
        line->x = i;
        line->y = i*i;
    }

    list_foreach (point, list_head(line))
        printf("(%zu, %zu)\n", point->x, point->y);

    printf("The 3rd point of the line is: %zu\n", list_idx(list_head(line), 2)->y);

    list_destroy(line);

    return 0;
}
```
