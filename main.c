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

int main() {
    arena a = {
        .base = arena_memory,
        .size = sizeof(arena_memory),
        .offset = 0
    };

    printf("arena size : %zu \n",   a.size);
    printf("arena offset : %zu \n",a.offset);

    return 0;
}
