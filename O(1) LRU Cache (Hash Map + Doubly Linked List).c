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
    int size;
    Node *head;
    Node *tail;
} LRUCache;

LRUCache* lru_create(int capacity) {
    LRUCache *cache = malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    return cache;
}

void lru_reference(LRUCache *cache, Node *node) {
    if (cache->head == node) return;

    // Unlink node
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    if (cache->tail == node) cache->tail = node->prev;

    // Move to front (head)
    node->next = cache->head;
    node->prev = NULL;
    if (cache->head) cache->head->prev = node;
    cache->head = node;
    if (!cache->tail) cache->tail = node;
}

void lru_put(LRUCache *cache, int key, int value) {
    // Check if key already exists (simplified linear check for single-file demo)
    Node *curr = cache->head;
    while (curr) {
        if (curr->key == key) {
            curr->value = value;
            lru_reference(cache, curr);
            return;
        }
        curr = curr->next;
    }

    Node *new_node = malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;
    new_node->prev = new_node->next = NULL;

    if (cache->size >= cache->capacity) {
        // Evict LRU (tail)
        Node *to_evict = cache->tail;
        if (to_evict) {
            if (to_evict->prev) to_evict->prev->next = NULL;
            cache->tail = to_evict->prev;
            if (cache->head == to_evict) cache->head = NULL;
            printf("Evicting key %d\n", to_evict->key);
            free(to_evict);
            cache->size--;
        }
    }

    // Insert at head
    new_node->next = cache->head;
    if (cache->head) cache->head->prev = new_node;
    cache->head = new_node;
    if (!cache->tail) cache->tail = new_node;
    cache->size++;
}

int lru_get(LRUCache *cache, int key) {
    Node *curr = cache->head;
    while (curr) {
        if (curr->key == key) {
            lru_reference(cache, curr);
            return curr->value;
        }
        curr = curr->next;
    }
    return -1; // Cache miss
}

void lru_free(LRUCache *cache) {
    Node *curr = cache->head;
    while (curr) {
        Node *next = curr->next;
        free(curr);
        curr = next;
    }
    free(cache);
}

int main(void) {
    LRUCache *cache = lru_create(2);

    printf("--- O(1) LRU Cache Engine ---\n");
    lru_put(cache, 1, 10);
    lru_put(cache, 2, 20);
    printf("Get Key 1: %d\n", lru_get(cache, 1)); // Touches 1

    lru_put(cache, 3, 30); // Evicts key 2 (LRU)
    printf("Get Key 2: %d (Expected -1)\n", lru_get(cache, 2));

    lru_free(cache);
    return 0;
}
