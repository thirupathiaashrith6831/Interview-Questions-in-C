#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN_DEGREE 3 // Degree t: Max keys = 2t-1 (5), Min keys = t-1 (2)
#define MAX_KEYS (2 * MIN_DEGREE - 1)

typedef struct BTreeNode {
    int keys[MAX_KEYS];
    struct BTreeNode *children[MAX_KEYS + 1];
    int num_keys;
    bool is_leaf;
} BTreeNode;

typedef struct {
    BTreeNode *root;
} BTree;

BTreeNode* btree_create_node(bool is_leaf) {
    BTreeNode *node = malloc(sizeof(BTreeNode));
    node->num_keys = 0;
    node->is_leaf = is_leaf;
    for (int i = 0; i <= MAX_KEYS; i++) {
        node->children[i] = NULL;
    }
    return node;
}

BTree* btree_create(void) {
    BTree *tree = malloc(sizeof(BTree));
    tree->root = btree_create_node(true);
    return tree;
}

void btree_split_child(BTreeNode *parent, int idx, BTreeNode *y) {
    BTreeNode *z = btree_create_node(y->is_leaf);
    z->num_keys = MIN_DEGREE - 1;

    for (int j = 0; j < MIN_DEGREE - 1; j++) {
        z->keys[j] = y->keys[j + MIN_DEGREE];
    }

    if (!y->is_leaf) {
        for (int j = 0; j < MIN_DEGREE; j++) {
            z->children[j] = y->children[j + MIN_DEGREE];
        }
    }

    y->num_keys = MIN_DEGREE - 1;

    for (int j = parent->num_keys; j >= idx + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[idx + 1] = z;

    for (int j = parent->num_keys - 1; j >= idx; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[idx] = y->keys[MIN_DEGREE - 1];
    parent->num_keys++;
}

void btree_insert_non_full(BTreeNode *node, int key) {
    int i = node->num_keys - 1;

    if (node->is_leaf) {
        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        while (i >= 0 && node->keys[i] > key) {
            i--;
        }
        i++;
        if (node->children[i]->num_keys == MAX_KEYS) {
            btree_split_child(node, i, node->children[i]);
            if (node->keys[i] < key) {
                i++;
            }
        }
        btree_insert_non_full(node->children[i], key);
    }
}

void btree_insert(BTree *tree, int key) {
    BTreeNode *root = tree->root;
    if (root->num_keys == MAX_KEYS) {
        BTreeNode *s = btree_create_node(false);
        tree->root = s;
        s->children[0] = root;
        btree_split_child(s, 0, root);
        btree_insert_non_full(s, key);
    } else {
        btree_insert_non_full(root, key);
    }
}

void btree_traverse(BTreeNode *node) {
    int i;
    for (i = 0; i < node->num_keys; i++) {
        if (!node->is_leaf) {
            btree_traverse(node->children[i]);
        }
        printf("%d ", node->keys[i]);
    }
    if (!node->is_leaf) {
        btree_traverse(node->children[i]);
    }
}

void btree_free_node(BTreeNode *node) {
    if (!node) return;
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            btree_free_node(node->children[i]);
        }
    }
    free(node);
}

int main(void) {
    BTree *tree = btree_create();

    printf("--- B-Tree Complete Insertion Engine ---\n");
    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    for (size_t i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
        btree_insert(tree, keys[i]);
    }

    printf("Sorted Keys (In-Order Traversal): ");
    btree_traverse(tree->root);
    printf("\nRoot Num Keys: %d\n", tree->root->num_keys);

    btree_free_node(tree->root);
    free(tree);
    return 0;
}
