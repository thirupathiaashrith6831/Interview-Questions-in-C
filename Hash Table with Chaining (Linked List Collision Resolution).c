#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 5

typedef struct HashNode {
    int key;
    int value;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode *buckets[TABLE_SIZE];
} ChainedHashTable;

void cht_init(ChainedHashTable *ht) {
    for (int i = 0; i < TABLE_SIZE; i++) ht->buckets[i] = NULL;
}

int cht_hash(int key) {
    return key % TABLE_SIZE;
}

void cht_insert(ChainedHashTable *ht, int key, int value) {
    int idx = cht_hash(key);
    HashNode *curr = ht->buckets[idx];
    while (curr) {
        if (curr->key == key) {
            curr->value = value;
            return;
        }
        curr = curr->next;
    }
    HashNode *new_node = malloc(sizeof(HashNode));
    new_node->key = key;
    new_node->value = value;
    new_node->next = ht->buckets[idx];
    ht->buckets[idx] = new_node;
}

int cht_search(ChainedHashTable *ht, int key) {
    int idx = cht_hash(key);
    HashNode *curr = ht->buckets[idx];
    while (curr) {
        if (curr->key == key) return curr->value;
        curr = curr->next;
    }
    return -1;
}

void cht_free(ChainedHashTable *ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *curr = ht->buckets[i];
        while (curr) {
            HashNode *next = curr->next;
            free(curr);
            curr = next;
        }
    }
}

int main(void) {
    ChainedHashTable ht;
    cht_init(&ht);

    printf("--- Chained Hash Table Collision Resolution ---\n");
    cht_insert(&ht, 1, 10);
    cht_insert(&ht, 6, 60);  // Collides with 1 in table size 5
    cht_insert(&ht, 11, 110); // Collides with 1 and 6

    printf("Search Key 1  : %d\n", cht_search(&ht, 1));
    printf("Search Key 6  : %d\n", cht_search(&ht, 6));
    printf("Search Key 11 : %d\n", cht_search(&ht, 11));

    cht_free(&ht);
    return 0;
}
