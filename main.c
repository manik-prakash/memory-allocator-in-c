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
    arena a = {
        .base = arena_memory,
        .size = sizeof(arena_memory),
        .offset = 0};
    printf("arena size : %zu \n", a.size);
    printf("arena offset : %zu \n", a.offset);
    
    int n;
    printf("how many times do u have to allocate memmory :");
    scanf("%d",&n);
    
    while(n--){
        size_t x;
        printf("enter the amount of memory u want to be allocated(in Bytes) :");
        if (scanf("%zu", &x) != 1)
        {
            printf("invalid input \n");
            return 1;
        }
        if (x == 0)
        {
            return 1;
        }
    
        if (x > a.size - a.offset)
        {
            printf("not possible\n");
            return 1;
        }
    
        unsigned char *allocated_start = a.base + a.offset;
        a.offset += x;
        unsigned char *allocated_end = allocated_start + x;
    
        printf("starting address: %p \n", (void *)allocated_start);
        printf("ending address: %p \n", (void *)allocated_end);
        printf("new arena offset : %zu \n", a.offset);
    }

    return 0;
}
