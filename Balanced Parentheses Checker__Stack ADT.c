#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX 100

typedef struct {
    char data[MAX];
    int top;
} Stack;

void stack_init(Stack *s) { s->top = -1; }
bool is_full(Stack *s) { return s->top == MAX - 1; }
bool is_empty(Stack *s) { return s->top == -1; }

void push(Stack *s, char c) {
    if (!is_full(s)) s->data[++(s->top)] = c;
}

char pop(Stack *s) {
    if (!is_empty(s)) return s->data[(s->top)--];
    return '\0';
}

bool is_matching(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '{' && close == '}') ||
           (open == '[' && close == ']');
}

bool is_balanced(const char *expr) {
    Stack s;
    stack_init(&s);
    for (int i = 0; expr[i] != '\0'; i++) {
        char c = expr[i];
        if (c == '(' || c == '{' || c == '[') {
            push(&s, c);
        } else if (c == ')' || c == '}' || c == ']') {
            if (is_empty(&s) || !is_matching(pop(&s), c)) return false;
        }
    }
    return is_empty(&s);
}

int main(void) {
    const char *test1 = "{ [ ( a + b ) * c ] }";
    const char *test2 = "( ( a + b ) * c ]";

    printf("--- Balanced Parentheses Checker ---\n");
    printf("Expr: %-25s -> %s\n", test1, is_balanced(test1) ? "BALANCED" : "UNBALANCED");
    printf("Expr: %-25s -> %s\n", test2, is_balanced(test2) ? "BALANCED" : "UNBALANCED");

    return 0;
}
