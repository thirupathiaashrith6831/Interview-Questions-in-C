#include <stdio.h>
#include <stdlib.h>

#define ROWS 4
#define COLS 8

typedef struct {
    char symbol;
    int count;
} RLEPair;

void decode_grid(const RLEPair *pairs, int num_pairs, char grid[ROWS][COLS]) {
    int total_cells = ROWS * COLS;
    int current_idx = 0;

    for (int i = 0; i < num_pairs; i++) {
        for (int j = 0; j < pairs[i].count; j++) {
            if (current_idx >= total_cells) break;
            
            int r = current_idx / COLS;
            int c = current_idx % COLS;
            grid[r][c] = pairs[i].symbol;
            current_idx++;
        }
    }
}

void print_grid(char grid[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        printf("  [ ");
        for (int j = 0; j < COLS; j++) {
            printf("%c ", grid[i][j]);
        }
        printf("]\n");
    }
}

int main(void) {
    // Encoded ASCII grid sequence (32 cells total)
    RLEPair compressed_image[] = {
        {'#', 8},  // Row 0
        {'.', 3}, {'#', 2}, {'.', 3}, // Row 1
        {'.', 3}, {'#', 2}, {'.', 3}, // Row 2
        {'#', 8}   // Row 3
    };
    int pair_count = sizeof(compressed_image) / sizeof(compressed_image[0]);

    char image_grid[ROWS][COLS];
    decode_grid(compressed_image, pair_count, image_grid);

    printf("--- Decoded 2D ASCII Matrix (%dx%d) ---\n", ROWS, COLS);
    print_grid(image_grid);

    return 0;
}
