#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WHEEL_SIZE 4

typedef struct TaskNode {
    int task_id;
    int delay_ticks;
    struct TaskNode *next;
} TaskNode;

typedef struct {
    TaskNode *slots[WHEEL_SIZE];
    int current_tick;
} TimingWheel;

void wheel_init(TimingWheel *wheel) {
    wheel->current_tick = 0;
    for (int i = 0; i < WHEEL_SIZE; i++) {
        wheel->slots[i] = NULL;
    }
}

void wheel_add_task(TimingWheel *wheel, int task_id, int delay_ticks) {
    int slot = (wheel->current_tick + delay_ticks) % WHEEL_SIZE;
    TaskNode *node = malloc(sizeof(TaskNode));
    node->task_id = task_id;
    node->delay_ticks = delay_ticks;
    node->next = wheel->slots[slot];
    wheel->slots[slot] = node;
}

void wheel_tick(TimingWheel *wheel) {
    int slot = wheel->current_tick % WHEEL_SIZE;
    printf("Tick %d -> Evaluating slot %d:\n", wheel->current_tick, slot);

    TaskNode *curr = wheel->slots[slot];
    while (curr) {
        printf("  Executing Task #%d (delay matched)\n", curr->task_id);
        TaskNode *temp = curr;
        curr = curr->next;
        free(temp);
    }
    wheel->slots[slot] = NULL;
    wheel->current_tick++;
}

int main(void) {
    TimingWheel wheel;
    wheel_init(&wheel);

    printf("--- Timing Wheel Scheduler Simulation ---\n");
    wheel_add_task(&wheel, 101, 1); // Executes at tick 1
    wheel_add_task(&wheel, 102, 3); // Executes at tick 3
    wheel_add_task(&wheel, 103, 5); // Executes at tick 5

    for (int i = 0; i < 6; i++) {
        wheel_tick(&wheel);
    }

    return 0;
}
