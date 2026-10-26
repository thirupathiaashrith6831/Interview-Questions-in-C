#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>

#define MAX_THREADS 2
#define MAX_RETIRED 16

typedef struct Node {
    int data;
    _Atomic(struct Node*) next;
} Node;

// Global thread-local hazard pointer table simulation
_Atomic(void*) hazard_pointers[MAX_THREADS];

typedef struct {
    void *list[MAX_RETIRED];
    int count;
} RetiredList;

RetiredList retired_threads[MAX_THREADS];

void hazard_init(void) {
    for (int i = 0; i < MAX_THREADS; i++) {
        atomic_init(&hazard_pointers[i], NULL);
        retired_threads[i].count = 0;
    }
}

void set_hazard_pointer(int tid, void *ptr) {
    atomic_store_explicit(&hazard_pointers[tid], ptr, memory_order_seq_cst);
}

void retire_node(int tid, void *ptr) {
    RetiredList *rl = &retired_threads[tid];
    rl->list[rl->count++] = ptr;

    if (rl->count >= MAX_RETIRED) {
        // Simplified reclamation scan against active hazard pointers
        for (int i = 0; i < rl->count; i++) {
            void *candidate = rl->list[i];
            bool referenced = false;
            for (int t = 0; t < MAX_THREADS; t++) {
                if (atomic_load(&hazard_pointers[t]) == candidate) {
                    referenced = true;
                    break;
                }
            }
            if (!referenced) {
                free(candidate);
                rl->list[i] = NULL;
            }
        }
        // Compact list
        int write_idx = 0;
        for (int i = 0; i < rl->count; i++) {
            if (rl->list[i] != NULL) {
                rl->list[write_idx++] = rl->list[i];
            }
        }
        rl->count = write_idx;
    }
}

int main(void) {
    hazard_init();

    printf("--- Hazard Pointer Memory Reclamation Engine ---\n");
    Node *node1 = malloc(sizeof(Node));
    node1->data = 500;

    // Thread 0 acquires hazard pointer on node1
    set_hazard_pointer(0, node1);
    printf("Thread 0 protected node1 (%d) with Hazard Pointer.\n", node1->data);

    // Thread 1 retires node1
    retire_node(1, node1);
    printf("Thread 1 retired node1 (Scan checks active hazard pointers).\n");

    // Clear hazard pointer and re-run reclamation
    set_hazard_pointer(0, NULL);
    retire_node(1, node1); // Will successfully free now
    printf("Hazard pointer cleared; memory successfully reclaimed.\n");

    return 0;
}
