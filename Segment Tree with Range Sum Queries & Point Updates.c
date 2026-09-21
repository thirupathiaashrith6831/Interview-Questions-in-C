#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *tree;
    int n;
} SegmentTree;

void build_tree(int arr[], int tree[], int node, int start, int end) {
    if (start == end) {
        tree[node] = arr[start];
        return;
    }
    int mid = start + (end - start) / 2;
    build_tree(arr, tree, 2 * node, start, mid);
    build_tree(arr, tree, 2 * node + 1, mid + 1, end);
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

SegmentTree* segtree_create(int arr[], int n) {
    SegmentTree *st = malloc(sizeof(SegmentTree));
    st->n = n;
    st->tree = malloc(sizeof(int) * (4 * n));
    build_tree(arr, st->tree, 1, 0, n - 1);
    return st;
}

int query_range(SegmentTree *st, int node, int start, int end, int l, int r) {
    if (r < start || end < l) return 0; // Completely outside
    if (l <= start && end <= r) return st->tree[node]; // Completely inside

    int mid = start + (end - start) / 2;
    int left_sum = query_range(st, 2 * node, start, mid, l, r);
    int right_sum = query_range(st, 2 * node + 1, mid + 1, end, l, r);
    return left_sum + right_sum;
}

void update_point(SegmentTree *st, int node, int start, int end, int idx, int val) {
    if (start == end) {
        st->tree[node] = val;
        return;
    }
    int mid = start + (end - start) / 2;
    if (start <= idx && idx <= mid) {
        update_point(st, 2 * node, start, mid, idx, val);
    } else {
        update_point(st, 2 * node + 1, mid + 1, end, idx, val);
    }
    st->tree[node] = st->tree[2 * node] + st->tree[2 * node + 1];
}

void segtree_free(SegmentTree *st) {
    free(st->tree);
    free(st);
}

int main(void) {
    int arr[] = {1, 3, 5, 7, 9, 11};
    int n = sizeof(arr) / sizeof(arr[0]);

    SegmentTree *st = segtree_create(arr, n);

    printf("--- Segment Tree Range Sum Engine ---\n");
    printf("Sum of range [1, 3] (indices 1 to 3): %d\n", query_range(st, 1, 0, n - 1, 1, 3)); // 3 + 5 + 7 = 15

    printf("Updating index 1 to value 10...\n");
    update_point(st, 1, 0, n - 1, 1, 10);

    printf("New sum of range [1, 3]: %d\n", query_range(st, 1, 0, n - 1, 1, 3)); // 10 + 5 + 7 = 22

    segtree_free(st);
    return 0;
}
