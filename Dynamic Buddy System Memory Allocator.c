#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_ORDER 5          // Max block size = 2^5 = 32 units (e.g., 32 * 32B = 1024B)
#define UNIT_SIZE 32
#define TOTAL_UNITS (1 << MAX_ORDER)

typedef struct FreeNode {
    struct FreeNode *next;
} FreeNode;

typedef struct {
    uint8_t memory[TOTAL_UNITS * UNIT_SIZE];
    FreeNode *free_lists[MAX_ORDER + 1];
} BuddyAllocator;

void buddy_init(BuddyAllocator *ba) {
    for (int i = 0; i <= MAX_ORDER; i++) {
        ba->free_lists[i] = NULL;
    }
    FreeNode *initial = (FreeNode*)ba->memory;
    initial->next = NULL;
    ba->free_lists[MAX_ORDER] = initial;
}

int get_order(size_t size) {
    int order = 0;
    size_t block_size = UNIT_SIZE;
    while (block_size < size && order < MAX_ORDER) {
        block_size <<= 1;
        order++;
    }
    return (block_size >= size) ? order : -1;
}

void* buddy_alloc(BuddyAllocator *ba, size_t size) {
    int req_order = get_order(size);
    if (req_order == -1) return NULL;

    int curr_order = req_order;
    while (curr_order <= MAX_ORDER && ba->free_lists[curr_order] == NULL) {
        curr_order++;
    }
    if (curr_order > MAX_ORDER) return NULL;

    // Pop block from free list
    FreeNode *block = ba->free_lists[curr_order];
    ba->free_lists[curr_order] = block->next;

    // Split down to requested order
    while (curr_order > req_order) {
        curr_order--;
        size_t half_bytes = (1ULL << curr_order) * UNIT_SIZE;
        FreeNode *buddy = (FreeNode*)((uint8_t*)block + half_bytes);
        buddy->next = ba->free_lists[curr_order];
        ba->free_lists[curr_order] = buddy;
    }

    return (void*)block;
}

void buddy_free(BuddyAllocator *ba, void *ptr, size_t size) {
    if (!ptr) return;
    int order = get_order(size);
    uintptr_t offset = (uint8_t*)ptr - ba->memory;

    while (order < MAX_ORDER) {
        size_t block_bytes = (1ULL << order) * UNIT_SIZE;
        uintptr_t buddy_offset = offset ^ block_bytes;
        uint8_t *buddy_addr = ba->memory + buddy_offset;

        // Search if buddy is currently in free_lists[order]
        FreeNode **curr = &ba->free_lists[order];
        bool found = false;
        while (*curr) {
            if ((uint8_t*)(*curr) == buddy_addr) {
                *curr = (*curr)->next; // Remove buddy from free list
                found = true;
                break;
            }
            curr = &(*curr)->next;
        }

        if (!found) break;

        // Coalesce into larger block
        if (buddy_offset < offset) {
            offset = buddy_offset;
        }
        order++;
    }

    FreeNode *coalesced = (FreeNode*)(ba->memory + offset);
    coalesced->next = ba->free_lists[order];
    ba->free_lists[order] = coalesced;
}

int main(void) {
    BuddyAllocator ba;
    buddy_init(&ba);

    printf("--- Dynamic Buddy System Memory Allocator ---\n");
    void *p1 = buddy_alloc(&ba, 64);  // Order 1 (64 bytes)
    void *p2 = buddy_alloc(&ba, 128); // Order 2 (128 bytes)
    void *p3 = buddy_alloc(&ba, 64);  // Order 1 (64 bytes)

    printf("Allocated p1 (64B)  at offset: %ld\n", (uint8_t*)p1 - ba.memory);
    printf("Allocated p2 (128B) at offset: %ld\n", (uint8_t*)p2 - ba.memory);
    printf("Allocated p3 (64B)  at offset: %ld\n", (uint8_t*)p3 - ba.memory);

    buddy_free(&ba, p1, 64);
    buddy_free(&ba, p3, 64);
    buddy_free(&ba, p2, 128);

    printf("All blocks freed and coalesced back to Max Order (%d): %s\n",
           MAX_ORDER, ba.free_lists[MAX_ORDER] != NULL ? "YES" : "NO");

    return 0;
}
