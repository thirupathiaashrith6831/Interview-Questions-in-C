#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define BIT_ARRAY_SIZE 128

typedef struct {
    uint8_t bits[BIT_ARRAY_SIZE / 8];
} BloomFilter;

// Hash Function 1 (djb2)
uint32_t hash1(const char *str) {
    uint32_t hash = 5381;
    int c;
    while ((c = *str++)) hash = ((hash << 5) + hash) + c;
    return hash;
}

// Hash Function 2 (sdbm)
uint32_t hash2(const char *str) {
    uint32_t hash = 0;
    int c;
    while ((c = *str++)) hash = c + (hash << 6) + (hash << 16) - hash;
    return hash;
}

void bloom_add(BloomFilter *filter, const char *str) {
    uint32_t h1 = hash1(str) % BIT_ARRAY_SIZE;
    uint32_t h2 = hash2(str) % BIT_ARRAY_SIZE;

    filter->bits[h1 / 8] |= (1 << (h1 % 8));
    filter->bits[h2 / 8] |= (1 << (h2 % 8));
}

bool bloom_contains(const BloomFilter *filter, const char *str) {
    uint32_t h1 = hash1(str) % BIT_ARRAY_SIZE;
    uint32_t h2 = hash2(str) % BIT_ARRAY_SIZE;

    bool b1 = filter->bits[h1 / 8] & (1 << (h1 % 8));
    bool b2 = filter->bits[h2 / 8] & (1 << (h2 % 8));

    return b1 && b2;
}

int main(void) {
    BloomFilter filter = {0};

    bloom_add(&filter, "apple");
    bloom_add(&filter, "banana");

    printf("--- Bloom Filter Query Results ---\n");
    printf("Contains 'apple' : %s\n", bloom_contains(&filter, "apple") ? "YES" : "NO");
    printf("Contains 'banana': %s\n", bloom_contains(&filter, "banana") ? "YES" : "NO");
    printf("Contains 'cherry': %s\n", bloom_contains(&filter, "cherry") ? "YES" : "NO");

    return 0;
}
