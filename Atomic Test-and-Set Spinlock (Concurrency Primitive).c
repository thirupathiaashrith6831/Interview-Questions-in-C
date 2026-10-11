#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    atomic_flag flag;
} SpinLock;

void spinlock_init(SpinLock *lock) {
    atomic_flag_clear(&lock->flag);
}

void spinlock_lock(SpinLock *lock) {
    while (atomic_flag_test_and_set_explicit(&lock->flag, memory_order_acquire)) {
        // Busy-wait (spin)
    }
}

void spinlock_unlock(SpinLock *lock) {
    atomic_flag_clear_explicit(&lock->flag, memory_order_release);
}

SpinLock global_lock;
int shared_counter = 0;

void* worker(void *arg) {
    for (int i = 0; i < 1000; i++) {
        spinlock_lock(&global_lock);
        shared_counter++;
        spinlock_unlock(&global_lock);
    }
    return NULL;
}

int main(void) {
    spinlock_init(&global_lock);
    pthread_t t1, t2;

    printf("--- Atomic Test-and-Set Spinlock ---\n");
    pthread_create(&t1, NULL, worker, NULL);
    pthread_create(&t2, NULL, worker, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Expected Counter: 2000\n");
    printf("Actual Counter  : %d\n", shared_counter);

    return 0;
}
