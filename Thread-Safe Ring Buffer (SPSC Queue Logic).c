#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define CAPACITY 8 // Must be power of 2 for fast modulo masking

typedef struct {
    int data[CAPACITY];
    volatile size_t head;
    volatile size_t tail;
} SPSCQueue;

void spsc_init(SPSCQueue *q) {
    q->head = 0;
    q->tail = 0;
}

bool spsc_enqueue(SPSCQueue *q, int val) {
    size_t current_tail = q->tail;
    size_t current_head = q->head;

    if ((current_tail - current_head) >= CAPACITY) {
        return false; // Queue full
    }

    q->data[current_tail & (CAPACITY - 1)] = val;
    q->tail = current_tail + 1; // Atomic write for single producer
    return true;
}

bool spsc_dequeue(SPSCQueue *q, int *val) {
    size_t current_head = q->head;
    size_t current_tail = q->tail;

    if (current_head == current_tail) {
        return false; // Queue empty
    }

    *val = q->data[current_head & (CAPACITY - 1)];
    q->head = current_head + 1; // Atomic write for single consumer
    return true;
}

int main(void) {
    SPSCQueue q;
    spsc_init(&q);

    printf("--- SPSC Ring Buffer Simulation ---\n");
    
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
