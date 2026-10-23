#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>

typedef struct QNode {
    int data;
    _Atomic(struct QNode*) next;
} QNode;

typedef struct {
    _Atomic(QNode*) head;
    _Atomic(QNode*) tail;
} LockFreeQueue;

void lfq_init(LockFreeQueue *q) {
    QNode *sentinel = malloc(sizeof(QNode));
    sentinel->data = 0;
    atomic_init(&sentinel->next, NULL);
    atomic_init(&q->head, sentinel);
    atomic_init(&q->tail, sentinel);
}

void lfq_enqueue(LockFreeQueue *q, int val) {
    QNode *node = malloc(sizeof(QNode));
    node->data = val;
    atomic_init(&node->next, NULL);

    while (true) {
        QNode *tail = atomic_load_explicit(&q->tail, memory_order_acquire);
        QNode *next = atomic_load_explicit(&tail->next, memory_order_acquire);
        if (tail == atomic_load_explicit(&q->tail, memory_order_acquire)) {
            if (next == NULL) {
                if (atomic_compare_exchange_weak_explicit(&tail->next, &next, node,
                    memory_order_release, memory_order_relaxed)) {
                    atomic_compare_exchange_strong_explicit(&q->tail, &tail, node,
                        memory_order_release, memory_order_relaxed);
                    return;
                }
            } else {
                atomic_compare_exchange_strong_explicit(&q->tail, &tail, next,
                    memory_order_release, memory_order_relaxed);
            }
        }
    }
}

bool lfq_dequeue(LockFreeQueue *q, int *val) {
    while (true) {
        QNode *head = atomic_load_explicit(&q->head, memory_order_acquire);
        QNode *tail = atomic_load_explicit(&q->tail, memory_order_acquire);
        QNode *next = atomic_load_explicit(&head->next, memory_order_acquire);
        if (head == atomic_load_explicit(&q->head, memory_order_acquire)) {
            if (head == tail) {
                if (next == NULL) return false;
                atomic_compare_exchange_strong_explicit(&q->tail, &tail, next,
                    memory_order_release, memory_order_relaxed);
            } else {
                *val = next->data;
                if (atomic_compare_exchange_strong_explicit(&q->head, &head, next,
                    memory_order_release, memory_order_relaxed)) {
                    free(head);
                    return true;
                }
            }
        }
    }
}

int main(void) {
    LockFreeQueue q;
    lfq_init(&q);

    printf("--- Michael-Scott Lock-Free Queue ---\n");
    lfq_enqueue(&q, 100);
    lfq_enqueue(&q, 200);
    lfq_enqueue(&q, 300);

    int val;
    while (lfq_dequeue(&q, &val)) {
        printf("Dequeued: %d\n", val);
    }
    return 0;
}
