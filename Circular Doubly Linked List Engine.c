#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    size_t size;
} CircularList;

CircularList* clist_create(void) {
    CircularList *list = malloc(sizeof(CircularList));
    list->head = NULL;
    list->size = 0;
    return list;
}

void clist_insert_end(CircularList *list, int value) {
    Node *new_node = malloc(sizeof(Node));
    new_node->data = value;

    if (list->head == NULL) {
        new_node->next = new_node;
        new_node->prev = new_node;
        list->head = new_node;
    } else {
        Node *tail = list->head->prev;
        new_node->next = list->head;
        new_node->prev = tail;
        tail->next = new_node;
        list->head->prev = new_node;
    }
    list->size++;
}

void clist_display_forward(const CircularList *list) {
    if (!list->head) return;
    printf("Forward Ring: ");
    Node *curr = list->head;
    for (size_t i = 0; i < list->size; i++) {
        printf("%d <-> ", curr->data);
        curr = curr->next;
    }
    printf("(back to %d)\n", list->head->data);
}

void clist_free(CircularList *list) {
    if (!list->head) {
        free(list);
        return;
    }
    Node *curr = list->head;
    for (size_t i = 0; i < list->size; i++) {
        Node *next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

int main(void) {
    CircularList *ring = clist_create();

    printf("--- Circular Doubly Linked List ---\n");
    clist_insert_end(ring, 10);
    clist_insert_end(ring, 20);
    clist_insert_end(ring, 30);

    clist_display_forward(ring);
    clist_free(ring);

    return 0;
}
