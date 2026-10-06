#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INF 1e9

typedef struct {
    int src, dest, weight;
} Edge;

typedef struct {
    int V, E;
    Edge *edges;
} Graph;

Graph* create_graph(int V, int E) {
    Graph *g = malloc(sizeof(Graph));
    g->V = V;
    g->E = E;
    g->edges = malloc(sizeof(Edge) * E);
    return g;
}

void bellman_ford(Graph *g, int src) {
    int V = g->V;
    int E = g->E;
    int dist[V];

    for (int i = 0; i < V; i++) dist[i] = INF;
    dist[src] = 0;

    // Relax all edges V - 1 times
    for (int i = 1; i <= V - 1; i++) {
        for (int j = 0; j < E; j++) {
            int u = g->edges[j].src;
            int v = g->edges[j].dest;
            int weight = g->edges[j].weight;
            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }

    // Check for negative-weight cycles
    for (int i = 0; i < E; i++) {
        int u = g->edges[i].src;
        int v = g->edges[i].dest;
        int weight = g->edges[i].weight;
        if (dist[u] != INF && dist[u] + weight < dist[v]) {
            printf("Graph contains a negative weight cycle!\n");
            return;
        }
    }

    printf("Vertex Distance from Source (%d):\n", src);
    for (int i = 0; i < V; i++) {
        printf("  Node %d : %d\n", i, dist[i]);
    }
}

int main(void) {
    int V = 5, E = 8;
    Graph *g = create_graph(V, E);

    g->edges[0] = (Edge){0, 1, -1};
    g->edges[1] = (Edge){0, 2, 4};
    g->edges[2] = (Edge){1, 2, 3};
    g->edges[3] = (Edge){1, 3, 2};
    g->edges[4] = (Edge){1, 4, 2};
    g->edges[5] = (Edge){3, 2, 5};
    g->edges[6] = (Edge){3, 1, 1};
    g->edges[7] = (Edge){4, 3, -3};

    printf("--- Bellman-Ford Shortest Path Engine ---\n");
    bellman_ford(g, 0);

    free(g->edges);
    free(g);
    return 0;
}
