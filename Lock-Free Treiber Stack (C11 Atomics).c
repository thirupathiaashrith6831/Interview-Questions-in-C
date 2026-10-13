#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    _Atomic(Node*) head;
} TreiberStack;

void stack_init(TreiberStack *s) {
    atomic_init(&s->head, NULL);
}

void stack_push(TreiberStack *s, int val) {
    Node *new_node = malloc(sizeof(Node));
    new_node->data = val;
    
    Node *old_head = atomic_load_explicit(&s->head, memory_order_relaxed);
    do {
        new_node->next = old_head;
    } while (!atomic_compare_exchange_weak_explicit(
        &s->head, &old_head, new_node,
        memory_order_release, memory_order_relaxed));
}

bool stack_pop(TreiberStack *s, int *val) {
    Node *old_head = atomic_load_explicit(&s->head, memory_order_acquire);
    do {
        if (old_head == NULL) return false;
    } while (!atomic_compare_exchange_weak_explicit(
        &s->head, &old_head, old_head->next,
        memory_order_acquire, memory_order_relaxed));

    *val = old_head->data;
    free(old_head);
    return true;
}

int main(void) {
    TreiberStack stack;
    stack_init(&stack);

    printf("--- Lock-Free Treiber Stack (C11 Atomics) ---\n");
    stack_push(&stack, 10);
    stack_push(&stack, 20);
    stack_push(&stack, 30);

    int val;
    while (stack_pop(&stack, &val)) {
        printf("Popped Node Data: %d\n", val);
    }

    return 0;
}
