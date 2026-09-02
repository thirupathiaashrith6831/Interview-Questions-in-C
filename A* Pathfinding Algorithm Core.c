#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define GRID_SIZE 5

typedef struct Node {
    int r, c;
    int g, h, f;
    struct Node *parent;
} Node;

int heuristic(int r1, int c1, int r2, int c2) {
    return abs(r1 - r2) + abs(c1 - c2); // Manhattan distance
}

bool is_valid(int r, int c, int grid[GRID_SIZE][GRID_SIZE]) {
    return (r >= 0 && r < GRID_SIZE && c >= 0 && c < GRID_SIZE && grid[r][c] == 0);
}

void print_grid_with_path(int grid[GRID_SIZE][GRID_SIZE], Node *end_node) {
    char display[GRID_SIZE][GRID_SIZE];
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            display[i][j] = (grid[i][j] == 1) ? 'X' : '.';
        }
    }

    Node *curr = end_node;
    while (curr) {
        display[curr->r][curr->c] = '*';
        curr = curr->parent;
    }

    printf("Grid Path ('*' = Path, 'X' = Obstacle):\n");
    for (int i = 0; i < GRID_SIZE; i++) {
        printf("  ");
        for (int j = 0; j < GRID_SIZE; j++) {
            printf("%c ", display[i][j]);
        }
        printf("\n");
    }
}

void solve_astar(int grid[GRID_SIZE][GRID_SIZE], int start_r, int start_c, int end_r, int end_c) {
    Node nodes[GRID_SIZE][GRID_SIZE];
    bool closed[GRID_SIZE][GRID_SIZE] = {false};

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            nodes[i][j] = (Node){i, j, 1e9, 1e9, 1e9, NULL};
        }
    }

    nodes[start_r][start_c].g = 0;
    nodes[start_r][start_c].h = heuristic(start_r, start_c, end_r, end_c);
    nodes[start_r][start_c].f = nodes[start_r][start_c].h;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (true) {
        // Find open node with lowest f cost
        int best_f = 1e9, cur_r = -1, cur_c = -1;
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                if (!closed[i][j] && nodes[i][j].f < best_f) {
                    best_f = nodes[i][j].f;
                    cur_r = i;
                    cur_c = j;
                }
            }
        }

        if (cur_r == -1) {
            printf("No path found!\n");
            return;
        }

        if (cur_r == end_r && cur_c == end_c) {
            printf("Path successfully found!\n");
            print_grid_with_path(grid, &nodes[end_r][end_c]);
            return;
        }

        closed[cur_r][cur_c] = true;

        for (int i = 0; i < 4; i++) {
            int nr = cur_r + dr[i];
            int nc = cur_c + dc[i];

            if (is_valid(nr, nc, grid) && !closed[nr][nc]) {
                int new_g = nodes[cur_r][cur_c].g + 1;
                if (new_g < nodes[nr][nc].g) {
                    nodes[nr][nc].g = new_g;
                    nodes[nr][nc].h = heuristic(nr, nc, end_r, end_c);
                    nodes[nr][nc].f = new_g + nodes[nr][nc].h;
                    nodes[nr][nc].parent = &nodes[cur_r][cur_c];
                }
            }
        }
    }
}

int main(void) {
    int grid[GRID_SIZE][GRID_SIZE] = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {1, 1, 0, 0, 0},
        {0, 0, 0, 1, 0}
    };

    printf("--- A* Pathfinding Simulation ---\n");
    solve_astar(grid, 0, 0, 4, 4);

    return 0;
}
