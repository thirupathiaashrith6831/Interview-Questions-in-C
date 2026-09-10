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

SkipNode* create_skip_node(int key, int level) {
    SkipNode *node = malloc(sizeof(SkipNode));
    node->key = key;
    node->forward = malloc(sizeof(SkipNode*) * (level + 1));
    for (int i = 0; i <= level; i++) {
        node->forward[i] = NULL;
    }
    return node;
}

SkipList* skiplist_create(void) {
    SkipList *list = malloc(sizeof(SkipList));
    list->level = 0;
    list->header = create_skip_node(-1, MAX_LEVEL);
    return list;
}

int random_level(void) {
    int lvl = 0;
    while ((rand() % 2) && lvl < MAX_LEVEL) {
        lvl++;
    }
    return lvl;
}

void skiplist_insert(SkipList *list, int key) {
    SkipNode *update[MAX_LEVEL + 1];
    SkipNode *curr = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (curr->forward[i] && curr->forward[i]->key < key) {
            curr = curr->forward[i];
        }
        update[i] = curr;
    }

    curr = curr->forward[0];

    if (curr == NULL || curr->key != key) {
        int rlevel = random_level();
        if (rlevel > list->level) {
            for (int i = list->level + 1; i <= rlevel; i++) {
                update[i] = list->header;
            }
            list->level = rlevel;
        }

        SkipNode *new_node = create_skip_node(key, rlevel);
        for (int i = 0; i <= rlevel; i++) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
    }
}

bool skiplist_search(SkipList *list, int key) {
    SkipNode *curr = list->header;
    for (int i = list->level; i >= 0; i--) {
        while (curr->forward[i] && curr->forward[i]->key < key) {
            curr = curr->forward[i];
        }
    }
    curr = curr->forward[0];
    return (curr && curr->key == key);
}

void skiplist_display(SkipList *list) {
    printf("Skip List Structure:\n");
    for (int i = list->level; i >= 0; i--) {
        SkipNode *node = list->header->forward[i];
        printf("Level %d: ", i);
        while (node != NULL) {
            printf("%d -> ", node->key);
            node = node->forward[i];
        }
        printf("NULL\n");
    }
}

void skiplist_free(SkipList *list) {
    SkipNode *curr = list->header->forward[0];
    while (curr) {
        SkipNode *next = curr->forward[0];
        free(curr->forward);
        free(curr);
        curr = next;
    }
    free(list->header->forward);
    free(list->header);
    free(list);
}

int main(void) {
    srand(42); // Fixed seed for reproducible levels
    SkipList *list = skiplist_create();

    printf("--- Skip List Probabilistic Index ---\n");
    int keys[] = {3, 6, 7, 9, 12, 19, 17, 26, 21, 25};
    for (size_t i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
        skiplist_insert(list, keys[i]);
    }

    skiplist_display(list);

    printf("\nSearch Key 19: %s\n", skiplist_search(list, 19) ? "FOUND" : "NOT FOUND");
    printf("Search Key 15: %s\n", skiplist_search(list, 15) ? "FOUND" : "NOT FOUND");

    skiplist_free(list);
    return 0;
}
