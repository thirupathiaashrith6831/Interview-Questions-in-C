#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int src, dest, weight;
} Edge;

typedef struct {
    int V, E;
    Edge *edges;
} Graph;

typedef struct {
    int *parent;
    int *rank;
} Subset;

Graph* create_graph(int V, int E) {
    Graph *g = malloc(sizeof(Graph));
    g->V = V;
    g->E = E;
    g->edges = malloc(sizeof(Edge) * E);
    return g;
}

int find(Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

void union_sets(Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank) {
        subsets[xroot].parent = yroot;
    } else if (subsets[xroot].rank > subsets[yroot].rank) {
        subsets[yroot].parent = xroot;
    } else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

int compare_edges(const void *a, const void *b) {
    Edge *a_edge = (Edge*)a;
    Edge *b_edge = (Edge*)b;
    return a_edge->weight - b_edge->weight;
}

void kruskal_mst(Graph *g) {
    Edge result[g->V];
    int e = 0;
    int i = 0;

    qsort(g->edges, g->E, sizeof(Edge), compare_edges);

    Subset *subsets = malloc(sizeof(Subset) * g->V);
    for (int v = 0; v < g->V; v++) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    while (e < g->V - 1 && i < g->E) {
        Edge next_edge = g->edges[i++];

        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);

        if (x != y) {
            result[e++] = next_edge;
            union_sets(subsets, x, y);
        }
    }

    printf("Edges in Minimum Spanning Tree:\n");
    int total_weight = 0;
    for (i = 0; i < e; i++) {
        printf("  %d -- %d == %d\n", result[i].src, result[i].dest, result[i].weight);
        total_weight += result[i].weight;
    }
    printf("Total MST Weight: %d\n", total_weight);

    free(subsets);
}

int main(void) {
    int V = 4, E = 5;
    Graph *g = create_graph(V, E);

    // Edge 0-1
    g->edges[0] = (Edge){0, 1, 10};
    // Edge 0-2
    g->edges[1] = (Edge){0, 2, 6};
    // Edge 0-3
    g->edges[2] = (Edge){0, 3, 5};
    // Edge 1-3
    g->edges[3] = (Edge){1, 3, 15};
    // Edge 2-3
    g->edges[4] = (Edge){2, 3, 4};

    printf("--- Kruskal's MST Algorithm ---\n");
    kruskal_mst(g);

    free(g->edges);
    free(g);
    return 0;
}
