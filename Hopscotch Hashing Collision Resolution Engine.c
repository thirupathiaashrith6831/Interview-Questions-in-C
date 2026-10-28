#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define TABLE_SIZE 16
#define HOP_RANGE 4

typedef struct {
    int key;
    int value;
} HopEntry;

typedef struct {
    HopEntry entries[TABLE_SIZE];
    uint32_t hop_bitmap[TABLE_SIZE]; // Neighborhood bitmask
    bool occupied[TABLE_SIZE];
} HopscotchHashTable;

void hop_init(HopscotchHashTable *ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->occupied[i] = false;
        ht->hop_bitmap[i] = 0;
    }
}

int hash_func(int key) {
    return key % TABLE_SIZE;
}

bool hop_insert(HopscotchHashTable *ht, int key, int value) {
    int ideal = hash_func(key);
    int curr = ideal;

    // Find first empty slot
    while (curr < TABLE_SIZE && ht->occupied[curr]) {
        curr++;
    }
    if (curr >= TABLE_SIZE) return false; // Table full

    // Relocate empty slot closer to ideal if too far
    while (curr - ideal >= HOP_RANGE) {
        int moved = 0;
        for (int h = HOP_RANGE - 1; h > 0; h--) {
            int candidate = curr - h;
            // Find an entry in neighborhood that can hop to fill empty slot
            for (int b = 0; b < HOP_RANGE; b++) {
                if ((ht->hop_bitmap[candidate] & (1 << b)) && (candidate + b < curr)) {
                    // Swap items
                    int target = candidate + b;
                    ht->entries[curr] = ht->entries[target];
                    ht->occupied[curr] = true;
                    ht->occupied[target] = false;
                    ht->hop_bitmap[candidate] &= ~(1 << b);
                    ht->hop_bitmap[candidate] |= (1 << (curr - candidate));
                    curr = target;
                    moved = 1;
                    break;
                }
            }
            if (moved) break;
        }
        if (!moved) return false; // Relocation failed
    }

    ht->entries[curr].key = key;
    ht->entries[curr].value = value;
    ht->occupied[curr] = true;
    ht->hop_bitmap[ideal] |= (1 << (curr - ideal));
    return true;
}

int hop_search(HopscotchHashTable *ht, int key) {
    int ideal = hash_func(key);
    uint32_t bitmap = ht->hop_bitmap[ideal];

    for (int i = 0; i < HOP_RANGE; i++) {
        if (bitmap & (1 << i)) {
            int idx = (ideal + i) % TABLE_SIZE;
            if (ht->occupied[idx] && ht->entries[idx].key == key) {
                return ht->entries[idx].value;
            }
        }
    }
    return -1;
}

int main(void) {
    HopscotchHashTable ht;
    hop_init(&ht);

    printf("--- Hopscotch Hashing Collision Resolution ---\n");
    hop_insert(&ht, 10, 100);
    hop_insert(&ht, 26, 260); // Collides with 10 (10 % 16 == 26 % 16 == 10)
    hop_insert(&ht, 42, 420); // Collides in same neighborhood

    printf("Search Key 10: Value = %d\n", hop_search(&ht, 10));
    printf("Search Key 26: Value = %d\n", hop_search(&ht, 26));
    printf("Search Key 42: Value = %d\n", hop_search(&ht, 42));
    printf("Search Key 99: Value = %d (Expected -1)\n", hop_search(&ht, 99));

    return 0;
}
