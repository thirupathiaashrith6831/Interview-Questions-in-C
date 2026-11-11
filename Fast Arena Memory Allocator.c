#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ARENA_SIZE 1024

typedef struct {
    uint8_t buffer[ARENA_SIZE];
    size_t offset;
} Arena;

void arena_init(Arena *arena) {
    arena->offset = 0;
}

void* arena_alloc(Arena *arena, size_t size) {
    // Align to 8-byte boundaries
    size_t aligned_size = (size + 7) & ~7;
    if (arena->offset + aligned_size > ARENA_SIZE) {
        printf("Arena Out of Memory!\n");
        return NULL;
    }
    void *ptr = &arena->buffer[arena->offset];
    arena->offset += aligned_size;
    return ptr;
}

void arena_reset(Arena *arena) {
    arena->offset = 0;
}

int main(void) {
    Arena arena;
    arena_init(&arena);

    printf("--- Fast Arena Memory Allocator ---\n");
    int *nums = arena_alloc(&arena, sizeof(int) * 10);
    for (int i = 0; i < 10; i++) {
        nums[i] = i * 10;
    }

    printf("Allocated array first element: %d, last element: %d\n", nums[0], nums[9]);
    printf("Current arena offset usage: %zu bytes\n", arena.offset);

    arena_reset(&arena);
    printf("Arena reset! Offset is now: %zu bytes\n", arena.offset);

    return 0;
}
