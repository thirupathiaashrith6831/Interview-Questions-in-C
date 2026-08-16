#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 50
#define MAX_LEN 30

typedef struct {
    char word[MAX_LEN];
    int count;
} WordFreq;

void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

int main(void) {
    char text[] = "C is clean C is fast and C is fun";
    WordFreq dict[MAX_WORDS];
    int word_count = 0;

    char *token = strtok(text, " ");
    while (token != NULL) {
        to_lowercase(token);
        int found = -1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(dict[i].word, token) == 0) {
                found = i;
                break;
            }
        }
        if (found != -1) {
            dict[found].count++;
        } else if (word_count < MAX_WORDS) {
            strncpy(dict[word_count].word, token, MAX_LEN - 1);
            dict[word_count].word[MAX_LEN - 1] = '\0';
            dict[word_count].count = 1;
            word_count++;
        }
        token = strtok(NULL, " ");
    }

    printf("--- Word Frequency Count ---\n");
    for (int i = 0; i < word_count; i++) {
        printf("%-10s : %d\n", dict[i].word, dict[i].count);
    }
    return 0;
}
