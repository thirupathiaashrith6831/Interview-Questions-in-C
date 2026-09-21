#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} MaxHeap;

MaxHeap* heap_create(size_t capacity) {
    MaxHeap *h = malloc(sizeof(MaxHeap));
    h->data = malloc(sizeof(int) * capacity);
    h->size = 0;
    h->capacity = capacity;
    return h;
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void heapify_up(MaxHeap *h, size_t idx) {
    while (idx > 0) {
        size_t parent = (idx - 1) / 2;
        if (h->data[idx] > h->data[parent]) {
            swap(&h->data[idx], &h->data[parent]);
            idx = parent;
        } else {
            break;
        }
    }
}

void heapify_down(MaxHeap *h, size_t idx) {
    size_t largest = idx;
    size_t left = 2 * idx + 1;
    size_t right = 2 * idx + 2;

    if (left < h->size && h->data[left] > h->data[largest])
        largest = left;
    if (right < h->size && h->data[right] > h->data[largest])
        largest = right;

    if (largest != idx) {
        swap(&h->data[idx], &h->data[largest]);
        heapify_down(h, largest);
    }
}

void heap_push(MaxHeap *h, int val) {
    if (h->size == h->capacity) return;
    h->data[h->size] = val;
    heapify_up(h, h->size);
    h->size++;
}

int heap_pop(MaxHeap *h) {
    if (h->size == 0) return -1;
    int top = h->data[0];
    h->data[0] = h->data[h->size - 1];
    h->size--;
    heapify_down(h, 0);
    return top;
}

void heap_free(MaxHeap *h) {
    free(h->data);
    free(h);
}

int main(void) {
    MaxHeap *h = heap_create(10);

    printf("--- Max-Heap Priority Queue Engine ---\n");
    int values[] = {3, 10, 12, 8, 2, 14};
    for (size_t i = 0; i < sizeof(values)/sizeof(values[0]); i++) {
        heap_push(h, values[i]);
        printf("Pushed: %2d | Current Max: %2d\n", values[i], h->data[0]);
    }

    printf("\nPopping elements in priority order:\n");
    while (h->size > 0) {
        printf("Popped: %d\n", heap_pop(h));
    }

    heap_free(h);
    return 0;
}
