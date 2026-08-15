#include <stdio.h>
#include <stdlib.h>

#define MAX_CAPACITY 100

typedef struct {
    int data[MAX_CAPACITY];
    int size;
} MaxHeap;

void heap_init(MaxHeap *heap) {
    heap->size = 0;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heap_insert(MaxHeap *heap, int value) {
    if (heap->size >= MAX_CAPACITY) return;

    int i = heap->size;
    heap->data[i] = value;
    heap->size++;

    // Sift up to restore max-heap property
    while (i != 0 && heap->data[(i - 1) / 2] < heap->data[i]) {
        swap(&heap->data[i], &heap->data[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int heap_extract_max(MaxHeap *heap) {
    if (heap->size <= 0) return -1;
    if (heap->size == 1) {
        heap->size--;
        return heap->data[0];
    }

    int root = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;

    // Sift down
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < heap->size && heap->data[left] > heap->data[largest])
            largest = left;
        if (right < heap->size && heap->data[right] > heap->data[largest])
            largest = right;

        if (largest != i) {
            swap(&heap->data[i], &heap->data[largest]);
            i = largest;
        } else {
            break;
        }
    }

    return root;
}

int main(void) {
    MaxHeap heap;
    heap_init(&heap);

    printf("--- Priority Queue (Max-Heap) ---\n");
    int elements[] = {15, 40, 8, 50, 30};
    for (int i = 0; i < 5; i++) {
        heap_insert(&heap, elements[i]);
        printf("Inserted: %d\n", elements[i]);
    }

    printf("\nExtracting in priority order (Highest to Lowest):\n");
    while (heap.size > 0) {
        printf("Extracted Max: %d\n", heap_extract_max(&heap));
    }

    return 0;
}
