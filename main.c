#include <stdio.h>
#include <stddef.h>
#include <stdalign.h>
#include <stdint.h>

typedef struct
{
    unsigned char *base;
    size_t size;
    size_t offset;
} arena;


const size_t alignment = alignof(max_align_t);

int arena_init(size_t arena_size, arena *a,unsigned char * arena_memory){
    if(arena_size == 0){
        return 0;
    }
    if(arena_memory == NULL){
        return 0;
    }
    a->base = arena_memory;
    a->offset = 0;
    a->size = arena_size;

    return 1;
}

void *mem_alloc(arena *a, size_t x, size_t *total_padding)
{
    if (x == 0)
    {
        printf("invalid memory request \n");
        return NULL;
    }
    size_t aligned_offset = (a->offset + (alignment - 1)) & ~(alignment - 1);
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

int main()
{
    size_t arena_size;
    printf("define the arena size : ");
    if(scanf("%zu",&arena_size) != 1){  
        printf("invalid input\n");
        return 1;
    }
    alignas(max_align_t) unsigned char arena_memory[arena_size];

    arena a;
    int res = arena_init(arena_size,&a,arena_memory);
    if(res == 0){
        return 1;
    }

    printf("arena size : %zu \n", a->size);
    printf("arena offset : %zu \n", a->offset);

    size_t total_padding = 0;

    int *num = mem_alloc(&a, 4 * sizeof(int), &total_padding);
    char *buffer = mem_alloc(&a, 100, &total_padding);

    printf("num : %p \n", num);
    printf("buffer : %p \n", buffer);
    printf("total padding : %zu \n",total_padding);

    return 0;
}
