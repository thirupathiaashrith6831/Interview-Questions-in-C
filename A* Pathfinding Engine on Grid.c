#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define ROWS 5
#define COLS 5

typedef struct {
    int r, c;
    double g, h, f;
    int parent_r, parent_c;
} Node;

double heuristic(int r1, int c1, int r2, int c2) {
    return sqrt((r1 - r2) * (r1 - r2) + (c1 - c2) * (c1 - c2));
}

bool is_valid(int r, int c, int grid[ROWS][COLS]) {
    return (r >= 0 && r < ROWS && c >= 0 && c < COLS && grid[r][c] == 0);
}

void a_star_search(int grid[ROWS][COLS], int src_r, int src_c, int dest_r, int dest_c) {
    Node map[ROWS][COLS];
    bool closed_set[ROWS][COLS] = {false};

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            map[i][j] = (Node){i, j, 1e9, 1e9, 1e9, -1, -1};
        }
    }

    map[src_r][src_c].g = 0;
    map[src_r][src_c].h = heuristic(src_r, src_c, dest_r, dest_c);
    map[src_r][src_c].f = map[src_r][src_c].h;

    while (1) {
        double min_f = 1e9;
        int curr_r = -1, curr_c = -1;

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (!closed_set[i][j] && map[i][j].f < min_f) {
                    min_f = map[i][j].f;
                    curr_r = i;
                    curr_c = j;
                }
            }
        }

        if (curr_r == -1) {
            printf("No path found to destination.\n");
            return;
        }

        if (curr_r == dest_r && curr_c == dest_c) {
            printf("Path found! Target reached with cost %.2f\n", map[curr_r][curr_c].f);
            return;
        }

        closed_set[curr_r][curr_c] = true;

        // Check 4 directional neighbors
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            int nr = curr_r + dr[i];
            int nc = curr_c + dc[i];

            if (is_valid(nr, nc, grid) && !closed_set[nr][nc]) {
                double g_new = map[curr_r][curr_c].g + 1.0;
                double h_new = heuristic(nr, nc, dest_r, dest_c);
                double f_new = g_new + h_new;

                if (f_new < map[nr][nc].f) {
                    map[nr][nc].g = g_new;
                    map[nr][nc].h = h_new;
                    map[nr][nc].f = f_new;
                    map[nr][nc].parent_r = curr_r;
                    map[nr][nc].parent_c = curr_c;
                }
            }
        }
    }
}

int main(void) {
    // 0 = walkable, 1 = obstacle
    int grid[ROWS][COLS] = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {1, 1, 0, 0, 0},
        {0, 0, 0, 1, 0}
    };

    printf("--- A* Heuristic Grid Pathfinding Engine ---\n");
    a_star_search(grid, 0, 0, 4, 4);

    return 0;
}
