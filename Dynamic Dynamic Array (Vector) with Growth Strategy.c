#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 2

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} Vector;

Vector* vector_create(void) {
    Vector *v = malloc(sizeof(Vector));
    v->size = 0;
    v->capacity = INITIAL_CAPACITY;
    v->data = malloc(sizeof(int) * v->capacity);
    return v;
}

void vector_push(Vector *v, int value) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->data = realloc(v->data, sizeof(int) * v->capacity);
        printf("Reallocating capacity: %zu elements\n", v->capacity);
    }
    v->data[v->size++] = value;
}

int vector_get(const Vector *v, size_t index) {
    if (index >= v->size) return -1;
    return v->data[index];
}

void vector_free(Vector *v) {
    free(v->data);
    free(v);
}

int main(void) {
    Vector *v = vector_create();

    printf("--- Dynamic Vector Resizing Engine ---\n");
    for (int i = 1; i <= 7; i++) {
        vector_push(v, i * 10);
        printf("Pushed: %2d | Size: %zu | Capacity: %zu\n", i * 10, v->size, v->capacity);
    }

    printf("\nVector Contents: [ ");
    for (size_t i = 0; i < v->size; i++) {
        printf("%d ", vector_get(v, i));
    }
    printf("]\n");

    vector_free(v);
    return 0;
}
