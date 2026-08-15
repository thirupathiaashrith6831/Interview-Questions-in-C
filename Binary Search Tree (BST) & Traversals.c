#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int key;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode* bst_create_node(int key) {
    TreeNode *node = malloc(sizeof(TreeNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreeNode* bst_insert(TreeNode *root, int key) {
    if (root == NULL) return bst_create_node(key);

    if (key < root->key) {
        root->left = bst_insert(root->left, key);
    } else if (key > root->key) {
        root->right = bst_insert(root->right, key);
    }
    return root;
}

void bst_inorder(const TreeNode *root) {
    if (root) {
        bst_inorder(root->left);
        printf("%d ", root->key);
        bst_inorder(root->right);
    }
}

void bst_preorder(const TreeNode *root) {
    if (root) {
        printf("%d ", root->key);
        bst_preorder(root->left);
        bst_preorder(root->right);
    }
}

void bst_free(TreeNode *root) {
    if (root) {
        bst_free(root->left);
        bst_free(root->right);
        free(root);
    }
}

int main(void) {
    TreeNode *root = NULL;

    printf("--- Binary Search Tree (BST) Traversals ---\n");
    int keys[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++) {
        root = bst_insert(root, keys[i]);
    }

    printf("In-Order  (Sorted) : ");
    bst_inorder(root);
    printf("\n");

    printf("Pre-Order (Root-First): ");
    bst_preorder(root);
    printf("\n");

    bst_free(root);
    return 0;
}
