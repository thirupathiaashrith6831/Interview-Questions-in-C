#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min3(int a, int b, int c) {
    if (a < b && a < c) return a;
    return (b < c) ? b : c;
}

int levenshtein_distance(const char *s1, const char *s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    // Allocate 2D DP table
    int **dp = malloc((len1 + 1) * sizeof(int *));
    for (int i = 0; i <= len1; i++) {
        dp[i] = malloc((len2 + 1) * sizeof(int));
    }

    for (int i = 0; i <= len1; i++) dp[i][0] = i;
    for (int j = 0; j <= len2; j++) dp[0][j] = j;

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dp[i][j] = min3(
                dp[i - 1][j] + 1,       // Deletion
                dp[i][j - 1] + 1,       // Insertion
                dp[i - 1][j - 1] + cost // Substitution
            );
        }
    }

    int distance = dp[len1][len2];

    for (int i = 0; i <= len1; i++) free(dp[i]);
    free(dp);

    return distance;
}

int main(void) {
    const char *target = "algorithm";
    const char *test_words[] = {"algo", "altruism", "logarithm", "algorithm", "alligator"};

    printf("Target Word: '%s'\n\n", target);
    printf("--- Levenshtein Edit Distance Calculation ---\n");

    for (int i = 0; i < 5; i++) {
        int dist = levenshtein_distance(target, test_words[i]);
        printf("Word: %-12s | Edit Distance: %d\n", test_words[i], dist);
    }

    return 0;
}
