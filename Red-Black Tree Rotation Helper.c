#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int data;
    Color color;
    struct Node *left, *right, *parent;
} Node;

Node* create_node(int data) {
    Node *n = malloc(sizeof(Node));
    n->data = data;
    n->color = RED;
    n->left = n->right = n->parent = NULL;
    return n;
}

void rotate_left(Node **root, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void print_tree(Node *node, int level) {
    if (node == NULL) return;
    print_tree(node->right, level + 1);
    for (int i = 0; i < level; i++) printf("    ");
    printf("%d(%s)\n", node->data, node->color == RED ? "R" : "B");
    print_tree(node->left, level + 1);
}

int main(void) {
    Node *root = create_node(10);
    root->color = BLACK;
    root->right = create_node(20);
    root->right->parent = root;
    root->right->right = create_node(30);
    root->right->right->parent = root->right;

    printf("--- Original Tree Structure ---\n");
    print_tree(root, 0);

    printf("\n--- Performing Left Rotation at Node 10 ---\n");
    rotate_left(&root, root);
    print_tree(root, 0);

    return 0;
}
