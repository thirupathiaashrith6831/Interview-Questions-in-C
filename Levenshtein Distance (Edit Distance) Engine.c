#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int min3(int a, int b, int c) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

int levenshtein_distance(const char *s1, const char *s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int **dp = malloc(sizeof(int*) * (len1 + 1));
    for (int i = 0; i <= len1; i++) {
        dp[i] = malloc(sizeof(int) * (len2 + 1));
    }

    for (int i = 0; i <= len1; i++) dp[i][0] = i;
    for (int j = 0; j <= len2; j++) dp[0][j] = j;

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dp[i][j] = min3(
                dp[i - 1][j] + 1,     // Deletion
                dp[i][j - 1] + 1,     // Insertion
                dp[i - 1][j - 1] + cost // Substitution
            );
        }
    }

    int result = dp[len1][len2];
    for (int i = 0; i <= len1; i++) free(dp[i]);
    free(dp);
    return result;
}

int main(void) {
    const char *s1 = "kitten";
    const char *s2 = "sitting";

    printf("--- Levenshtein Edit Distance Engine ---\n");
    printf("String 1: \"%s\"\n", s1);
    printf("String 2: \"%s\"\n", s2);
    printf("Edit Distance: %d\n", levenshtein_distance(s1, s2));

    return 0;
}
