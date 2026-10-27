#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int index;
    char *suffix;
} Suffix;

int compare_suffixes(const void *a, const void *b) {
    return strcmp(((Suffix*)a)->suffix, ((Suffix*)b)->suffix);
}

void build_suffix_array(char *txt, Suffix *sa, int n) {
    for (int i = 0; i < n; i++) {
        sa[i].index = i;
        sa[i].suffix = txt + i;
    }
    qsort(sa, n, sizeof(Suffix), compare_suffixes);
}

void search_suffix_array(char *pat, char *txt, Suffix *sa, int n) {
    int m = strlen(pat);
    int l = 0, r = n - 1;
    bool found = false;

    while (l <= r) {
        int mid = l + (r - l) / 2;
        int res = strncmp(sa[mid].suffix, pat, m);

        if (res == 0) {
            printf("Pattern found at text index %d\n", sa[mid].index);
            found = true;
            // Check adjacent matches for multiple occurrences
            break;
        }
        if (res < 0) l = mid + 1;
        else r = mid - 1;
    }

    if (!found) printf("Pattern '%s' not found.\n", pat);
}

int main(void) {
    char txt[] = "banana";
    int n = strlen(txt);
    Suffix *sa = malloc(sizeof(Suffix) * n);

    printf("--- Suffix Array Construction & Search Engine ---\n");
    printf("Text: \"%s\"\n", txt);

    build_suffix_array(txt, sa, n);

    printf("Constructed Suffix Array:\n");
    for (int i = 0; i < n; i++) {
        printf("  [%2d] -> Offset %d (%s)\n", i, sa[i].index, sa[i].suffix);
    }

    printf("\nSearching for pattern \"nan\":\n");
    search_suffix_array("nan", txt, sa, n);

    free(sa);
    return 0;
}
