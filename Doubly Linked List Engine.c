#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
} DoublyLinkedList;

DoublyLinkedList* dll_create(void) {
    DoublyLinkedList *list = malloc(sizeof(DoublyLinkedList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void dll_push_back(DoublyLinkedList *list, int value) {
    Node *new_node = malloc(sizeof(Node));
    new_node->data = value;
    new_node->next = NULL;
    new_node->prev = list->tail;

    if (list->tail) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }
    list->tail = new_node;
}

void dll_print_forward(const DoublyLinkedList *list) {
    printf("Forward : ");
    Node *curr = list->head;
    while (curr) {
        printf("%d <-> ", curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
}

void dll_print_backward(const DoublyLinkedList *list) {
    printf("Backward: ");
    Node *curr = list->tail;
    while (curr) {
        printf("%d <-> ", curr->data);
        curr = curr->prev;
    }
    printf("NULL\n");
}

void dll_free(DoublyLinkedList *list) {
    Node *curr = list->head;
    while (curr) {
        Node *next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

int main(void) {
    DoublyLinkedList *list = dll_create();

    printf("--- Doubly Linked List Navigation ---\n");
    dll_push_back(list, 100);
    dll_push_back(list, 200);
    dll_push_back(list, 300);

    dll_print_forward(list);
    dll_print_backward(list);

    dll_free(list);
    return 0;
}
