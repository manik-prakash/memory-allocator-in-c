#include <stdio.h>
#include <stddef.h>
#include <stdalign.h>

#define ARENA_SIZE 1024

typedef struct
{
    unsigned char *base;
    size_t size;
    size_t offset;
} arena;

alignas(max_align_t) static unsigned char arena_memory[ARENA_SIZE];

int main()
{
    const size_t alignment = alignof(max_align_t);
    arena a = {
        .base = arena_memory,
        .size = sizeof(arena_memory),
        .offset = 0
    };
    printf("arena size : %zu \n", a.size);
    printf("arena offset : %zu \n", a.offset);

    int n;
    printf("how many times do u have to allocate memmory :");
    if (scanf("%d", &n) != 1)
    {
        printf("invalid input!!\n");
        return 1;
    }

    size_t total_padding = 0;

    while (n--)
    {
        size_t x;
        printf("enter the amount of memory u want to be allocated(in Bytes) :");
        if (scanf("%zu", &x) != 1 || x == 0)
        {
            printf("invalid count \n");
            return 1;
        }
        size_t aligned_offset = (a.offset + (alignment - 1)) & ~(alignment - 1);
        size_t padding = aligned_offset - a.offset;
        total_padding += padding;
        if (aligned_offset > a.size || x > a.size - aligned_offset)
        {
            printf("not possible\n");
            return 1;
        }

        unsigned char *allocated_start = a.base + padding + a.offset;
        a.offset = a.offset + padding + x;
        unsigned char *allocated_end = allocated_start + x;

        printf("padding added to the memory: %zu \n", padding);
        printf("starting address: %p \n", (void *)allocated_start);
        printf("ending address: %p \n", (void *)allocated_end);
        printf("new arena offset : %zu \n", a.offset);
    }
    printf("total padding in mem arena : %zu \n", total_padding);

    return 0;
}
