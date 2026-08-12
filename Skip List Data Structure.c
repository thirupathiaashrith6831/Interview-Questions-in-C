#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LEVEL 4

typedef struct SkipNode {
    int key;
    struct SkipNode **forward;
} SkipNode;

typedef struct {
    int level;
    SkipNode *header;
} SkipList;

SkipNode* create_node(int key, int level) {
    SkipNode *node = malloc(sizeof(SkipNode));
    node->key = key;
    node->forward = malloc(sizeof(SkipNode*) * (level + 1));
    for (int i = 0; i <= level; i++) node->forward[i] = NULL;
    return node;
}

SkipList* skiplist_create(void) {
    SkipList *list = malloc(sizeof(SkipList));
    list->level = 0;
    list->header = create_node(-1, MAX_LEVEL);
    return list;
}

int random_level(void) {
    int lvl = 0;
    while ((rand() % 2) && lvl < MAX_LEVEL) lvl++;
    return lvl;
}

void skiplist_insert(SkipList *list, int key) {
    SkipNode *update[MAX_LEVEL + 1];
    SkipNode *current = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    int rlevel = random_level();
    if (rlevel > list->level) {
        for (int i = list->level + 1; i <= rlevel; i++) {
            update[i] = list->header;
        }
        list->level = rlevel;
    }

    SkipNode *new_node = create_node(key, rlevel);
    for (int i = 0; i <= rlevel; i++) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }
}

void skiplist_display(SkipList *list) {
    printf("--- Skip List Multi-Level View ---\n");
    for (int i = list->level; i >= 0; i--) {
        SkipNode *node = list->header->forward[i];
        printf("Level %d: ", i);
        while (node) {
            printf("%d -> ", node->key);
            node = node->forward[i];
        }
        printf("NULL\n");
    }
}

int main(void) {
    srand(42); // Fixed seed for reproducible levels
    SkipList *list = skiplist_create();

    int keys[] = {3, 6, 7, 9, 12, 19, 17, 26, 21, 25};
    for (int i = 0; i < 10; i++) {
        skiplist_insert(list, keys[i]);
    }

    skiplist_display(list);
    return 0;
}
