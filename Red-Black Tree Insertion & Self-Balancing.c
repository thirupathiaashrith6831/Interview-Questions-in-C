#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } NodeColor;

typedef struct RBNode {
    int data;
    NodeColor color;
    struct RBNode *left, *right, *parent;
} RBNode;

typedef struct {
    RBNode *root;
    RBNode *NIL;
} RBTree;

RBNode* create_rb_node(RBTree *tree, int data) {
    RBNode *node = malloc(sizeof(RBNode));
    node->data = data;
    node->color = RED;
    node->left = tree->NIL;
    node->right = tree->NIL;
    node->parent = tree->NIL;
    return node;
}

RBTree* rb_tree_create(void) {
    RBTree *tree = malloc(sizeof(RBTree));
    tree->NIL = malloc(sizeof(RBNode));
    tree->NIL->color = BLACK;
    tree->NIL->left = tree->NIL->right = tree->NIL->parent = NULL;
    tree->root = tree->NIL;
    return tree;
}

void rotate_left(RBTree *tree, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    if (y->left != tree->NIL) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == tree->NIL) tree->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rotate_right(RBTree *tree, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    if (x->right != tree->NIL) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == tree->NIL) tree->root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

void rb_insert_fixup(RBTree *tree, RBNode *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotate_left(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_right(tree, z->parent->parent);
            }
        } else {
            RBNode *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotate_right(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_left(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

void rb_insert(RBTree *tree, int data) {
    RBNode *z = create_rb_node(tree, data);
    RBNode *y = tree->NIL;
    RBNode *x = tree->root;

    while (x != tree->NIL) {
        y = x;
        if (z->data < x->data) x = x->left;
        else x = x->right;
    }
    z->parent = y;
    if (y == tree->NIL) tree->root = z;
    else if (z->data < y->data) y->left = z;
    else y->right = z;

    rb_insert_fixup(tree, z);
}

void rb_inorder(RBTree *tree, RBNode *node) {
    if (node != tree->NIL) {
        rb_inorder(tree, node->left);
        printf("%d(%s) ", node->data, node->color == RED ? "RED" : "BLK");
        rb_inorder(tree, node->right);
    }
}

void rb_free_node(RBTree *tree, RBNode *node) {
    if (node != tree->NIL) {
        rb_free_node(tree, node->left);
        rb_free_node(tree, node->right);
        free(node);
    }
}

void rb_tree_free(RBTree *tree) {
    rb_free_node(tree, tree->root);
    free(tree->NIL);
    free(tree);
}

int main(void) {
    RBTree *tree = rb_tree_create();

    printf("--- Red-Black Tree Self-Balancing Engine ---\n");
    int keys[] = {7, 3, 18, 10, 22, 8, 11, 26, 2, 6};
    for (size_t i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
        rb_insert(tree, keys[i]);
    }

    printf("In-Order Traversal (Value and Color):\n");
    rb_inorder(tree, tree->root);
    printf("\nRoot Element: %d(%s)\n", tree->root->data, tree->root->color == RED ? "RED" : "BLK");

    rb_tree_free(tree);
    return 0;
}
