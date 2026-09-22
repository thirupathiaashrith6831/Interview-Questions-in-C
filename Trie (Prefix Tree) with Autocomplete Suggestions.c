#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool is_end_of_word;
} TrieNode;

TrieNode* create_trie_node(void) {
    TrieNode *node = malloc(sizeof(TrieNode));
    node->is_end_of_word = false;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void trie_insert(TrieNode *root, const char *key) {
    TrieNode *curr = root;
    for (int i = 0; key[i] != '\0'; i++) {
        int index = key[i] - 'a';
        if (!curr->children[index]) {
            curr->children[index] = create_trie_node();
        }
        curr = curr->children[index];
    }
    curr->is_end_of_word = true;
}

void autocomplete_dfs(TrieNode* node, char* prefix, int length) {
    if (node->is_end_of_word) {
        prefix[length] = '\0';
        printf("  - %s\n", prefix);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            prefix[length] = 'a' + i;
            autocomplete_dfs(node->children[i], prefix, length + 1);
        }
    }
}

void trie_autocomplete(TrieNode *root, const char *prefix) {
    TrieNode *curr = root;
    char buffer[100];
    int len = 0;

    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = prefix[i] - 'a';
        if (!curr->children[index]) {
            printf("No suggestions found for prefix '%s'\n", prefix);
            return;
        }
        buffer[len++] = prefix[i];
        curr = curr->children[index];
    }

    printf("Suggestions for prefix '%s':\n", prefix);
    autocomplete_dfs(curr, buffer, len);
}

void trie_free(TrieNode *node) {
    if (!node) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            trie_free(node->children[i]);
        }
    }
    free(node);
}

int main(void) {
    TrieNode *root = create_trie_node();

    printf("--- Trie Autocomplete Search Engine ---\n");
    trie_insert(root, "app");
    trie_insert(root, "apple");
    trie_insert(root, "application");
    trie_insert(root, "aptitude");
    trie_insert(root, "bat");
    trie_insert(root, "batch");

    trie_autocomplete(root, "app");
    trie_autocomplete(root, "bat");

    trie_free(root);
    return 0;
}
