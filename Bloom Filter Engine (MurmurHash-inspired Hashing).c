#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define FILTER_SIZE 128 // Bit vector length

typedef struct {
    uint8_t bits[FILTER_SIZE / 8];
} BloomFilter;

void bloom_init(BloomFilter *filter) {
    memset(filter->bits, 0, sizeof(filter->bits));
}

// Simple hash 1: DJB2 hash
uint32_t hash1(const char *str) {
    uint32_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % FILTER_SIZE;
}

// Simple hash 2: FNV-1a hash
uint32_t hash2(const char *str) {
    uint32_t hash = 2166136261u;
    while (*str) {
        hash ^= (uint8_t)*str++;
        hash *= 16777619u;
    }
    return hash % FILTER_SIZE;
}

void bloom_add(BloomFilter *filter, const char *key) {
    uint32_t h1 = hash1(key);
    uint32_t h2 = hash2(key);

    filter->bits[h1 / 8] |= (1 << (h1 % 8));
    filter->bits[h2 / 8] |= (1 << (h2 % 8));
}

bool bloom_check(const BloomFilter *filter, const char *key) {
    uint32_t h1 = hash1(key);
    uint32_t h2 = hash2(key);

    bool b1 = (filter->bits[h1 / 8] & (1 << (h1 % 8))) != 0;
    bool b2 = (filter->bits[h2 / 8] & (1 << (h2 % 8))) != 0;

    return b1 && b2;
}

int main(void) {
    BloomFilter filter;
    bloom_init(&filter);

    printf("--- Bloom Filter Probabilistic Engine ---\n");
    bloom_add(&filter, "apple");
    bloom_add(&filter, "google");
    bloom_add(&filter, "microsoft");

    printf("Check 'apple'    : %s\n", bloom_check(&filter, "apple") ? "MAYBE PRESENT" : "DEFINITELY NOT");
    printf("Check 'google'   : %s\n", bloom_check(&filter, "google") ? "MAYBE PRESENT" : "DEFINITELY NOT");
    printf("Check 'netflix'  : %s\n", bloom_check(&filter, "netflix") ? "MAYBE PRESENT" : "DEFINITELY NOT");

    return 0;
}
