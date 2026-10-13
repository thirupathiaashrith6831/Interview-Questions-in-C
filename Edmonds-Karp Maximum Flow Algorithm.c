#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define V 6
#define INF 1e9

int min(int a, int b) { return (a < b) ? a : b; }

bool bfs(int r_graph[V][V], int s, int t, int parent[]) {
    bool visited[V];
    memset(visited, 0, sizeof(visited));

    int queue[V];
    int head = 0, tail = 0;

    queue[tail++] = s;
    visited[s] = true;
    parent[s] = -1;

    while (head < tail) {
        int u = queue[head++];

        for (int v = 0; v < V; v++) {
            if (!visited[v] && r_graph[u][v] > 0) {
                queue[tail++] = v;
                parent[v] = u;
                visited[v] = true;
                if (v == t) return true;
            }
        }
    }
    return false;
}

int edmonds_karp(int graph[V][V], int s, int t) {
    int u, v;
    int r_graph[V][V];

    for (u = 0; u < V; u++)
        for (v = 0; v < V; v++)
            r_graph[u][v] = graph[u][v];

    int parent[V];
    int max_flow = 0;

    while (bfs(r_graph, s, t, parent)) {
        int path_flow = INF;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = min(path_flow, r_graph[u][v]);
        }

        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            r_graph[u][v] -= path_flow;
            r_graph[v][u] += path_flow;
        }

        max_flow += path_flow;
    }

    return max_flow;
}

int main(void) {
    // Residual graph capacity matrix
    int graph[V][V] = {
        {0, 16, 13, 0, 0, 0},
        {0, 0, 10, 12, 0, 0},
        {0, 4, 0, 0, 14, 0},
        {0, 0, 9, 0, 0, 20},
        {0, 0, 0, 7, 0, 4},
        {0, 0, 0, 0, 0, 0}
    };

    printf("--- Edmonds-Karp Maximum Flow Engine ---\n");
    printf("The maximum possible flow is: %d\n", edmonds_karp(graph, 0, 5));

    return 0;
}
