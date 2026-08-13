#include <stdio.h>
#include <stdbool.h>

#define QUEUE_CAPACITY 5

typedef struct {
    int data[QUEUE_CAPACITY];
    int front;
    int rear;
    int count;
} CircularQueue;

void queue_init(CircularQueue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

bool queue_enqueue(CircularQueue *q, int value) {
    if (q->count == QUEUE_CAPACITY) return false; // Buffer full
    q->rear = (q->rear + 1) % QUEUE_CAPACITY;
    q->data[q->rear] = value;
    q->count++;
    return true;
}

bool queue_dequeue(CircularQueue *q, int *out_value) {
    if (q->count == 0) return false; // Buffer empty
    *out_value = q->data[q->front];
    q->front = (q->front + 1) % QUEUE_CAPACITY;
    q->count--;
    return true;
}

void queue_print(const CircularQueue *q) {
    printf("Queue elements: ");
    for (int i = 0; i < q->count; i++) {
        int idx = (q->front + i) % QUEUE_CAPACITY;
        printf("%d ", q->data[idx]);
    }
    printf("(Count: %d/%d)\n", q->count, QUEUE_CAPACITY);
}

int main(void) {
    CircularQueue q;
    queue_init(&q);

    printf("--- Circular Ring Buffer Queue ---\n");
    queue_enqueue(&q, 10);
    queue_enqueue(&q, 20);
    queue_enqueue(&q, 30);
    queue_print(&q);

    int val;
    queue_dequeue(&q, &val);
    printf("Dequeued value: %d\n", val);
    queue_print(&q);

    queue_enqueue(&q, 40);
    queue_enqueue(&q, 50);
    queue_enqueue(&q, 60);
    queue_print(&q);

    return 0;
}
