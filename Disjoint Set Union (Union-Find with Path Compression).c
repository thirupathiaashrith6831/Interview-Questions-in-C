#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *bit;
    int n;
} FenwickTree;

FenwickTree* fenwick_create(int n) {
    FenwickTree *ft = malloc(sizeof(FenwickTree));
    ft->n = n;
    ft->bit = calloc(n + 1, sizeof(int)); // 1-based indexing
    return ft;
}

void fenwick_update(FenwickTree *ft, int idx, int delta) {
    for (; idx <= ft->n; idx += idx & -idx) {
        ft->bit[idx] += delta;
    }
}

int fenwick_prefix_sum(const FenwickTree *ft, int idx) {
    int sum = 0;
    for (; idx > 0; idx -= idx & -idx) {
        sum += ft->bit[idx];
    }
    return sum;
}

int fenwick_range_sum(const FenwickTree *ft, int left, int right) {
    return fenwick_prefix_sum(ft, right) - fenwick_prefix_sum(ft, left - 1);
}

void fenwick_free(FenwickTree *ft) {
    free(ft->bit);
    free(ft);
}

int main(void) {
    int arr[] = {3, 2, -1, 6, 5, 4, -3, 3};
    int n = sizeof(arr) / sizeof(arr[0]);

    FenwickTree *ft = fenwick_create(n);
    for (int i = 0; i < n; i++) {
        fenwick_update(ft, i + 1, arr[i]);
    }

    printf("--- Fenwick Tree (Binary Indexed Tree) ---\n");
    printf("Prefix sum [1..5] (3+2-1+6+5): %d\n", fenwick_prefix_sum(ft, 5));
    printf("Range sum  [3..6] (-1+6+5+4) : %d\n", fenwick_range_sum(ft, 3, 6));

    printf("Adding +4 to index 4...\n");
    fenwick_update(ft, 4, 4);
    printf("Updated Range sum [3..6]     : %d\n", fenwick_range_sum(ft, 3, 6));

    fenwick_free(ft);
    return 0;
}
