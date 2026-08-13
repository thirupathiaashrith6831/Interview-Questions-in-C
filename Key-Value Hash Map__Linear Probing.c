#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 11

typedef struct {
    char *key;
    int value;
    bool occupied;
} HashEntry;

typedef struct {
    HashEntry entries[TABLE_SIZE];
} HashMap;

unsigned int hash_function(const char *key) {
    unsigned int hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TABLE_SIZE;
}

void map_init(HashMap *map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        map->entries[i].key = NULL;
        map->entries[i].occupied = false;
    }
}

bool map_put(HashMap *map, const char *key, int value) {
    unsigned int index = hash_function(key);
    for (int i = 0; i < TABLE_SIZE; i++) {
        unsigned int probe = (index + i) % TABLE_SIZE;
        if (!map->entries[probe].occupied || strcmp(map->entries[probe].key, key) == 0) {
            if (map->entries[probe].occupied) {
                free(map->entries[probe].key);
            }
            map->entries[probe].key = strdup(key);
            map->entries[probe].value = value;
            map->entries[probe].occupied = true;
            return true;
        }
    }
    return false; // Table is full
}

bool map_get(const HashMap *map, const char *key, int *out_value) {
    unsigned int index = hash_function(key);
    for (int i = 0; i < TABLE_SIZE; i++) {
        unsigned int probe = (index + i) % TABLE_SIZE;
        if (!map->entries[probe].occupied) return false;
        if (strcmp(map->entries[probe].key, key) == 0) {
            *out_value = map->entries[probe].value;
            return true;
        }
    }
    return false;
}

void map_free(HashMap *map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (map->entries[i].occupied) {
            free(map->entries[i].key);
        }
    }
}

int main(void) {
    HashMap map;
    map_init(&map);

    printf("--- Key-Value Hash Map (Linear Probing) ---\n");
    map_put(&map, "alice", 95);
    map_put(&map, "bob", 88);
    map_put(&map, "charlie", 92);

    int score;
    if (map_get(&map, "bob", &score)) {
        printf("Found 'bob' with score: %d\n", score);
    }
    if (map_get(&map, "alice", &score)) {
        printf("Found 'alice' with score: %d\n", score);
    }

    map_free(&map);
    return 0;
}
