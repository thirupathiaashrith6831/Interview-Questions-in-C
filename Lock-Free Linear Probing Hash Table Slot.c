#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>

#define TABLE_CAPACITY 8
#define EMPTY 0

typedef struct {
    _Atomic int key;
    _Atomic int value;
} LFHashTable;

void lfht_init(LFHashTable *ht) {
    for (int i = 0; i < TABLE_CAPACITY; i++) {
        atomic_init(&ht[i].key, EMPTY);
        atomic_init(&ht[i].value, 0);
    }
}

bool lfht_insert(LFHashTable *ht, int key, int value) {
    if (key == EMPTY) return false;
    int idx = key % TABLE_CAPACITY;

    for (int i = 0; i < TABLE_CAPACITY; i++) {
        int probe = (idx + i) % TABLE_CAPACITY;
        int expected_key = EMPTY;

        if (atomic_compare_exchange_strong_explicit(&ht[probe].key, &expected_key, key,
            memory_order_acq_rel, memory_order_relaxed)) {
            atomic_store_explicit(&ht[probe].value, value, memory_order_release);
            return true;
        }
        
        // If key already matches, update value
        if (atomic_load_explicit(&ht[probe].key, memory_order_relaxed) == key) {
            atomic_store_explicit(&ht[probe].value, value, memory_order_release);
            return true;
        }
    }
    return false; // Table full
}

bool lfht_search(LFHashTable *ht, int key, int *value) {
    int idx = key % TABLE_CAPACITY;
    for (int i = 0; i < TABLE_CAPACITY; i++) {
        int probe = (idx + i) % TABLE_CAPACITY;
        int k = atomic_load_explicit(&ht[probe].key, memory_order_acquire);
        if (k == EMPTY) return false;
        if (k == key) {
            *value = atomic_load_explicit(&ht[probe].value, memory_order_acquire);
            return true;
        }
    }
    return false;
}

int main(void) {
    LFHashTable ht[TABLE_CAPACITY];
    lfht_init(ht);

    printf("--- Lock-Free Linear Probing Hash Table ---\n");
    lfht_insert(ht, 12, 1200);
    lfht_insert(ht, 20, 2000); // Collides with 12 (12 % 8 == 20 % 8 == 4)

    int val;
    if (lfht_search(ht, 12, &val)) {
        printf("Found Key 12: Value = %d\n", val);
    }
    if (lfht_search(ht, 20, &val)) {
        printf("Found Key 20: Value = %d (Resolved via linear probing)\n", val);
    }

    return 0;
}
