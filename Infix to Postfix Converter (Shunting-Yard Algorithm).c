#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

typedef struct {
    char items[MAX];
    int top;
} Stack;

void init(Stack *s) { s->top = -1; }
void push(Stack *s, char c) { s->items[++(s->top)] = c; }
char pop(Stack *s) { return s->items[(s->top)--]; }
char peek(Stack *s) { return s->items[s->top]; }
int is_empty(Stack *s) { return s->top == -1; }

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

void infix_to_postfix(const char *infix, char *postfix) {
    Stack s;
    init(&s);
    int k = 0;

    for (int i = 0; infix[i] != '\0'; i++) {
        char ch = infix[i];

        if (isalnum((unsigned char)ch)) {
            postfix[k++] = ch;
        } else if (ch == '(') {
            push(&s, ch);
        } else if (ch == ')') {
            while (!is_empty(&s) && peek(&s) != '(') {
                postfix[k++] = pop(&s);
            }
            pop(&s); // Remove '('
        } else { // Operator encountered
            while (!is_empty(&s) && precedence(peek(&s)) >= precedence(ch)) {
                postfix[k++] = pop(&s);
            }
            push(&s, ch);
        }
    }

    while (!is_empty(&s)) {
        postfix[k++] = pop(&s);
    }
    postfix[k] = '\0';
}

int main(void) {
    const char *infix = "a+b*(c^d-e)^(f+g*h)-i";
    char postfix[MAX];

    infix_to_postfix(infix, postfix);

    printf("--- Shunting-Yard Infix to Postfix Converter ---\n");
    printf("Infix   : %s\n", infix);
    printf("Postfix : %s\n", postfix);

    return 0;
}
