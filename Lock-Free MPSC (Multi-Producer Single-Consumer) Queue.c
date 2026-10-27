#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>

typedef struct MPSCNode {
    int data;
    _Atomic(struct MPSCNode*) next;
} MPSCNode;

typedef struct {
    _Atomic(MPSCNode*) head;
    MPSCNode *tail;
    MPSCNode stub;
} MPSCQueue;

void mpsc_init(MPSCQueue *q) {
    q->stub.next = NULL;
    atomic_init(&q->head, &q->stub);
    q->tail = &q->stub;
}

void mpsc_push(MPSCQueue *q, int val) {
    MPSCNode *node = malloc(sizeof(MPSCNode));
    node->data = val;
    atomic_init(&node->next, NULL);

    MPSCNode *prev = atomic_exchange_explicit(&q->head, node, memory_order_acq_rel);
    atomic_store_explicit(&prev->next, node, memory_order_release);
}

bool mpsc_pop(MPSCQueue *q, int *val) {
    MPSCNode *tail = q->tail;
    MPSCNode *next = atomic_load_explicit(&tail->next, memory_order_acquire);

    if (tail == &q->stub) {
        if (next == NULL) return false;
        q->tail = next;
        tail = next;
        next = atomic_load_explicit(&next->next, memory_order_acquire);
    }

    if (next != NULL) {
        *val = next->data;
        q->tail = next;
        free(tail);
        return true;
    }

    // Check if queue is empty or if producer is lagging
    MPSCNode *head = atomic_load_explicit(&q->head, memory_order_acquire);
    if (tail != head) return false; // In-flight push

    mpsc_push(q, 0); // Push dummy stub to complete state
    next = atomic_load_explicit(&tail->next, memory_order_acquire);
    if (next != NULL) {
        *val = next->data;
        q->tail = next;
        free(tail);
        return true;
    }
    return false;
}

int main(void) {
    MPSCQueue q;
    mpsc_init(&q);

    printf("--- Lock-Free MPSC Queue ---\n");
    mpsc_push(&q, 1000);
    mpsc_push(&q, 2000);
    mpsc_push(&q, 3000);

    int val;
    while (mpsc_pop(&q, &val)) {
        printf("Consumed Message: %d\n", val);
    }

    return 0;
}
