#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>

#define MAX_LEVEL 4

typedef struct SkipNode {
    int key;
    int value;
    int level;
    _Atomic(struct SkipNode*) forward[MAX_LEVEL];
} SkipNode;

typedef struct {
    SkipNode *header;
    int level;
} LockFreeSkipList;

SkipNode* create_skip_node(int key, int value, int level) {
    SkipNode *node = malloc(sizeof(SkipNode));
    node->key = key;
    node->value = value;
    node->level = level;
    for (int i = 0; i < level; i++) {
        atomic_init(&node->forward[i], NULL);
    }
    return node;
}

LockFreeSkipList* skip_list_create(void) {
    LockFreeSkipList *list = malloc(sizeof(LockFreeSkipList));
    list->level = 1;
    list->header = create_skip_node(-1, -1, MAX_LEVEL);
    return list;
}

int random_level(void) {
    int lvl = 1;
    while ((rand() % 2) && lvl < MAX_LEVEL) {
        lvl++;
    }
    return lvl;
}

void skip_list_insert(LockFreeSkipList *list, int key, int value) {
    int lvl = random_level();
    SkipNode *new_node = create_skip_node(key, value, lvl);
    SkipNode *update[MAX_LEVEL];
    SkipNode *curr = list->header;

    for (int i = list->level - 1; i >= 0; i--) {
        SkipNode *next = atomic_load_explicit(&curr->forward[i], memory_order_relaxed);
        while (next != NULL && next->key < key) {
            curr = next;
            next = atomic_load_explicit(&curr->forward[i], memory_order_relaxed);
        }
        update[i] = curr;
    }

    if (lvl > list->level) {
        for (int i = list->level; i < lvl; i++) {
            update[i] = list->header;
        }
        list->level = lvl;
    }

    for (int i = 0; i < lvl; i++) {
        SkipNode *nxt = atomic_load_explicit(&update[i]->forward[i], memory_order_relaxed);
        atomic_store_explicit(&new_node->forward[i], nxt, memory_order_relaxed);
        atomic_store_explicit(&update[i]->forward[i], new_node, memory_order_release);
    }
}

bool skip_list_search(LockFreeSkipList *list, int key, int *val) {
    SkipNode *curr = list->header;
    for (int i = list->level - 1; i >= 0; i--) {
        SkipNode *next = atomic_load_explicit(&curr->forward[i], memory_order_acquire);
        while (next != NULL && next->key < key) {
            curr = next;
            next = atomic_load_explicit(&curr->forward[i], memory_order_acquire);
        }
    }
    SkipNode *target = atomic_load_explicit(&curr->forward[0], memory_order_acquire);
    if (target != NULL && target->key == key) {
        *val = target->value;
        return true;
    }
    return false;
}

int main(void) {
    srand(1337);
    LockFreeSkipList *list = skip_list_create();

    printf("--- Lock-Free Skip List Engine ---\n");
    skip_list_insert(list, 10, 100);
    skip_list_insert(list, 25, 250);
    skip_list_insert(list, 5, 50);

    int val;
    if (skip_list_search(list, 25, &val)) {
        printf("Found key 25 with value: %d\n", val);
    }
    if (!skip_list_search(list, 99, &val)) {
        printf("Key 99 correctly reported as NOT FOUND.\n");
    }

    return 0;
}
