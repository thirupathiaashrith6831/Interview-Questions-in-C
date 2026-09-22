#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct FibNode {
    int key;
    int degree;
    bool marked;
    struct FibNode *parent;
    struct FibNode *child;
    struct FibNode *left;
    struct FibNode *right;
} FibNode;

typedef struct {
    FibNode *min;
    int num_nodes;
} FibHeap;

FibNode* create_fib_node(int key) {
    FibNode *node = malloc(sizeof(FibNode));
    node->key = key;
    node->degree = 0;
    node->marked = false;
    node->parent = node->child = NULL;
    node->left = node->right = node;
    return node;
}

FibHeap* fib_heap_create(void) {
    FibHeap *heap = malloc(sizeof(FibHeap));
    heap->min = NULL;
    heap->num_nodes = 0;
    return heap;
}

void fib_heap_insert(FibHeap *heap, int key) {
    FibNode *node = create_fib_node(key);

    if (heap->min == NULL) {
        heap->min = node;
    } else {
        // Insert into root circular doubly linked list
        node->left = heap->min;
        node->right = heap->min->right;
        heap->min->right->left = node;
        heap->min->right = node;

        if (key < heap->min->key) {
            heap->min = node;
        }
    }
    heap->num_nodes++;
}

void fib_heap_free_nodes(FibNode *node) {
    if (!node) return;
    FibNode *curr = node;
    do {
        FibNode *next = curr->right;
        if (curr->child) fib_heap_free_nodes(curr->child);
        free(curr);
        curr = next;
    } while (curr != node);
}

void fib_heap_free(FibHeap *heap) {
    if (heap->min) {
        fib_heap_free_nodes(heap->min);
    }
    free(heap);
}

int main(void) {
    FibHeap *heap = fib_heap_create();

    printf("--- Fibonacci Heap Min-Pointer & Insertion Engine ---\n");
    int keys[] = {10, 3, 15, 6, 2, 25};
    for (size_t i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
        fib_heap_insert(heap, keys[i]);
        printf("Inserted: %2d | Current Min Key: %d\n", keys[i], heap->min->key);
    }

    printf("\nTotal Nodes in Fibonacci Heap: %d\n", heap->num_nodes);

    fib_heap_free(heap);
    return 0;
}
