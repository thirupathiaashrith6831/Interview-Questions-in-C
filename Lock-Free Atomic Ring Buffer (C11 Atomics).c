#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>

#define RING_SIZE 8

typedef struct {
    int buffer[RING_SIZE];
    _Atomic size_t head;
    _Atomic size_t tail;
} LockFreeRingBuffer;

void ring_init(LockFreeRingBuffer *ring) {
    atomic_init(&ring->head, 0);
    atomic_init(&ring->tail, 0);
}

bool ring_push(LockFreeRingBuffer *ring, int item) {
    size_t tail = atomic_load_explicit(&ring->tail, memory_order_relaxed);
    size_t head = atomic_load_explicit(&ring->head, memory_order_acquire);

    if ((tail + 1) % RING_SIZE == head) {
        return false; // Full
    }

    ring->buffer[tail] = item;
    atomic_store_explicit(&ring->tail, (tail + 1) % RING_SIZE, memory_order_release);
    return true;
}

bool ring_pop(LockFreeRingBuffer *ring, int *item) {
    size_t head = atomic_load_explicit(&ring->head, memory_order_relaxed);
    size_t tail = atomic_load_explicit(&ring->tail, memory_order_acquire);

    if (head == tail) {
        return false; // Empty
    }

    *item = ring->buffer[head];
    atomic_store_explicit(&ring->head, (head + 1) % RING_SIZE, memory_order_release);
    return true;
}

int main(void) {
    LockFreeRingBuffer ring;
    ring_init(&ring);

    printf("--- C11 Atomic Lock-Free Queue ---\n");

    for (int i = 10; i <= 50; i += 10) {
        if (ring_push(&ring, i)) {
            printf("Pushed: %d\n", i);
        }
    }

    int val;
    while (ring_pop(&ring, &val)) {
        printf("Popped: %d\n", val);
    }

    return 0;
}
