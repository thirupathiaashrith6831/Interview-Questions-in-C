#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define THREAD_COUNT 4
#define QUEUE_SIZE 10

typedef struct {
    void (*function)(void *arg);
    void *arg;
} Task;

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t threads[THREAD_COUNT];
    Task queue[QUEUE_SIZE];
    int head;
    int tail;
    int count;
    int shutdown;
} ThreadPool;

void* worker_thread(void *arg) {
    ThreadPool *pool = (ThreadPool*)arg;

    while (1) {
        pthread_mutex_lock(&pool->lock);

        while (pool->count == 0 && !pool->shutdown) {
            pthread_cond_wait(&pool->notify, &pool->lock);
        }

        if (pool->shutdown && pool->count == 0) {
            pthread_mutex_unlock(&pool->lock);
            pthread_exit(NULL);
        }

        Task task = pool->queue[pool->head];
        pool->head = (pool->head + 1) % QUEUE_SIZE;
        pool->count--;

        pthread_mutex_unlock(&pool->lock);

        // Execute job
        (*(task.function))(task.arg);
    }
    return NULL;
}

void threadpool_init(ThreadPool *pool) {
    pool->head = pool->tail = pool->count = pool->shutdown = 0;
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->notify, NULL);

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&pool->threads[i], NULL, worker_thread, pool);
    }
}

void threadpool_add_task(ThreadPool *pool, void (*function)(void *), void *arg) {
    pthread_mutex_lock(&pool->lock);

    if (pool->count < QUEUE_SIZE) {
        pool->queue[pool->tail].function = function;
        pool->queue[pool->tail].arg = arg;
        pool->tail = (pool->tail + 1) % QUEUE_SIZE;
        pool->count++;
        pthread_cond_signal(&pool->notify);
    }

    pthread_mutex_unlock(&pool->lock);
}

void threadpool_destroy(ThreadPool *pool) {
    pthread_mutex_lock(&pool->lock);
    pool->shutdown = 1;
    pthread_cond_broadcast(&pool->notify);
    pthread_mutex_unlock(&pool->lock);

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->notify);
}

void sample_task(void *arg) {
    int num = *(int*)arg;
    printf("  [Thread %ld] Executed Task #%d\n", (long)pthread_self(), num);
}

int main(void) {
    ThreadPool pool;
    threadpool_init(&pool);

    printf("--- Thread Pool Dispatcher Simulation ---\n");
    int task_args[6];
    for (int i = 0; i < 6; i++) {
        task_args[i] = i + 1;
        threadpool_add_task(&pool, sample_task, &task_args[i]);
    }

    usleep(100000); // Allow tasks time to finish execution
    threadpool_destroy(&pool);
    printf("Thread pool successfully shut down.\n");

    return 0;
}
