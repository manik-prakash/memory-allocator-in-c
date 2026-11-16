#include <stdio.h>
#include <stddef.h>
#include <stdalign.h>
#include <stdint.h>

#define ARENA_SIZE 1024

typedef struct
{
    unsigned char *base;
    size_t size;
    size_t offset;
} arena;

alignas(max_align_t) static unsigned char arena_memory[ARENA_SIZE];
const size_t alignment = alignof(max_align_t);

void *mem_alloc(arena *a, size_t x, size_t *total_padding)
{
    if (x == 0)
    {
        printf("invalid memory request \n");
        return NULL;
    }
    size_t aligned_offset = (a->offset + (alignment - 1)) & ~(alignment - 1);
    size_t padding = aligned_offset - a->offset;
    total_padding += padding;

    if (aligned_offset > a->size || x > a->size - aligned_offset)
    {
        printf("not possible\n");
        return NULL;
    }

    unsigned char *allocated_start = a->base + padding + a->offset;
    a->offset = a->offset + padding + x;
    unsigned char *allocated_end = allocated_start + x;

    printf("padding added to the memory: %zu \n", padding);
    printf("starting address: %p \n", (void *)allocated_start);
    printf("ending address: %p \n", (void *)allocated_end);
    printf("new arena offset : %zu \n", a->offset);

    return allocated_start;
}

int main()
{

    arena a = {
        .base = arena_memory,
        .size = sizeof(arena_memory),
        .offset = 0};

    printf("arena size : %zu \n", a.size);
    printf("arena offset : %zu \n", a.offset);

    size_t total_padding = 0;

    int *num = mem_alloc(&a, 4 * sizeof(int), &total_padding);
    char *buffer = mem_alloc(&a, 100, &total_padding);

    printf("num : %p \n", num);
    printf("buffer : %p \n", buffer);
    printf("total padding : %zu \n",total_padding);
    
    return 0;
}
