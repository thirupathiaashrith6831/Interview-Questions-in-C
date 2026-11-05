#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define BITS 6               // 2^6 = 64 registers
#define NUM_REGISTERS (1 << BITS)

typedef struct {
    uint8_t registers[NUM_REGISTERS];
} HyperLogLog;

void hll_init(HyperLogLog *hll) {
    memset(hll->registers, 0, sizeof(hll->registers));
}

// 64-bit MurmurHash3-inspired string hash function
uint64_t hll_hash(const char *str) {
    uint64_t h = 0xcbf29ce484222325ULL;
    while (*str) {
        h ^= (uint8_t)*str++;
        h *= 0x100000001b3ULL;
    }
    return h;
}

uint8_t count_leading_zeros(uint64_t val) {
    if (val == 0) return 64 - BITS;
    uint8_t count = 1;
    while ((val & (1ULL << 63)) == 0) {
        count++;
        val <<= 1;
    }
    return count;
}

void hll_add(HyperLogLog *hll, const char *key) {
    uint64_t hash = hll_hash(key);
    uint32_t index = hash >> (64 - BITS);
    uint64_t w = hash << BITS;
    uint8_t rho = count_leading_zeros(w);

    if (rho > hll->registers[index]) {
        hll->registers[index] = rho;
    }
}

double hll_estimate(const HyperLogLog *hll) {
    double alpha_m = 0.7213 / (1.0 + 1.079 / NUM_REGISTERS);
    double sum = 0.0;

    for (int i = 0; i < NUM_REGISTERS; i++) {
        sum += 1.0 / (1 << hll->registers[i]);
    }

    double estimate = alpha_m * NUM_REGISTERS * NUM_REGISTERS / sum;
    return estimate;
}

int main(void) {
    HyperLogLog hll;
    hll_init(&hll);

    printf("--- HyperLogLog Probabilistic Cardinality Estimator ---\n");
    char item_buf[32];
    int true_cardinality = 5000;

    for (int i = 0; i < true_cardinality; i++) {
        snprintf(item_buf, sizeof(item_buf), "user_session_%d", i);
        hll_add(&hll, item_buf);
    }

    double estimated = hll_estimate(&hll);
    printf("True Unique Count     : %d\n", true_cardinality);
    printf("Estimated Cardinality : %.0f\n", estimated);
    printf("Estimation Error      : %.2f%%\n", 
           fabs(estimated - true_cardinality) / true_cardinality * 100.0);

    return 0;
}
