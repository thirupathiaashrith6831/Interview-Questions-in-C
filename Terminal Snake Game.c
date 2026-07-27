#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define WIDTH 20
#define HEIGHT 10

typedef struct { int x, y; } Point;

Point snake[100];
int snake_len = 3;
Point food = {10, 5};
int dirX = 1, dirY = 0;

void set_nonblocking(int enable) {
    struct termios tt;
    tcgetattr(STDIN_FILENO, &tt);
    if (enable) {
        tt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &tt);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    } else {
        tt.c_lflag |= (ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &tt);
    }
}

void draw(void) {
    printf("\033[H"); // Move cursor to top-left
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) printf("#");
            else if (x == snake[0].x && y == snake[0].y) printf("O"); // Head
            else if (x == food.x && y == food.y) printf("*");          // Food
            else {
                int is_body = 0;
                for (int i = 1; i < snake_len; i++) {
                    if (snake[i].x == x && snake[i].y == y) is_body = 1;
                }
                printf(is_body ? "o" : " ");
            }
        }
        printf("\n");
    }
}

int main(void) {
    set_nonblocking(1);
    printf("\033[2J"); // Clear screen

    snake[0] = (Point){5, 5};
    snake[1] = (Point){4, 5};
    snake[2] = (Point){3, 5};

    for (int step = 0; step < 30; step++) { // Run for 30 ticks
        char key = 0;
        if (read(STDIN_FILENO, &key, 1) > 0) {
            if (key == 'w') { dirX = 0; dirY = -1; }
            if (key == 's') { dirX = 0; dirY = 1; }
            if (key == 'a') { dirX = -1; dirY = 0; }
            if (key == 'd') { dirX = 1; dirY = 0; }
        }

        // Update body
        for (int i = snake_len - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }
        snake[0].x += dirX;
        snake[0].y += dirY;

        draw();
        usleep(150000); // Frame delay
    }

    set_nonblocking(0);
    printf("\nGame finished.\n");
    return 0;
}