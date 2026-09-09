#include <stdio.h>
#include <stdlib.h>

typedef struct AVLNode {
    int key;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

int max(int a, int b) { return (a > b) ? a : b; }

int height(AVLNode *n) {
    if (n == NULL) return 0;
    return n->height;
}

AVLNode* create_node(int key) {
    AVLNode *node = malloc(sizeof(AVLNode));
    node->key = key;
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

int get_balance(AVLNode *n) {
    if (n == NULL) return 0;
    return height(n->left) - height(n->right);
}

AVLNode* rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode* rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

AVLNode* avl_insert(AVLNode *node, int key) {
    if (node == NULL) return create_node(key);

    if (key < node->key)
        node->left = avl_insert(node->left, key);
    else if (key > node->key)
        node->right = avl_insert(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = get_balance(node);

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rotate_right(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return rotate_left(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

void avl_inorder(AVLNode *root) {
    if (root) {
        avl_inorder(root->left);
        printf("%d(H:%d) ", root->key, root->height);
        avl_inorder(root->right);
    }
}

void avl_free(AVLNode *root) {
    if (root) {
        avl_free(root->left);
        avl_free(root->right);
        free(root);
    }
}

int main(void) {
    AVLNode *root = NULL;

    printf("--- AVL Self-Balancing Tree ---\n");
    // Sequential insertions trigger rebalancing rotations
    int keys[] = {10, 20, 30, 40, 50, 25};
    for (int i = 0; i < 6; i++) {
        root = avl_insert(root, keys[i]);
    }

    printf("In-Order Traversal with Heights:\n");
    avl_inorder(root);
    printf("\nRoot Key: %d\n", root->key);

    avl_free(root);
    return 0;
}
