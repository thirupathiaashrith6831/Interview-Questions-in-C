#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Interval {
    int low, high;
} Interval;

typedef struct IntervalNode {
    Interval iv;
    int max;
    struct IntervalNode *left, *right;
} IntervalNode;

int max(int a, int b) { return (a > b) ? a : b; }

IntervalNode* create_interval_node(Interval iv) {
    IntervalNode *node = malloc(sizeof(IntervalNode));
    node->iv = iv;
    node->max = iv.high;
    node->left = node->right = NULL;
    return node;
}

IntervalNode* interval_insert(IntervalNode *root, Interval iv) {
    if (root == NULL) return create_interval_node(iv);

    int l = root->iv.low;
    if (iv.low < l) {
        root->left = interval_insert(root->left, iv);
    } else {
        root->right = interval_insert(root->right, iv);
    }

    if (root->max < iv.high) {
        root->max = iv.high;
    }
    return root;
}

bool do_overlap(Interval i1, Interval i2) {
    return (i1.low <= i2.high && i2.low <= i1.high);
}

Interval* interval_search(IntervalNode *root, Interval iv) {
    if (root == NULL) return NULL;

    if (do_overlap(root->iv, iv)) {
        return &root->iv;
    }

    if (root->left != NULL && root->left->max >= iv.low) {
        return interval_search(root->left, iv);
    }

    return interval_search(root->right, iv);
}

void interval_free(IntervalNode *root) {
    if (root) {
        interval_free(root->left);
        interval_free(root->right);
        free(root);
    }
}

int main(void) {
    IntervalNode *root = NULL;

    printf("--- Interval Tree Overlap Engine ---\n");
    Interval intervals[] = {{15, 20}, {10, 30}, {17, 19}, {5, 20}, {12, 15}, {30, 40}};
    for (size_t i = 0; i < sizeof(intervals)/sizeof(intervals[0]); i++) {
        root = interval_insert(root, intervals[i]);
    }

    Interval query = {14, 16};
    Interval *res = interval_search(root, query);
    if (res) {
        printf("Overlap found for query [%d, %d]: matched interval [%d, %d]\n", 
               query.low, query.high, res->low, res->high);
    } else {
        printf("No overlap found.\n");
    }

    interval_free(root);
    return 0;
}
