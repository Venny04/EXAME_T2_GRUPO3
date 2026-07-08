#ifndef LEXER_H
#define LEXER_H
#include "compiler.h"

void initLexer(const char *code);
Token getNextToken(void);

#endif
