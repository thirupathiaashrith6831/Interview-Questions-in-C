#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN_DEGREE 3 // Degree t: Node can hold at most 2t-1 keys (5 keys max)
#define MAX_KEYS (2 * MIN_DEGREE - 1)

typedef struct BTreeNode {
    int keys[MAX_KEYS];
    struct BTreeNode *children[MAX_KEYS + 1];
    int num_keys;
    bool is_leaf;
} BTreeNode;

BTreeNode* create_node(bool is_leaf) {
    BTreeNode *node = malloc(sizeof(BTreeNode));
    node->num_keys = 0;
    node->is_leaf = is_leaf;
    for (int i = 0; i <= MAX_KEYS; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// Splits full child `y` of node `parent` at index `idx`
void split_child(BTreeNode *parent, int idx, BTreeNode *y) {
    BTreeNode *z = create_node(y->is_leaf);
    z->num_keys = MIN_DEGREE - 1;

    // Copy second half of keys from y to z
    for (int j = 0; j < MIN_DEGREE - 1; j++) {
        z->keys[j] = y->keys[j + MIN_DEGREE];
    }

    // Copy corresponding child pointers if not a leaf
    if (!y->is_leaf) {
        for (int j = 0; j < MIN_DEGREE; j++) {
            z->children[j] = y->children[j + MIN_DEGREE];
        }
    }

    y->num_keys = MIN_DEGREE - 1;

    // Shift children of parent to make room for z
    for (int j = parent->num_keys; j >= idx + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[idx + 1] = z;

    // Shift keys of parent to insert median key of y
    for (int j = parent->num_keys - 1; j >= idx; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[idx] = y->keys[MIN_DEGREE - 1];
    parent->num_keys++;
}

void print_node(const char *label, BTreeNode *node) {
    printf("%s: [ ", label);
    for (int i = 0; i < node->num_keys; i++) {
        printf("%d ", node->keys[i]);
    }
    printf("]\n");
}

int main(void) {
    printf("--- B-Tree Node Splitting Simulator ---\n");

    BTreeNode *parent = create_node(false);
    BTreeNode *child = create_node(true);

    // Fill child node to full capacity (5 keys for t=3)
    int initial_keys[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < MAX_KEYS; i++) {
        child->keys[i] = initial_keys[i];
    }
    child->num_keys = MAX_KEYS;
    parent->children[0] = child;

    print_node("Full Child Before Split", child);

    split_child(parent, 0, child);

    print_node("Parent After Split    ", parent);
    print_node("Left Child (y)        ", child);
    print_node("Right Child (z)       ", parent->children[1]);

    free(parent->children[1]);
    free(child);
    free(parent);

    return 0;
}
