#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum { RED, BLACK } NodeColor;

typedef struct RBNode {
    int key;
    NodeColor color;
    struct RBNode *left, *right;
} RBNode;

RBNode* create_rb_node(int key, NodeColor color) {
    RBNode *node = malloc(sizeof(RBNode));
    node->key = key;
    node->color = color;
    node->left = node->right = NULL;
    return node;
}

// Helper: Verifies no two consecutive Red nodes exist
bool verify_red_property(RBNode *root) {
    if (root == NULL) return true;

    if (root->color == RED) {
        if ((root->left && root->left->color == RED) ||
            (root->right && root->right->color == RED)) {
            return false;
        }
    }
    return verify_red_property(root->left) && verify_red_property(root->right);
}

// Helper: Verifies that all paths from root to leaf contain equal black nodes
int verify_black_height(RBNode *root, bool *is_valid) {
    if (root == NULL) return 1; // Null leaves count as black nodes

    int left_bh = verify_black_height(root->left, is_valid);
    int right_bh = verify_black_height(root->right, is_valid);

    if (left_bh != right_bh) {
        *is_valid = false;
    }

    return left_bh + (root->color == BLACK ? 1 : 0);
}

bool is_valid_red_black_tree(RBNode *root) {
    if (root == NULL) return true;

    // Property 1: Root must be black
    if (root->color != BLACK) {
        printf("Violation: Root node is not BLACK!\n");
        return false;
    }

    // Property 2: No double RED parent-child relationship
    if (!verify_red_property(root)) {
        printf("Violation: Red node has a Red child!\n");
        return false;
    }

    // Property 3: Uniform Black Height across all leaf paths
    bool height_valid = true;
    verify_black_height(root, &height_valid);
    if (!height_valid) {
        printf("Violation: Path black heights are unbalanced!\n");
        return false;
    }

    return true;
}

void free_rb_tree(RBNode *root) {
    if (!root) return;
    free_rb_tree(root->left);
    free_rb_tree(root->right);
    free(root);
}

int main(void) {
    /* Constructing a valid RB Tree:
              20(B)
             /     \
          10(R)    30(B)
    */
    RBNode *root = create_rb_node(20, BLACK);
    root->left = create_rb_node(10, RED);
    root->right = create_rb_node(30, BLACK);

    printf("--- Red-Black Tree Property Validator ---\n");
    printf("Checking tree validity... ");
    if (is_valid_red_black_tree(root)) {
        printf("VALID Red-Black Tree!\n");
    } else {
        printf("INVALID Red-Black Tree!\n");
    }

    free_rb_tree(root);
    return 0;
}
