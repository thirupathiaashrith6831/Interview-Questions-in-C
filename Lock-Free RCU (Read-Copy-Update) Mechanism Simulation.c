#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>

typedef struct {
    int version;
    int data;
} Config;

_Atomic(Config*) current_config = NULL;

void rcu_init(void) {
    Config *initial = malloc(sizeof(Config));
    initial->version = 1;
    initial->data = 42;
    atomic_init(&current_config, initial);
}

int rcu_read(void) {
    Config *cfg = atomic_load_explicit(&current_config, memory_order_consume);
    return cfg->data;
}

void rcu_update(int new_data) {
    Config *old_cfg = atomic_load_explicit(&current_config, memory_order_relaxed);
    Config *new_cfg = malloc(sizeof(Config));
    new_cfg->version = old_cfg->version + 1;
    new_cfg->data = new_data;

    atomic_store_explicit(&current_config, new_cfg, memory_order_release);
    
    printf("[RCU Writer] Updated config version to %d with data %d. Freeing old version %d.\n", 
           new_cfg->version, new_cfg->data, old_cfg->version);
    free(old_cfg);
}

int main(void) {
    rcu_init();

    printf("--- Lock-Free RCU (Read-Copy-Update) Simulation ---\n");
    printf("Reader read data: %d\n", rcu_read());

    rcu_update(100);

    printf("Reader read updated data: %d\n", rcu_read());

    free(atomic_load(&current_config));
    return 0;
}
