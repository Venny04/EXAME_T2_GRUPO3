#include <stdio.h>
#include <string.h>
#include "compiler.h"

extern Token currentToken;
extern char *sourceCode;
extern ASTNode *astRoot;
extern BSTNode *bstRoot;
extern AVLNode *avlRoot;

void initParser()
{
    initLexer(sourceCode);
    currentToken = getNextToken();
}

void match(TokenType expected)
{
    if (currentToken.type == expected) currentToken = getNextToken();
    else {
        printf("\n[ERRO SINTATICO]\nLinha: %d Coluna: %d\nToken encontrado: %s\n", 
                currentToken.line, currentToken.column, currentToken.lexeme);
    }
}

void parseProgram()
{
    initParser();
    while (currentToken.type != TOKEN_END) parseStatement();
    printf("\nParsing concluido com sucesso!\n");
}

void parseStatement()
{
    if (currentToken.type == TOKEN_INT) {
        match(TOKEN_INT);
        char variableName[32];
        strcpy(variableName, currentToken.lexeme);
        insertHash(variableName, "int");
        bstRoot = insertBST(bstRoot, variableName, "int");
        avlRoot = insertAVL(avlRoot, variableName, "int");
        addSymbol(variableName, "int");
        match(TOKEN_ID);
        if (currentToken.type == TOKEN_ASSIGN) {
            match(TOKEN_ASSIGN);
            astRoot = parseExpressionAST();
        }
        match(TOKEN_SEMICOLON);
    }
    else if (currentToken.type == TOKEN_ID) {
        match(TOKEN_ID);
        match(TOKEN_ASSIGN);
        astRoot = parseExpressionAST();
        match(TOKEN_SEMICOLON);
    }
    else { printf("\n[ERRO] Statement invalido\n"); advance(); }
}

ASTNode *parseExpressionAST()
{
    ASTNode *left = NULL;
    if (currentToken.type == TOKEN_NUMBER || currentToken.type == TOKEN_ID) {
        left = createNode(currentToken.lexeme);
        match(currentToken.type);
    }
    while (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS) {
        char op[2];
        op[0] = currentToken.lexeme[0]; op[1] = '\0';
        match(currentToken.type);
        ASTNode *right = NULL;
        if (currentToken.type == TOKEN_NUMBER || currentToken.type == TOKEN_ID) {
            right = createNode(currentToken.lexeme);
            match(currentToken.type);
        }
        ASTNode *parent = createNode(op);
        if (parent) { parent->left = left; parent->right = right; left = parent; }
    }
    return left;
}
