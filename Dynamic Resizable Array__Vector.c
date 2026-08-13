#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} Vector;

Vector* vector_create(size_t initial_capacity) {
    Vector *v = malloc(sizeof(Vector));
    v->size = 0;
    v->capacity = initial_capacity;
    v->data = malloc(sizeof(int) * v->capacity);
    return v;
}

void vector_push(Vector *v, int value) {
    // Automatically double capacity when full
    if (v->size == v->capacity) {
        v->capacity *= 2;
        v->data = realloc(v->data, sizeof(int) * v->capacity);
    }
    v->data[v->size++] = value;
}

int vector_get(const Vector *v, size_t index) {
    if (index >= v->size) {
        fprintf(stderr, "Index out of bounds!\n");
        exit(EXIT_FAILURE);
    }
    return v->data[index];
}

void vector_free(Vector *v) {
    if (v) {
        free(v->data);
        free(v);
    }
}

int main(void) {
    Vector *v = vector_create(2);

    printf("--- Dynamic Vector (Resizable Array) ---\n");
    for (int i = 1; i <= 5; i++) {
        vector_push(v, i * 10);
        printf("Pushed %d | Size: %zu | Capacity: %zu\n", i * 10, v->size, v->capacity);
    }

    printf("\nVector elements: ");
    for (size_t i = 0; i < v->size; i++) {
        printf("%d ", vector_get(v, i));
    }
    printf("\n");

    vector_free(v);
    return 0;
}
