#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compiler.h"

ASTNode *astRoot = NULL;

ASTNode *createNode(const char *value)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (node) { strcpy(node->value, value); node->left = NULL; node->right = NULL; }
    return node;
}

void printAST(ASTNode *node, int level)
{
    if (node == NULL) return;
    for (int j = 0; j < level; j++) printf("  ");
    printf("%s\n", node->value);
    printAST(node->left, level + 1);
    printAST(node->right, level + 1);
}

int evaluateAST(ASTNode *node)
{
    if (node == NULL) return 0;
    if (isdigit((unsigned char)node->value[0])) return atoi(node->value);
    if (strcmp(node->value, "+") == 0) return evaluateAST(node->left) + evaluateAST(node->right);
    if (strcmp(node->value, "-") == 0) return evaluateAST(node->left) - evaluateAST(node->right);
    return 0;
}
