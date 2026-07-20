#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

Node* create_node(int value) {
    Node *node = malloc(sizeof(Node));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* insert(Node *root, int value) {
    if (root == NULL) return create_node(value);

    if (value < root->value) {
        root->left = insert(root->left, value);
    } else if (value > root->value) {
        root->right = insert(root->right, value);
    }
    return root;
}

void inorder_print(Node *root) {
    if (root != NULL) {
        inorder_print(root->left);
        printf("%d ", root->value);
        inorder_print(root->right);
    }
}

void free_tree(Node *root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main(void) {
    Node *root = NULL;
    int values[] = {50, 30, 20, 40, 70, 60, 80};

    for (int i = 0; i < 7; i++) {
        root = insert(root, values[i]);
    }

    printf("In-order traversal (sorted output):\n");
    inorder_print(root);
    printf("\n");

    free_tree(root);
    return 0;
}