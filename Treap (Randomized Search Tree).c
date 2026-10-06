#include <stdio.h>
#include <stdlib.h>

typedef struct TreapNode {
    int key;
    int priority;
    struct TreapNode *left, *right;
} TreapNode;

TreapNode* create_treap_node(int key) {
    TreapNode *node = malloc(sizeof(TreapNode));
    node->key = key;
    node->priority = rand() % 100;
    node->left = node->right = NULL;
    return node;
}

TreapNode* rotate_right(TreapNode *y) {
    TreapNode *x = y->left;
    y->left = x->right;
    x->right = y;
    return x;
}

TreapNode* rotate_left(TreapNode *x) {
    TreapNode *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

TreapNode* treap_insert(TreapNode *root, int key) {
    if (!root) return create_treap_node(key);

    if (key <= root->key) {
        root->left = treap_insert(root->left, key);
        if (root->left->priority > root->priority) {
            root = rotate_right(root);
        }
    } else {
        root->right = treap_insert(root->right, key);
        if (root->right->priority > root->priority) {
            root = rotate_left(root);
        }
    }
    return root;
}

void treap_inorder(TreapNode *root) {
    if (root) {
        treap_inorder(root->left);
        printf("%d(P:%d) ", root->key, root->priority);
        treap_inorder(root->right);
    }
}

void treap_free(TreapNode *root) {
    if (root) {
        treap_free(root->left);
        treap_free(root->right);
        free(root);
    }
}

int main(void) {
    srand(42);
    TreapNode *root = NULL;

    printf("--- Treap (Randomized BST + Heap) ---\n");
    int keys[] = {50, 30, 20, 40, 70, 60, 80};
    for (size_t i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
        root = treap_insert(root, keys[i]);
    }

    printf("In-Order Traversal (Key and Heap Priority):\n");
    treap_inorder(root);
    printf("\nRoot Element: %d (Priority: %d)\n", root->key, root->priority);

    treap_free(root);
    return 0;
}
