#ifndef PARSER_H
#define PARSER_H
#include "compiler.h"

void initParser(void);
void parseProgram(void);
void parseStatement(void);
ASTNode *parseExpressionAST(void);

#endif
