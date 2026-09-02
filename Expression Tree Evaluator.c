#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct ExpNode {
    char value;
    struct ExpNode *left;
    struct ExpNode *right;
} ExpNode;

ExpNode* create_exp_node(char val) {
    ExpNode *node = malloc(sizeof(ExpNode));
    node->value = val;
    node->left = node->right = NULL;
    return node;
}

ExpNode* build_tree_from_postfix(const char *postfix) {
    ExpNode *stack[100];
    int top = -1;

    for (int i = 0; postfix[i] != '\0'; i++) {
        char ch = postfix[i];

        ExpNode *node = create_exp_node(ch);
        if (isdigit((unsigned char)ch)) {
            stack[++top] = node;
        } else { // Operator
            node->right = stack[top--];
            node->left = stack[top--];
            stack[++top] = node;
        }
    }
    return stack[top];
}

int evaluate_exp_tree(ExpNode *root) {
    if (!root) return 0;

    if (isdigit((unsigned char)root->value)) {
        return root->value - '0';
    }

    int left_val = evaluate_exp_tree(root->left);
    int right_val = evaluate_exp_tree(root->right);

    switch (root->value) {
        case '+': return left_val + right_val;
        case '-': return left_val - right_val;
        case '*': return left_val * right_val;
        case '/': return left_val / right_val;
    }
    return 0;
}

void free_exp_tree(ExpNode *root) {
    if (!root) return;
    free_exp_tree(root->left);
    free_exp_tree(root->right);
    free(root);
}

int main(void) {
    // Postfix expression representing: (5 + 3) * 4 -> "53+4*"
    const char *postfix = "53+4*";

    ExpNode *root = build_tree_from_postfix(postfix);

    printf("--- Expression Tree Evaluator ---\n");
    printf("Postfix Expression : %s\n", postfix);
    printf("Evaluated Result   : %d\n", evaluate_exp_tree(root));

    free_exp_tree(root);
    return 0;
}
