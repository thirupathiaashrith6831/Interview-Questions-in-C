#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>

#define CAPACITY 64

typedef struct {
    _Atomic int top;
    _Atomic int bottom;
    int buffer[CAPACITY];
} WorkStealingDeque;

void deque_init(WorkStealingDeque *d) {
    atomic_init(&d->top, 0);
    atomic_init(&d->bottom, 0);
}

void deque_push(WorkStealingDeque *d, int val) {
    int b = atomic_load_explicit(&d->bottom, memory_order_relaxed);
    int t = atomic_load_explicit(&d->top, memory_order_acquire);
    
    if (b - t >= CAPACITY - 1) {
        printf("Deque is full!\n");
        return;
    }
    
    d->buffer[b % CAPACITY] = val;
    atomic_store_explicit(&d->bottom, b + 1, memory_order_release);
}

int deque_pop(WorkStealingDeque *d) {
    int b = atomic_load_explicit(&d->bottom, memory_order_relaxed) - 1;
    atomic_store_explicit(&d->bottom, b, memory_order_relaxed);
    
    int t = atomic_load_explicit(&d->top, memory_order_acquire);
    if (t > b) {
        atomic_store_explicit(&d->bottom, t, memory_order_relaxed);
        return -1; // Empty
    }
    
    int val = d->buffer[b % CAPACITY];
    if (t == b) {
        // Last element race condition check
        int expected = t;
        if (!atomic_compare_exchange_strong_explicit(&d->top, &expected, t + 1,
            memory_order_seq_cst, memory_order_relaxed)) {
            atomic_store_explicit(&d->bottom, t + 1, memory_order_relaxed);
            return -1;
        }
        atomic_store_explicit(&d->bottom, t + 1, memory_order_relaxed);
    }
    return val;
}

int main(void) {
    WorkStealingDeque deque;
    deque_init(&deque);

    printf("--- Chase-Lev Work-Stealing Deque ---\n");
    deque_push(&deque, 10);
    deque_push(&deque, 20);
    deque_push(&deque, 30);

    printf("Popped by Owner: %d\n", deque_pop(&deque));
    printf("Popped by Owner: %d\n", deque_pop(&deque));
    printf("Popped by Owner: %d\n", deque_pop(&deque));
    printf("Popped from Empty: %d (Expected -1)\n", deque_pop(&deque));

    return 0;
}
