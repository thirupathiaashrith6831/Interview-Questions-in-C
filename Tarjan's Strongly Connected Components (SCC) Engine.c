#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_V 10

typedef struct {
    int V;
    int adj[MAX_V][MAX_V];
    int adj_size[MAX_V];
} Graph;

Graph* create_graph(int V) {
    Graph *g = malloc(sizeof(Graph));
    g->V = V;
    for (int i = 0; i < V; i++) {
        g->adj_size[i] = 0;
    }
    return g;
}

void add_edge(Graph *g, int u, int v) {
    g->adj[u][g->adj_size[u]++] = v;
}

int min(int a, int b) { return (a < b) ? a : b; }

void scc_util(Graph *g, int u, int disc[], int low[], int stack[], bool in_stack[], int *time_cnt, int *top) {
    disc[u] = low[u] = ++(*time_cnt);
    stack[++(*top)] = u;
    in_stack[u] = true;

    for (int i = 0; i < g->adj_size[u]; i++) {
        int v = g->adj[u][i];

        if (disc[v] == -1) {
            scc_util(g, v, disc, low, stack, in_stack, time_cnt, top);
            low[u] = min(low[u], low[v]);
        } else if (in_stack[v]) {
            low[u] = min(low[u], disc[v]);
        }
    }

    // Root node of SCC found
    if (low[u] == disc[u]) {
        printf("SCC Component: ");
        while (1) {
            int v = stack[(*top)--];
            in_stack[v] = false;
            printf("%d ", v);
            if (u == v) break;
        }
        printf("\n");
    }
}

void tarjan_scc(Graph *g) {
    int disc[MAX_V], low[MAX_V], stack[MAX_V];
    bool in_stack[MAX_V];
    int time_cnt = 0, top = -1;

    for (int i = 0; i < g->V; i++) {
        disc[i] = -1;
        low[i] = -1;
        in_stack[i] = false;
    }

    for (int i = 0; i < g->V; i++) {
        if (disc[i] == -1) {
            scc_util(g, i, disc, low, stack, in_stack, &time_cnt, &top);
        }
    }
}

int main(void) {
    Graph *g = create_graph(5);

    add_edge(g, 1, 0);
    add_edge(g, 0, 2);
    add_edge(g, 2, 1);
    add_edge(g, 0, 3);
    add_edge(g, 3, 4);

    printf("--- Tarjan's Strongly Connected Components (SCC) ---\n");
    tarjan_scc(g);

    free(g);
    return 0;
}
