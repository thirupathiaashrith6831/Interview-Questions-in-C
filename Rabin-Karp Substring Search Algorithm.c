#include <stdio.h>
#include <string.h>

#define D 256 // Number of characters in the alphabet
#define Q 101 // A prime number for modulo hashing

void search_rabin_karp(char *pat, char *txt) {
    int M = strlen(pat);
    int N = strlen(txt);
    int i, j;
    int p = 0; // Hash value for pattern
    int t = 0; // Hash value for txt
    int h = 1;

    for (i = 0; i < M - 1; i++)
        h = (h * D) % Q;

    // Calculate the hash value of pattern and first window of text
    for (i = 0; i < M; i++) {
        p = (D * p + pat[i]) % Q;
        t = (D * t + txt[i]) % Q;
    }

    // Slide the pattern over text one by one
    for (i = 0; i <= N - M; i++) {
        if (p == t) {
            // Check characters one by one if hash matches
            for (j = 0; j < M; j++) {
                if (txt[i + j] != pat[j])
                    break;
            }
            if (j == M)
                printf("Pattern found at index %d\n", i);
        }

        // Calculate hash value for next window of text: remove leading digit, add trailing digit
        if (i < N - M) {
            t = (D * (t - txt[i] * h) + txt[i + M]) % Q;
            if (t < 0)
                t = (t + Q);
        }
    }
}

int main(void) {
    char txt[] = "GEEKS FOR GEEKS";
    char pat[] = "GEEK";

    printf("--- Rabin-Karp Substring Search Engine ---\n");
    printf("Text   : %s\n", txt);
    printf("Pattern: %s\n", pat);
    search_rabin_karp(pat, txt);

    return 0;
}
