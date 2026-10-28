#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>

#define MAX_THREADS 4

_Atomic uint64_t global_epoch = 0;
_Atomic uint64_t thread_epochs[MAX_THREADS];
_Atomic bool thread_active[MAX_THREADS];

typedef struct RetiredNode {
    void *ptr;
    uint64_t epoch;
    struct RetiredNode *next;
} RetiredNode;

typedef struct {
    RetiredNode *head;
} EpochCollector;

EpochCollector collector_bins[MAX_THREADS];

void ebgc_init(void) {
    atomic_init(&global_epoch, 0);
    for (int i = 0; i < MAX_THREADS; i++) {
        atomic_init(&thread_epochs[i], 0);
        atomic_init(&thread_active[i], false);
        collector_bins[i].head = NULL;
    }
}

void ebgc_enter(int tid) {
    atomic_store_explicit(&thread_epochs[tid], atomic_load_explicit(&global_epoch, memory_order_relaxed), memory_order_relaxed);
    atomic_store_explicit(&thread_active[tid], true, memory_order_release);
}

void ebgc_exit(int tid) {
    atomic_store_explicit(&thread_active[tid], false, memory_order_release);
}

void ebgc_retire(int tid, void *ptr) {
    uint64_t cur_epoch = atomic_load_explicit(&global_epoch, memory_order_relaxed);
    RetiredNode *node = malloc(sizeof(RetiredNode));
    node->ptr = ptr;
    node->epoch = cur_epoch;
    node->next = collector_bins[tid].head;
    collector_bins[tid].head = node;
}

void ebgc_collect(int tid) {
    uint64_t min_epoch = atomic_load_explicit(&global_epoch, memory_order_relaxed);
    for (int i = 0; i < MAX_THREADS; i++) {
        if (atomic_load_explicit(&thread_active[i], memory_order_acquire)) {
            uint64_t te = atomic_load_explicit(&thread_epochs[i], memory_order_relaxed);
            if (te < min_epoch) min_epoch = te;
        }
    }

    RetiredNode *curr = collector_bins[tid].head;
    RetiredNode *prev = NULL;
    while (curr) {
        if (curr->epoch < min_epoch) {
            RetiredNode *to_free = curr;
            if (prev) prev->next = curr->next;
            else collector_bins[tid].head = curr->next;
            curr = curr->next;
            free(to_free->ptr);
            free(to_free);
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

int main(void) {
    ebgc_init();

    printf("--- Epoch-Based Garbage Collection Engine ---\n");
    ebgc_enter(0);
    int *data = malloc(sizeof(int));
    *data = 999;
    
    ebgc_retire(0, data);
    printf("Data pointer retired in epoch %lu.\n", atomic_load(&global_epoch));

    // Advance global epoch simulation
    atomic_fetch_add(&global_epoch, 1);
    ebgc_exit(0);

    ebgc_collect(0);
    printf("EBGC cleanup pass completed; retired nodes collected.\n");

    return 0;
}
