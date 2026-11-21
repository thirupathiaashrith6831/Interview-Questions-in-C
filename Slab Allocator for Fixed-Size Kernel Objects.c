#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define SLAB_CAPACITY 8

typedef struct {
    int id;
    uint8_t payload[56]; // Total 64-byte cache-line-friendly object
} TaskControlBlock;

typedef struct Slab {
    TaskControlBlock objects[SLAB_CAPACITY];
    uint8_t free_bitmap; // 1 = free, 0 = allocated (for up to 8 slots)
    struct Slab *next;
} Slab;

typedef struct {
    Slab *slabs;
    void (*ctor)(TaskControlBlock *obj);
} SlabCache;

void tcb_init_ctor(TaskControlBlock *obj) {
    obj->id = -1;
    memset(obj->payload, 0, sizeof(obj->payload));
}

Slab* slab_create(SlabCache *cache) {
    Slab *slab = malloc(sizeof(Slab));
    if (!slab) return NULL;
    slab->free_bitmap = 0xFF; // All 8 slots free
    slab->next = cache->slabs;
    if (cache->ctor) {
        for (int i = 0; i < SLAB_CAPACITY; i++) {
            cache->ctor(&slab->objects[i]);
        }
    }
    cache->slabs = slab;
    return slab;
}

SlabCache* slab_cache_create(void (*ctor)(TaskControlBlock*)) {
    SlabCache *cache = malloc(sizeof(SlabCache));
    cache->slabs = NULL;
    cache->ctor = ctor;
    slab_create(cache);
    return cache;
}

TaskControlBlock* slab_alloc(SlabCache *cache) {
    Slab *curr = cache->slabs;
    while (curr) {
        if (curr->free_bitmap != 0) {
            for (int i = 0; i < SLAB_CAPACITY; i++) {
                if (curr->free_bitmap & (1u << i)) {
                    curr->free_bitmap &= ~(1u << i);
                    return &curr->objects[i];
                }
            }
        }
        curr = curr->next;
    }

    Slab *new_slab = slab_create(cache);
    if (!new_slab) return NULL;
    new_slab->free_bitmap &= ~(1u << 0);
    return &new_slab->objects[0];
}

bool slab_free(SlabCache *cache, TaskControlBlock *obj) {
    Slab *curr = cache->slabs;
    while (curr) {
        uintptr_t start = (uintptr_t)&curr->objects[0];
        uintptr_t end = (uintptr_t)&curr->objects[SLAB_CAPACITY];
        uintptr_t addr = (uintptr_t)obj;

        if (addr >= start && addr < end && ((addr - start) % sizeof(TaskControlBlock) == 0)) {
            int idx = (int)((addr - start) / sizeof(TaskControlBlock));
            curr->free_bitmap |= (1u << idx);
            if (cache->ctor) {
                cache->ctor(obj);
            }
            return true;
        }
        curr = curr->next;
    }
    return false;
}

void slab_cache_destroy(SlabCache *cache) {
    Slab *curr = cache->slabs;
    while (curr) {
        Slab *next = curr->next;
        free(curr);
        curr = next;
    }
    free(cache);
}

int main(void) {
    SlabCache *tcb_cache = slab_cache_create(tcb_init_ctor);

    printf("--- Slab Allocator for Fixed-Size Objects ---\n");
    TaskControlBlock *t1 = slab_alloc(tcb_cache);
    TaskControlBlock *t2 = slab_alloc(tcb_cache);
    t1->id = 101;
    t2->id = 202;

    printf("Allocated TCB #1 (id=%d) at %p\n", t1->id, (void*)t1);
    printf("Allocated TCB #2 (id=%d) at %p\n", t2->id, (void*)t2);

    slab_free(tcb_cache, t1);
    TaskControlBlock *t3 = slab_alloc(tcb_cache);
    printf("Reallocated slot after freeing TCB #1 -> default ctor id=%d at %p\n", t3->id, (void*)t3);

    slab_cache_destroy(tcb_cache);
    return 0;
}
