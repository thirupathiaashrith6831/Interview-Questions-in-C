#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *tree;
    int size;
} FenwickTree;

FenwickTree* fenwick_create(int size) {
    FenwickTree *ft = malloc(sizeof(FenwickTree));
    ft->size = size;
    ft->tree = calloc(size + 1, sizeof(int));
    return ft;
}

// Point update: Add val to element at index `idx` (1-based)
void fenwick_add(FenwickTree *ft, int idx, int val) {
    for (; idx <= ft->size; idx += idx & -idx) {
        ft->tree[idx] += val;
    }
}

// Prefix sum: Calculate sum from index 1 to `idx` (1-based)
int fenwick_query(FenwickTree *ft, int idx) {
    int sum = 0;
    for (; idx > 0; idx -= idx & -idx) {
        sum += ft->tree[idx];
    }
    return sum;
}

// Range sum query: Sum from index `l` to `r`
int fenwick_range_query(FenwickTree *ft, int l, int r) {
    return fenwick_query(ft, r) - fenwick_query(ft, l - 1);
}

void fenwick_free(FenwickTree *ft) {
    free(ft->tree);
    free(ft);
}

int main(void) {
    int arr[] = {2, 1, 1, 3, 2, 3, 4, 5, 6, 78, 9, 10};
    int n = sizeof(arr) / sizeof(arr[0]);

    FenwickTree *ft = fenwick_create(n);

    // Build Fenwick Tree
    for (int i = 0; i < n; i++) {
        fenwick_add(ft, i + 1, arr[i]);
    }

    printf("--- Fenwick Tree (Binary Indexed Tree) ---\n");
    printf("Prefix Sum (1 to 6) : %d\n", fenwick_query(ft, 6)); // Expected: 2+1+1+3+2+3 = 12
    printf("Range Sum (index 3 to 7): %d\n", fenwick_range_query(ft, 3, 7)); // 1+3+2+3+4 = 13

    printf("\nUpdating index 4 by +6...\n");
    fenwick_add(ft, 4, 6); // arr[3] was 3, now 9

    printf("Updated Range Sum (3 to 7): %d\n", fenwick_range_query(ft, 3, 7));

    fenwick_free(ft);
    return 0;
}
