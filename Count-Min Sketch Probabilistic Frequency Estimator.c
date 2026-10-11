#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define WIDTH 10
#define DEPTH 3

typedef struct {
    int table[DEPTH][WIDTH];
} CountMinSketch;

void cms_init(CountMinSketch *cms) {
    memset(cms->table, 0, sizeof(cms->table));
}

uint32_t hash_func(const char *str, int seed) {
    uint32_t hash = seed;
    while (*str) {
        hash = (hash * 31) + (uint8_t)(*str++);
    }
    return hash % WIDTH;
}

void cms_add(CountMinSketch *cms, const char *key) {
    for (int i = 0; i < DEPTH; i++) {
        int col = hash_func(key, i + 100);
        cms->table[i][col]++;
    }
}

int cms_estimate(CountMinSketch *cms, const char *key) {
    int min_val = 1e9;
    for (int i = 0; i < DEPTH; i++) {
        int col = hash_func(key, i + 100);
        if (cms->table[i][col] < min_val) {
            min_val = cms->table[i][col];
        }
    }
    return min_val;
}

int main(void) {
    CountMinSketch cms;
    cms_init(&cms);

    printf("--- Count-Min Sketch Frequency Estimator ---\n");
    cms_add(&cms, "apple");
    cms_add(&cms, "apple");
    cms_add(&cms, "banana");
    cms_add(&cms, "apple");

    printf("Estimated frequency of 'apple'  : %d\n", cms_estimate(&cms, "apple"));
    printf("Estimated frequency of 'banana' : %d\n", cms_estimate(&cms, "banana"));
    printf("Estimated frequency of 'orange' : %d\n", cms_estimate(&cms, "orange"));

    return 0;
}
