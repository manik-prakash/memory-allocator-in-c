#include <stdio.h>
#include <stddef.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdlib.h>
typedef struct
{
    unsigned char *base;
    size_t size;
    size_t offset;
} arena;

const size_t alignment = alignof(max_align_t);

int arena_init(size_t arena_size, arena *a)
{
    if(a == NULL){
        return 0;
    }
    a->base = NULL;
    a->offset = 0;
    a->size = 0;
    if (arena_size == 0)
    {
        a->base = NULL;
        a->offset = 0;
        a->size = arena_size;
        return 0;
    }
    unsigned char *memory = malloc(arena_size);
    if (memory == NULL)
    {
        return 0;
    }
    a->base = memory;
    a->offset = 0;
    a->size = arena_size;

    return 1;
}

void *mem_alloc(arena *a, size_t x, size_t *total_padding)
{
    if (x == 0 || a == NULL || total_padding == NULL || a->base == NULL)
    {
        printf("invalid request \n");
        return NULL;
    }
    size_t aligned_offset = 0;
    if (a->offset <= SIZE_MAX - (alignment - 1) && a->offset <= a->size)
    {
        aligned_offset = (a->offset + (alignment - 1)) & ~(alignment - 1);
    }
    else
    {
        return NULL;
    }
    size_t padding = aligned_offset - a->offset;

    if (aligned_offset > a->size || x > a->size - aligned_offset)
    {
        printf("not possible\n");
        return NULL;
    }
    *total_padding += padding;

    unsigned char *allocated_start = a->base + padding + a->offset;
    a->offset = a->offset + padding + x;
    unsigned char *allocated_end = allocated_start + x;

    printf("padding added to the memory: %zu \n", padding);
    printf("starting address: %p \n", (void *)allocated_start);
    printf("ending address: %p \n", (void *)allocated_end);
    printf("new arena offset : %zu \n", a->offset);

    return allocated_start;
}

int arena_reset(arena *a, size_t *total_padding)
{
    if (a == NULL)
    {
        return 0;
    }
    a->offset = 0;
    if (total_padding != NULL)
    {
        *total_padding = 0;
    }
    return 1;
}

int arena_destroy(arena *a)
{
    if (a == NULL)
    {
        return 0;
    }
    free(a->base);
    a->base = NULL;
    a->offset = 0;
    a->size = 0;
    return 1;
}

int main()
{
    size_t arena_size;
    printf("define the arena size : ");
    if (scanf("%zu", &arena_size) != 1)
    {
        printf("invalid input\n");
        return 1;
    }

    arena a;
    int res = arena_init(arena_size, &a);
    if (res == 0)
    {
        return 1;
    }

    printf("arena size : %zu \n", a.size);
    printf("arena offset : %zu \n", a.offset);

    size_t total_padding = 0;

    int *num = mem_alloc(&a, 4 * sizeof(int), &total_padding);
    char *buffer = mem_alloc(&a, 100, &total_padding);

    printf("num : %p \n", num);
    printf("buffer : %p \n", buffer);
    printf("total padding : %zu \n", total_padding);
    printf("------------\n");
    arena_reset(&a, &total_padding);

    printf("arena offset after reset: %zu \n", a.offset);
    printf("total padding after reset: %zu \n", total_padding);
    printf("------------\n");
    int *num1 = mem_alloc(&a, 6 * sizeof(int), &total_padding);
    char *buffer1 = mem_alloc(&a, 400, &total_padding);

    printf("num : %p \n", num1);
    printf("buffer : %p \n", buffer1);
    printf("total padding : %zu \n", total_padding);

    arena_destroy(&a);

    return 0;
}
