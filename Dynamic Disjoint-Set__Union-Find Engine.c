#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *parent;
    int *rank;
    int size;
} DisjointSet;

DisjointSet* dsu_create(int size) {
    DisjointSet *dsu = malloc(sizeof(DisjointSet));
    dsu->size = size;
    dsu->parent = malloc(sizeof(int) * size);
    dsu->rank = malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++) {
        dsu->parent[i] = i;
        dsu->rank[i] = 0;
    }
    return dsu;
}

// Find with Path Compression
int dsu_find(DisjointSet *dsu, int i) {
    if (dsu->parent[i] == i)
        return i;
    return dsu->parent[i] = dsu_find(dsu, dsu->parent[i]);
}

// Union by Rank
bool dsu_union(DisjointSet *dsu, int i, int j) {
    int root_i = dsu_find(dsu, i);
    int root_j = dsu_find(dsu, j);

    if (root_i == root_j) return false; // Cycle or already connected

    if (dsu->rank[root_i] < dsu->rank[root_j]) {
        dsu->parent[root_i] = root_j;
    } else if (dsu->rank[root_i] > dsu->rank[root_j]) {
        dsu->parent[root_j] = root_i;
    } else {
        dsu->parent[root_j] = root_i;
        dsu->rank[root_i]++;
    }
    return true;
}

void dsu_free(DisjointSet *dsu) {
    free(dsu->parent);
    free(dsu->rank);
    free(dsu);
}

int main(void) {
    int nodes = 6;
    DisjointSet *dsu = dsu_create(nodes);

    printf("--- Disjoint-Set Union (DSU) Engine ---\n");
    printf("Connecting 0-1: %s\n", dsu_union(dsu, 0, 1) ? "Success" : "Already Connected");
    printf("Connecting 1-2: %s\n", dsu_union(dsu, 1, 2) ? "Success" : "Already Connected");
    printf("Connecting 3-4: %s\n", dsu_union(dsu, 3, 4) ? "Success" : "Already Connected");

    printf("\nAre 0 and 2 in same set? %s\n", dsu_find(dsu, 0) == dsu_find(dsu, 2) ? "YES" : "NO");
    printf("Are 0 and 3 in same set? %s\n", dsu_find(dsu, 0) == dsu_find(dsu, 3) ? "YES" : "NO");

    dsu_free(dsu);
    return 0;
}
