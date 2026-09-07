#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct RadixNode {
    char *prefix;
    bool is_end;
    struct RadixNode **children;
    int child_count;
} RadixNode;

RadixNode* create_radix_node(const char *prefix, bool is_end) {
    RadixNode *node = malloc(sizeof(RadixNode));
    node->prefix = strdup(prefix);
    node->is_end = is_end;
    node->children = NULL;
    node->child_count = 0;
    return node;
}

void radix_add_child(RadixNode *parent, RadixNode *child) {
    parent->child_count++;
    parent->children = realloc(parent->children, sizeof(RadixNode*) * parent->child_count);
    parent->children[parent->child_count - 1] = child;
}

// Simple illustrative insertion for common prefix branching
void radix_insert(RadixNode *root, const char *word) {
    for (int i = 0; i < root->child_count; i++) {
        RadixNode *child = root->children[i];
        size_t len = strlen(child->prefix);
        if (strncmp(child->prefix, word, len) == 0) {
            if (strlen(word) == len) {
                child->is_end = true;
            } else {
                radix_insert(child, word + len);
            }
            return;
        }
    }
    // No common prefix child found, attach directly
    radix_add_child(root, create_radix_node(word, true));
}

void radix_print(RadixNode *node, int level) {
    for (int i = 0; i < level; i++) printf("  ");
    printf("-> '%s' %s\n", node->prefix, node->is_end ? "[END]" : "");
    for (int i = 0; i < node->child_count; i++) {
        radix_print(node->children[i], level + 1);
    }
}

void radix_free(RadixNode *node) {
    if (!node) return;
    for (int i = 0; i < node->child_count; i++) {
        radix_free(node->children[i]);
    }
    free(node->children);
    free(node->prefix);
    free(node);
}

int main(void) {
    RadixNode *root = create_radix_node("", false);

    printf("--- Radix Tree (Compressed Trie) ---\n");
    radix_insert(root, "tester");
    radix_insert(root, "testing");
    radix_insert(root, "team");

    radix_print(root, 0);

    radix_free(root);
    return 0;
}
