#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BLOCK_SIZE 32
#define NUM_BLOCKS 4

typedef struct FreeBlock {
    struct FreeBlock *next;
} FreeBlock;

typedef struct {
    uint8_t memory[BLOCK_SIZE * NUM_BLOCKS];
    FreeBlock *free_list;
} MemoryPool;

void pool_init(MemoryPool *pool) {
    pool->free_list = (FreeBlock*)pool->memory;
    FreeBlock *curr = pool->free_list;

    for (int i = 0; i < NUM_BLOCKS - 1; i++) {
        curr->next = (FreeBlock*)(pool->memory + (i + 1) * BLOCK_SIZE);
        curr = curr->next;
    }
    curr->next = NULL;
}

void* pool_alloc(MemoryPool *pool) {
    if (pool->free_list == NULL) {
        printf("Memory Pool Out of Blocks!\n");
        return NULL;
    }

    FreeBlock *block = pool->free_list;
    pool->free_list = pool->free_list->next;
    return (void*)block;
}

void pool_free(MemoryPool *pool, void *ptr) {
    if (!ptr) return;
    FreeBlock *block = (FreeBlock*)ptr;
    block->next = pool->free_list;
    pool->free_list = block;
}

int main(void) {
    MemoryPool pool;
    pool_init(&pool);

    printf("--- Fixed-Size Block Memory Pool Allocator ---\n");

    int *p1 = (int*)pool_alloc(&pool);
    int *p2 = (int*)pool_alloc(&pool);

    *p1 = 42;
    *p2 = 84;

    printf("Allocated block 1 value: %d\n", *p1);
    printf("Allocated block 2 value: %d\n", *p2);

    pool_free(&pool, p1);
    printf("Released block 1 back to pool.\n");

    int *p3 = (int*)pool_alloc(&pool);
    *p3 = 99;
    printf("Reallocated block 3 value: %d (Reuses recycled block slot)\n", *p3);

    return 0;
}
