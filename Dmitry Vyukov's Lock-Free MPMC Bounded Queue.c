#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>

#define BUFFER_SIZE 8

typedef struct {
    _Atomic size_t sequence;
    int data;
} Cell;

typedef struct {
    Cell buffer[BUFFER_SIZE];
    _Atomic size_t enqueue_pos;
    _Atomic size_t dequeue_pos;
} MPMCQueue;

void mpmc_init(MPMCQueue *q) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        atomic_init(&q->buffer[i].sequence, i);
        q->buffer[i].data = 0;
    }
    atomic_init(&q->enqueue_pos, 0);
    atomic_init(&q->dequeue_pos, 0);
}

bool mpmc_enqueue(MPMCQueue *q, int val) {
    Cell *cell;
    size_t pos = atomic_load_explicit(&q->enqueue_pos, memory_order_relaxed);
    while (true) {
        cell = &q->buffer[pos % BUFFER_SIZE];
        size_t seq = atomic_load_explicit(&cell->sequence, memory_order_acquire);
        intptr_t dif = (intptr_t)seq - (intptr_t)pos;
        if (dif == 0) {
            if (atomic_compare_exchange_weak_explicit(&q->enqueue_pos, &pos, pos + 1,
                memory_order_relaxed, memory_order_relaxed)) {
                break;
            }
        } else if (dif < 0) {
            return false; // Queue full
        } else {
            pos = atomic_load_explicit(&q->enqueue_pos, memory_order_relaxed);
        }
    }
    cell->data = val;
    atomic_store_explicit(&cell->sequence, pos + 1, memory_order_release);
    return true;
}

bool mpmc_dequeue(MPMCQueue *q, int *val) {
    Cell *cell;
    size_t pos = atomic_load_explicit(&q->dequeue_pos, memory_order_relaxed);
    while (true) {
        cell = &q->buffer[pos % BUFFER_SIZE];
        size_t seq = atomic_load_explicit(&cell->sequence, memory_order_acquire);
        intptr_t dif = (intptr_t)seq - (intptr_t)(pos + 1);
        if (dif == 0) {
            if (atomic_compare_exchange_weak_explicit(&q->dequeue_pos, &pos, pos + 1,
                memory_order_relaxed, memory_order_relaxed)) {
                break;
            }
        } else if (dif < 0) {
            return false; // Queue empty
        } else {
            pos = atomic_load_explicit(&q->dequeue_pos, memory_order_relaxed);
        }
    }
    *val = cell->data;
    atomic_store_explicit(&cell->sequence, pos + BUFFER_SIZE, memory_order_release);
    return true;
}

int main(void) {
    MPMCQueue q;
    mpmc_init(&q);

    printf("--- Dmitry Vyukov Lock-Free MPMC Bounded Queue ---\n");
    mpmc_enqueue(&q, 100);
    mpmc_enqueue(&q, 200);
    mpmc_enqueue(&q, 300);

    int val;
    while (mpmc_dequeue(&q, &val)) {
        printf("Dequeued: %d\n", val);
    }
    return 0;
}
