#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    int value;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    int capacity;
    int count;
    Node *head;
    Node *tail;
} LRUCache;

Node* create_node(int key, int value) {
    Node *node = malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->prev = node->next = NULL;
    return node;
}

LRUCache* lru_create(int capacity) {
    LRUCache *cache = malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->count = 0;
    cache->head = create_node(0, 0); // Dummy head
    cache->tail = create_node(0, 0); // Dummy tail
    cache->head->next = cache->tail;
    cache->tail->prev = cache->head;
    return cache;
}

void remove_node(Node *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void add_to_head(LRUCache *cache, Node *node) {
    node->next = cache->head->next;
    node->next->prev = node;
    node->prev = cache->head;
    cache->head->next = node;
}

void lru_put(LRUCache *cache, int key, int value) {
    // Basic append/eviction demonstration
    if (cache->count >= cache->capacity) {
        Node *lru = cache->tail->prev;
        remove_node(lru);
        free(lru);
        cache->count--;
    }
    Node *node = create_node(key, value);
    add_to_head(cache, node);
    cache->count++;
}

void lru_print(LRUCache *cache) {
    Node *curr = cache->head->next;
    printf("Cache state (Most to Least Recently Used): ");
    while (curr != cache->tail) {
        printf("[%d: %d] ", curr->key, curr->value);
        curr = curr->next;
    }
    printf("\n");
}

int main(void) {
    LRUCache *cache = lru_create(3);
    lru_put(cache, 1, 100);
    lru_put(cache, 2, 200);
    lru_put(cache, 3, 300);
    lru_print(cache);

    // Adding 4th item evicts key 1 (least recently used)
    lru_put(cache, 4, 400);
    lru_print(cache);

    return 0;
}