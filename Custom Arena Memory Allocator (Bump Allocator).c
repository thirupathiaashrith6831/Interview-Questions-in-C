#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define ARENA_CAPACITY (1024 * 1024) // 1 MB Pool

typedef struct {
    uint8_t *buffer;
    size_t capacity;
    size_t offset;
} Arena;

Arena arena_create(size_t capacity) {
    Arena arena;
    arena.buffer = malloc(capacity);
    arena.capacity = capacity;
    arena.offset = 0;
    return arena;
}

// Align allocation offset to required byte boundary (default 8 bytes)
uintptr_t align_forward(uintptr_t ptr, size_t align) {
    size_t remainder = ptr % align;
    if (remainder != 0) {
        ptr += (align - remainder);
    }
    return ptr;
}

void* arena_alloc(Arena *arena, size_t size) {
    uintptr_t current_ptr = (uintptr_t)arena->buffer + arena->offset;
    uintptr_t aligned_ptr = align_forward(current_ptr, sizeof(void*));
    size_t next_offset = (aligned_ptr - (uintptr_t)arena->buffer) + size;

    if (next_offset > arena->capacity) {
        printf("Arena Out of Memory!\n");
        return NULL;
    }

    arena->offset = next_offset;
    return (void*)aligned_ptr;
}

void arena_reset(Arena *arena) {
    arena->offset = 0; // O(1) bulk deallocation of all objects
}

void arena_free(Arena *arena) {
    free(arena->buffer);
    arena->buffer = NULL;
    arena->capacity = 0;
    arena->offset = 0;
}

int main(void) {
    Arena arena = arena_create(ARENA_CAPACITY);

    printf("--- Custom Arena Allocator ---\n");

    int *numbers = (int*)arena_alloc(&arena, sizeof(int) * 5);
    for (int i = 0; i < 5; i++) {
        numbers[i] = (i + 1) * 10;
    }

    char *msg = (char*)arena_alloc(&arena, 32);
    snprintf(msg, 32, "Arena Memory in C");

    printf("Allocated Numbers: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\nAllocated String : %s\n", msg);
    printf("Current Arena Offset: %zu / %d bytes\n", arena.offset, ARENA_CAPACITY);

    arena_reset(&arena);
    printf("Reset Arena Offset  : %zu bytes (All memory released)\n", arena.offset);

    arena_free(&arena);
    return 0;
}
