#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define V 5

int min_distance(const int dist[], const bool visited[]) {
    int min = INT_MAX, min_index = -1;

    for (int v = 0; v < V; v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(int graph[V][V], int src) {
    int dist[V];
    bool visited[V];

    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = min_distance(dist, visited);
        if (u == -1) break;

        visited[u] = true;

        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    printf("Node \t Shortest Distance from Source (%d)\n", src);
    for (int i = 0; i < V; i++) {
        printf("%d \t\t %d\n", i, dist[i]);
    }
}

int main(void) {
    // Graph representation (Adjacency Matrix)
    int graph[V][V] = {
        {0, 4, 0, 0, 2},
        {4, 0, 8, 0, 0},
        {0, 8, 0, 7, 0},
        {0, 0, 7, 0, 3},
        {2, 0, 0, 3, 0}
    };

    printf("--- Running Dijkstra's Shortest Path Algorithm ---\n");
    dijkstra(graph, 0);

    return 0;
}
