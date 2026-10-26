#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET 26
#define MAX_NODES 100

typedef struct ACNode {
    int children[ALPHABET];
    int failure_link;
    bool is_end;
    char word[32];
} ACNode;

ACNode pool[MAX_NODES];
int node_count = 0;

int create_ac_node(void) {
    if (node_count >= MAX_NODES) return 0;
    int idx = node_count++;
    for (int i = 0; i < ALPHABET; i++) pool[idx].children[i] = -1;
    pool[idx].failure_link = 0;
    pool[idx].is_end = false;
    return idx;
}

void ac_insert(const char *word) {
    int curr = 0;
    for (int i = 0; word[i] != '\0'; i++) {
        int c = word[i] - 'a';
        if (pool[curr].children[c] == -1) {
            pool[curr].children[c] = create_ac_node();
        }
        curr = pool[curr].children[c];
    }
    pool[curr].is_end = true;
    strcpy(pool[curr].word, word);
}

void ac_build_failure_links(void) {
    int queue[MAX_NODES];
    int head = 0, tail = 0;

    for (int i = 0; i < ALPHABET; i++) {
        if (pool[0].children[i] != -1) {
            pool[pool[0].children[i]].failure_link = 0;
            queue[tail++] = pool[0].children[i];
        } else {
            pool[0].children[i] = 0;
        }
    }

    while (head < tail) {
        int curr = queue[head++];
        for (int i = 0; i < ALPHABET; i++) {
            int child = pool[curr].children[i];
            if (child != -1) {
                pool[child].failure_link = pool[pool[curr].failure_link].children[i];
                if (pool[pool[child].failure_link].is_end) {
                    // Inherit matches if needed
                }
                queue[tail++] = child;
            } else {
                pool[curr].children[i] = pool[pool[curr].failure_link].children[i];
            }
        }
    }
}

void ac_search(const char *text) {
    int curr = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        int c = text[i] - 'a';
        curr = pool[curr].children[c];
        int temp = curr;
        while (temp > 0) {
            if (pool[temp].is_end) {
                printf("Pattern found: '%s' ending at index %d\n", pool[temp].word, i);
            }
            temp = pool[temp].failure_link;
        }
    }
}

int main(void) {
    create_ac_node(); // Root node 0

    printf("--- Aho-Corasick Multi-Pattern Automaton ---\n");
    ac_insert("he");
    ac_insert("she");
    ac_insert("his");
    ac_insert("hers");

    ac_build_failure_links();

    const char *text = "ushers";
    printf("Searching text: \"%s\"\n", text);
    ac_search(text);

    return 0;
}
