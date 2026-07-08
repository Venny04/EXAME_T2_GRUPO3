#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compiler.h"

// Globais do Lexer
static const char *src;
static int i = 0;
static int line = 1;
static int column = 1;
static char currentChar;

void initLexer(const char *code)
{
    src = code;
    i = 0;
    line = 1;
    column = 1;
    if (src) currentChar = src[i];
    else currentChar = '\0';
}

void advance()
{
    if (currentChar == '\n') { line++; column = 1; }
    else column++;
    i++;
    currentChar = src[i];
}

void skipSpaces()
{
    while (currentChar == ' ' || currentChar == '\n' || currentChar == '\t' || currentChar == '\r')
        advance();
}

Token number()
{
    Token t;
    t.type = TOKEN_NUMBER;
    t.line = line;
    t.column = column;
    int k = 0;
    while (isdigit((unsigned char)currentChar) && k < (MAX_LEXEME_SIZE - 1))
    {
        t.lexeme[k++] = currentChar;
        advance();
    }
    t.lexeme[k] = '\0';
    return t;
}

Token id()
{
    Token t;
    t.type = TOKEN_ID;
    t.line = line;
    t.column = column;
    int k = 0;
    while (isalnum((unsigned char)currentChar) && k < (MAX_LEXEME_SIZE - 1))
    {
        t.lexeme[k++] = currentChar;
        advance();
    }
    t.lexeme[k] = '\0';
    if (strcmp(t.lexeme, "int") == 0) t.type = TOKEN_INT;
    return t;
}

Token getNextToken()
{
    Token t;
    skipSpaces();
    t.line = line;
    t.column = column;
    if (currentChar == '\0') { t.type = TOKEN_END; strcpy(t.lexeme, "EOF"); return t; }
    if (isdigit((unsigned char)currentChar)) return number();
    if (isalpha((unsigned char)currentChar)) return id();
    if (currentChar == '=') { t.type = TOKEN_ASSIGN; strcpy(t.lexeme, "="); advance(); return t; }
    if (currentChar == '+') { t.type = TOKEN_PLUS; strcpy(t.lexeme, "+"); advance(); return t; }
    if (currentChar == '-') { t.type = TOKEN_MINUS; strcpy(t.lexeme, "-"); advance(); return t; }
    if (currentChar == ';') { t.type = TOKEN_SEMICOLON; strcpy(t.lexeme, ";"); advance(); return t; }
    t.type = TOKEN_UNKNOWN;
    t.lexeme[0] = currentChar;
    t.lexeme[1] = '\0';
    advance();
    return t;
}
