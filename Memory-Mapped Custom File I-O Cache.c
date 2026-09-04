#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PAGE_SIZE 16
#define NUM_PAGES 4

typedef struct {
    int page_id;
    char data[PAGE_SIZE];
    bool dirty;
    bool valid;
} PageFrame;

typedef struct {
    PageFrame frames[NUM_PAGES];
    int hits;
    int misses;
} MemoryCache;

void cache_init(MemoryCache *cache) {
    cache->hits = 0;
    cache->misses = 0;
    for (int i = 0; i < NUM_PAGES; i++) {
        cache->frames[i].valid = false;
        cache->frames[i].dirty = false;
        cache->frames[i].page_id = -1;
    }
}

char cache_read(MemoryCache *cache, const char *virtual_disk, int address) {
    int page_id = address / PAGE_SIZE;
    int offset = address % PAGE_SIZE;

    // Search for hit in cache
    for (int i = 0; i < NUM_PAGES; i++) {
        if (cache->frames[i].valid && cache->frames[i].page_id == page_id) {
            cache->hits++;
            return cache->frames[i].data[offset];
        }
    }

    // Cache Miss: load page into first free frame (or FIFO replacement slot 0)
    cache->misses++;
    int slot = cache->misses % NUM_PAGES;

    cache->frames[slot].page_id = page_id;
    cache->frames[slot].valid = true;
    memcpy(cache->frames[slot].data, &virtual_disk[page_id * PAGE_SIZE], PAGE_SIZE);

    return cache->frames[slot].data[offset];
}

int main(void) {
    // 64-byte mock storage
    const char disk_data[] = "Page0_Data_Block" "Page1_Data_Block" "Page2_Data_Block" "Page3_Data_Block";

    MemoryCache cache;
    cache_init(&cache);

    printf("--- Memory-Mapped Page Cache Engine ---\n");

    // Sequential & repeat address access simulation
    int read_addresses[] = {2, 5, 20, 2, 35, 5};
    for (size_t i = 0; i < sizeof(read_addresses)/sizeof(read_addresses[0]); i++) {
        int addr = read_addresses[i];
        char val = cache_read(&cache, disk_data, addr);
        printf("Read Address %2d (Page %d): '%c'\n", addr, addr / PAGE_SIZE, val);
    }

    printf("\nCache Statistics -> Hits: %d | Misses: %d\n", cache.hits, cache.misses);
    return 0;
}
