#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define K 2

typedef struct KDNode {
    int point[K];
    struct KDNode *left, *right;
} KDNode;

KDNode* create_kd_node(int point[]) {
    KDNode *node = malloc(sizeof(KDNode));
    for (int i = 0; i < K; i++) node->point[i] = point[i];
    node->left = node->right = NULL;
    return node;
}

KDNode* kd_insert_rec(KDNode *root, int point[], unsigned depth) {
    if (root == NULL) return create_kd_node(point);

    unsigned cd = depth % K;

    if (point[cd] < root->point[cd]) {
        root->left = kd_insert_rec(root->left, point, depth + 1);
    } else {
        root->right = kd_insert_rec(root->right, point, depth + 1);
    }

    return root;
}

KDNode* kd_insert(KDNode *root, int point[]) {
    return kd_insert_rec(root, point, 0);
}

bool are_points_same(int p1[], int p2[]) {
    for (int i = 0; i < K; i++)
        if (p1[i] != p2[i]) return false;
    return true;
}

bool kd_search_rec(KDNode *root, int point[], unsigned depth) {
    if (root == NULL) return false;
    if (are_points_same(root->point, point)) return true;

    unsigned cd = depth % K;

    if (point[cd] < root->point[cd]) {
        return kd_search_rec(root->left, point, depth + 1);
    }
    return kd_search_rec(root->right, point, depth + 1);
}

bool kd_search(KDNode *root, int point[]) {
    return kd_search_rec(root, point, 0);
}

void kd_free(KDNode *root) {
    if (root) {
        kd_free(root->left);
        kd_free(root->right);
        free(root);
    }
}

int main(void) {
    KDNode *root = NULL;

    printf("--- 2D Spatial K-D Tree Engine ---\n");
    int points[][2] = {{3, 6}, {17, 15}, {13, 15}, {6, 12}, {9, 1}, {2, 7}, {10, 19}};

    for (size_t i = 0; i < sizeof(points)/sizeof(points[0]); i++) {
        root = kd_insert(root, points[i]);
    }

    int p1[] = {10, 19};
    int p2[] = {12, 19};

    printf("Search Point (10, 19): %s\n", kd_search(root, p1) ? "FOUND" : "NOT FOUND");
    printf("Search Point (12, 19): %s\n", kd_search(root, p2) ? "FOUND" : "NOT FOUND");

    kd_free(root);
    return 0;
}
