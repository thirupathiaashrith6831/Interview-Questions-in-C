#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool is_end_of_word;
} TrieNode;

TrieNode* trie_create_node(void) {
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
            curr->children[index] = trie_create_node();
        }
        curr = curr->children[index];
    }
    curr->is_end_of_word = true;
}

bool trie_search(TrieNode *root, const char *key) {
    TrieNode *curr = root;
    for (int i = 0; key[i] != '\0'; i++) {
        int index = key[i] - 'a';
        if (!curr->children[index]) return false;
        curr = curr->children[index];
    }
    return (curr != NULL && curr->is_end_of_word);
}

void trie_free(TrieNode *root) {
    if (!root) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]) {
            trie_free(root->children[i]);
        }
    }
    free(root);
}

int main(void) {
    TrieNode *root = trie_create_node();

    printf("--- Trie (Prefix Tree) Search ---\n");
    trie_insert(root, "code");
    trie_insert(root, "coder");
    trie_insert(root, "coding");

    printf("Search 'code'  : %s\n", trie_search(root, "code") ? "FOUND" : "NOT FOUND");
    printf("Search 'cod'   : %s\n", trie_search(root, "cod") ? "FOUND" : "NOT FOUND");
    printf("Search 'coder' : %s\n", trie_search(root, "coder") ? "FOUND" : "NOT FOUND");

    trie_free(root);
    return 0;
}
