#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TABLE_SIZE 10
#define EMPTY_KEY -1
#define DELETED_KEY -2

typedef struct {
    int key;
    int value;
} HashEntry;

typedef struct {
    HashEntry entries[TABLE_SIZE];
    int count;
} HashTable;

void ht_init(HashTable *ht) {
    ht->count = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->entries[i].key = EMPTY_KEY;
        ht->entries[i].value = 0;
    }
}

int hash_function(int key) {
    return (key & 0x7FFFFFFF) % TABLE_SIZE;
}

bool ht_insert(HashTable *ht, int key, int value) {
    if (ht->count >= TABLE_SIZE) return false;

    int index = hash_function(key);
    int first_deleted = -1;

    for (int i = 0; i < TABLE_SIZE; i++) {
        int probe_idx = (index + i) % TABLE_SIZE;

        if (ht->entries[probe_idx].key == key) {
            ht->entries[probe_idx].value = value; // Update existing key
            return true;
        }

        if (ht->entries[probe_idx].key == DELETED_KEY && first_deleted == -1) {
            first_deleted = probe_idx;
        }

        if (ht->entries[probe_idx].key == EMPTY_KEY) {
            int insert_idx = (first_deleted != -1) ? first_deleted : probe_idx;
            ht->entries[insert_idx].key = key;
            ht->entries[insert_idx].value = value;
            ht->count++;
            return true;
        }
    }
    return false;
}

int ht_search(HashTable *ht, int key) {
    int index = hash_function(key);

    for (int i = 0; i < TABLE_SIZE; i++) {
        int probe_idx = (index + i) % TABLE_SIZE;

        if (ht->entries[probe_idx].key == key) {
            return ht->entries[probe_idx].value;
        }
        if (ht->entries[probe_idx].key == EMPTY_KEY) {
            break;
        }
    }
    return -1; // Not found
}

void ht_display(HashTable *ht) {
    printf("Slot | Key    | Value\n");
    printf("---------------------\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (ht->entries[i].key == EMPTY_KEY) {
            printf("[%2d] | EMPTY  | -\n", i);
        } else if (ht->entries[i].key == DELETED_KEY) {
            printf("[%2d] | DELETED| -\n", i);
        } else {
            printf("[%2d] | %-6d | %d\n", i, ht->entries[i].key, ht->entries[i].value);
        }
    }
}

int main(void) {
    HashTable ht;
    ht_init(&ht);

    printf("--- Hash Table with Linear Probing ---\n");
    ht_insert(&ht, 12, 100); // 12 % 10 = index 2
    ht_insert(&ht, 22, 200); // Collision at index 2 -> probes to 3
    ht_insert(&ht, 42, 400); // Collision at index 2 -> probes to 4

    ht_display(&ht);

    printf("\nSearch Key 22: Value = %d\n", ht_search(&ht, 22));
    printf("Search Key 99: Value = %d (Expected -1)\n", ht_search(&ht, 99));

    return 0;
}
