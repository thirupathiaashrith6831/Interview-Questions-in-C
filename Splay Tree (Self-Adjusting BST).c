#include <stdio.h>
#include <stdlib.h>

typedef struct SplayNode {
    int key;
    struct SplayNode *left, *right;
} SplayNode;

SplayNode* create_splay_node(int key) {
    SplayNode *node = malloc(sizeof(SplayNode));
    node->key = key;
    node->left = node->right = NULL;
    return node;
}

SplayNode* splay_rotate_right(SplayNode *x) {
    SplayNode *y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

SplayNode* splay_rotate_left(SplayNode *x) {
    SplayNode *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// Bring key to root if present, or last accessed node
SplayNode* splay(SplayNode *root, int key) {
    if (root == NULL || root->key == key) return root;

    // Key lies in left subtree
    if (key < root->key) {
        if (root->left == NULL) return root;

        // Zig-Zig (Left Left)
        if (key < root->left->key) {
            root->left->left = splay(root->left->left, key);
            root = splay_rotate_right(root);
        }
        // Zig-Zag (Left Right)
        else if (key > root->left->key) {
            root->left->right = splay(root->left->right, key);
            if (root->left->right != NULL)
                root->left = splay_rotate_left(root->left);
        }
        return (root->left == NULL) ? root : splay_rotate_right(root);
    } 
    // Key lies in right subtree
    else {
        if (root->right == NULL) return root;

        // Zag-Zig (Right Left)
        if (key < root->right->key) {
            root->right->left = splay(root->right->left, key);
            if (root->right->left != NULL)
                root->right = splay_rotate_right(root->right);
        }
        // Zag-Zag (Right Right)
        else if (key > root->right->key) {
            root->right->right = splay(root->right->right, key);
            root = splay_rotate_left(root);
        }
        return (root->right == NULL) ? root : splay_rotate_left(root);
    }
}

SplayNode* splay_insert(SplayNode *root, int key) {
    if (root == NULL) return create_splay_node(key);

    root = splay(root, key);

    if (root->key == key) return root;

    SplayNode *new_node = create_splay_node(key);
    if (key < root->key) {
        new_node->right = root;
        new_node->left = root->left;
        root->left = NULL;
    } else {
        new_node->left = root;
        new_node->right = root->right;
        root->right = NULL;
    }
    return new_node;
}

void splay_free(SplayNode *root) {
    if (root) {
        splay_free(root->left);
        splay_free(root->right);
        free(root);
    }
}

int main(void) {
    SplayNode *root = NULL;

    printf("--- Splay Tree Self-Adjusting Engine ---\n");
    int keys[] = {100, 50, 200, 40, 30, 20, 10};
    for (size_t i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
        root = splay_insert(root, keys[i]);
    }

    printf("Root after inserting all keys: %d\n", root->key);

    // Accessing an element brings it to the top
    printf("Searching for key 30...\n");
    root = splay(root, 30);
    printf("New Root Key after search(30): %d\n", root->key);

    splay_free(root);
    return 0;
}
