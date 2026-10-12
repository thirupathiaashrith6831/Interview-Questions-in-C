#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_UNKNOWN
} TokenType;

void print_token(TokenType type, const char *lexeme) {
    const char *names[] = {"KEYWORD", "IDENTIFIER", "NUMBER", "OPERATOR", "UNKNOWN"};
    printf("  %-12s : %s\n", names[type], lexeme);
}

bool is_keyword(const char *str) {
    return strcmp(str, "if") == 0 || strcmp(str, "while") == 0 || strcmp(str, "return") == 0;
}

void tokenize(const char *src) {
    int i = 0;
    int len = strlen(src);

    while (i < len) {
        char c = src[i];

        if (isspace((unsigned char)c)) {
            i++;
            continue;
        }

        if (isalpha((unsigned char)c)) {
            char buffer[32];
            int b = 0;
            while (i < len && (isalnum((unsigned char)src[i]) || src[i] == '_')) {
                if (b < 31) buffer[b++] = src[i];
                i++;
            }
            buffer[b] = '\0';
            print_token(is_keyword(buffer) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER, buffer);
        } else if (isdigit((unsigned char)c)) {
            char buffer[32];
            int b = 0;
            while (i < len && isdigit((unsigned char)src[i])) {
                if (b < 31) buffer[b++] = src[i];
                i++;
            }
            buffer[b] = '\0';
            print_token(TOKEN_NUMBER, buffer);
        } else {
            char buffer[2] = {c, '\0'};
            print_token(TOKEN_OPERATOR, buffer);
            i++;
        }
    }
}

int main(void) {
    const char *source_code = "if (x5 >= 100) return result;";

    printf("--- Lexical Tokenizer Engine ---\n");
    printf("Source: %s\n\nTokens:\n", source_code);
    tokenize(source_code);

    return 0;
}
