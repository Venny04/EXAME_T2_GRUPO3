#ifndef AST_H
#define AST_H
#include "compiler.h"

ASTNode *createNode(const char *value);
void printAST(ASTNode *node, int level);
int evaluateAST(ASTNode *node);

#endif
