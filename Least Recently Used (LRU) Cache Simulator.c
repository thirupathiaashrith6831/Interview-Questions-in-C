#include <stdio.h>
#include <stdbool.h>

#define CACHE_CAPACITY 3

typedef struct {
    int key;
    int value;
    int last_used;
} CacheNode;

typedef struct {
    CacheNode nodes[CACHE_CAPACITY];
    int size;
    int timer;
} LRUCache;

void lru_init(LRUCache *cache) {
    cache->size = 0;
    cache->timer = 0;
}

int lru_get(LRUCache *cache, int key) {
    cache->timer++;
    for (int i = 0; i < cache->size; i++) {
        if (cache->nodes[i].key == key) {
            cache->nodes[i].last_used = cache->timer;
            return cache->nodes[i].value;
        }
    }
    return -1; // Cache miss
}

void lru_put(LRUCache *cache, int key, int value) {
    cache->timer++;
    
    // Key exists: update value and timestamp
    for (int i = 0; i < cache->size; i++) {
        if (cache->nodes[i].key == key) {
            cache->nodes[i].value = value;
            cache->nodes[i].last_used = cache->timer;
            return;
        }
    }
    
    // Space available: insert key
    if (cache->size < CACHE_CAPACITY) {
        cache->nodes[cache->size].key = key;
        cache->nodes[cache->size].value = value;
        cache->nodes[cache->size].last_used = cache->timer;
        cache->size++;
        return;
    }
    
    // Cache full: find oldest (LRU) entry and replace
    int lru_idx = 0;
    for (int i = 1; i < CACHE_CAPACITY; i++) {
        if (cache->nodes[i].last_used < cache->nodes[lru_idx].last_used) {
            lru_idx = i;
        }
    }
    printf("Evicting key %d (LRU)\n", cache->nodes[lru_idx].key);
    cache->nodes[lru_idx].key = key;
    cache->nodes[lru_idx].value = value;
    cache->nodes[lru_idx].last_used = cache->timer;
}

int main(void) {
    LRUCache cache;
    lru_init(&cache);

    printf("--- LRU Cache Simulation ---\n");
    lru_put(&cache, 1, 100);
    lru_put(&cache, 2, 200);
    lru_put(&cache, 3, 300);

    printf("Get Key 1: %d\n", lru_get(&cache, 1)); // Touches Key 1

    lru_put(&cache, 4, 400); // Key 2 is least recently used and evicted

    printf("Get Key 2: %d (Expected -1 for Miss)\n", lru_get(&cache, 2));
    printf("Get Key 4: %d\n", lru_get(&cache, 4));

    return 0;
}
