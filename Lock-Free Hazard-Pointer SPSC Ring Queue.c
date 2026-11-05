#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>

#define RING_CAPACITY 16

typedef struct {
    int data[RING_CAPACITY];
    _Atomic size_t head;
    _Atomic size_t tail;
    // Cached indices to avoid unnecessary atomic loads across L1/L2 cache lines
    size_t cached_head;
    size_t cached_tail;
} CachedRingQueue;

void spsc_init(CachedRingQueue *q) {
    atomic_init(&q->head, 0);
    atomic_init(&q->tail, 0);
    q->cached_head = 0;
    q->cached_tail = 0;
}

bool spsc_enqueue(CachedRingQueue *q, int value) {
    size_t tail = atomic_load_explicit(&q->tail, memory_order_relaxed);

    if (tail - q->cached_head >= RING_CAPACITY) {
        q->cached_head = atomic_load_explicit(&q->head, memory_order_acquire);
        if (tail - q->cached_head >= RING_CAPACITY) {
            return false; // Queue is full
        }
    }

    q->data[tail % RING_CAPACITY] = value;
    atomic_store_explicit(&q->tail, tail + 1, memory_order_release);
    return true;
}

bool spsc_dequeue(CachedRingQueue *q, int *value) {
    size_t head = atomic_load_explicit(&q->head, memory_order_relaxed);

    if (head == q->cached_tail) {
        q->cached_tail = atomic_load_explicit(&q->tail, memory_order_acquire);
        if (head == q->cached_tail) {
            return false; // Queue is empty
        }
    }

    *value = q->data[head % RING_CAPACITY];
    atomic_store_explicit(&q->head, head + 1, memory_order_release);
    return true;
}

int main(void) {
    CachedRingQueue q;
    spsc_init(&q);

    printf("--- Zero-Bouncing Lock-Free SPSC Queue ---\n");

    for (int i = 1; i <= 5; i++) {
        if (spsc_enqueue(&q, i * 100)) {
            printf("Enqueued: %d\n", i * 100);
        }
    }

    int val;
    while (spsc_dequeue(&q, &val)) {
        printf("Dequeued: %d\n", val);
    }

    return 0;
}
