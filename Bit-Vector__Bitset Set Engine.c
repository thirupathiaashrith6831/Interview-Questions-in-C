#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_ELEMENTS 64

typedef struct {
    uint64_t bits; // 64-bit vector representing elements 0 to 63
} BitSet;

void bitset_init(BitSet *s) { s->bits = 0; }

void bitset_add(BitSet *s, int element) {
    if (element >= 0 && element < MAX_ELEMENTS) {
        s->bits |= (1ULL << element);
    }
}

void bitset_remove(BitSet *s, int element) {
    if (element >= 0 && element < MAX_ELEMENTS) {
        s->bits &= ~(1ULL << element);
    }
}

bool bitset_contains(const BitSet *s, int element) {
    if (element >= 0 && element < MAX_ELEMENTS) {
        return (s->bits & (1ULL << element)) != 0;
    }
    return false;
}

BitSet bitset_union(const BitSet *a, const BitSet *b) {
    BitSet result;
    result.bits = a->bits | b->bits;
    return result;
}

BitSet bitset_intersection(const BitSet *a, const BitSet *b) {
    BitSet result;
    result.bits = a->bits & b->bits;
    return result;
}

void bitset_print(const BitSet *s, const char *name) {
    printf("%s: { ", name);
    for (int i = 0; i < MAX_ELEMENTS; i++) {
        if (bitset_contains(s, i)) {
            printf("%d ", i);
        }
    }
    printf("}\n");
}

int main(void) {
    BitSet setA, setB;
    bitset_init(&setA);
    bitset_init(&setB);

    bitset_add(&setA, 3);
    bitset_add(&setA, 12);
    bitset_add(&setA, 45);

    bitset_add(&setB, 12);
    bitset_add(&setB, 30);
    bitset_add(&setB, 45);

    printf("--- Bit-Vector / Bitset Engine ---\n");
    bitset_print(&setA, "Set A");
    bitset_print(&setB, "Set B");

    BitSet u = bitset_union(&setA, &setB);
    bitset_print(&u, "Union (A U B)");

    BitSet i = bitset_intersection(&setA, &setB);
    bitset_print(&i, "Intersection (A n B)");

    return 0;
}
