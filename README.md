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
