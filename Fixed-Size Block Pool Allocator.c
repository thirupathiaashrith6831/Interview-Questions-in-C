#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define BLOCK_SIZE 32
#define BLOCK_COUNT 4

typedef struct {
    uint8_t memory[BLOCK_SIZE * BLOCK_COUNT];
    bool used[BLOCK_COUNT];
} PoolAllocator;

void pool_init(PoolAllocator *pool) {
    for (int i = 0; i < BLOCK_COUNT; i++) {
        pool->used[i] = false;
    }
}

void* pool_alloc(PoolAllocator *pool) {
    for (int i = 0; i < BLOCK_COUNT; i++) {
        if (!pool->used[i]) {
            pool->used[i] = true;
            printf("Allocated block index %d\n", i);
            return &pool->memory[i * BLOCK_SIZE];
        }
    }
    return NULL; // Pool exhausted
}

void pool_free(PoolAllocator *pool, void *ptr) {
    if (!ptr) return;
    uint8_t *byte_ptr = (uint8_t*)ptr;
    ptrdiff_t offset = byte_ptr - pool->memory;
    int index = offset / BLOCK_SIZE;

    if (index >= 0 && index < BLOCK_COUNT) {
        pool->used[index] = false;
        printf("Freed block index %d\n", index);
    }
}

int main(void) {
    PoolAllocator pool;
    pool_init(&pool);

    printf("--- Fixed-Size Block Pool Allocator ---\n");
    void *p1 = pool_alloc(&pool);
    void *p2 = pool_alloc(&pool);

    pool_free(&pool, p1);
    void *p3 = pool_alloc(&pool); // Reuses index 0

    return 0;
}
