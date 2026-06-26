#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Necessário para isdigit, isalpha, isalnum
#include "compiler.h"

#ifndef MAX_LEXEME_SIZE
#define MAX_LEXEME_SIZE 64
#endif

ASTNode *astRoot = NULL;

// =====================
// LEXER GLOBAL
// =====================
static const char *src;
static int i = 0;
static int line = 1;
static int column = 1;
static char currentChar;
static Token currentToken;
static char *sourceCode = NULL;
static Symbol symbolTable[100];
static int symbolCount = 0;
static char tac[100][64];
static int tacCount = 0;
static int tempCount = 0;

// ====================
// HASH, BST E AVL TABLE GLOBAL
// ====================
HashNode *hashTable[HASH_SIZE];
BSTNode *bstRoot = NULL;
AVLNode *avlRoot = NULL;

// =====================
// INICIALIZAR LEXER
// =====================
void initLexer(const char *code)
{
    src = code;
    i = 0;
    line = 1;
    column = 1;
    if (src)
    {
        currentChar = src[i];
    }
    else
    {
        currentChar = '\0';
    }
}

void advance()
{
    if (currentChar == '\n')
    {
        line++;
        column = 1;
    }
    else
    {
        column++;
    }
    i++;
    currentChar = src[i];
}

void skipSpaces()
{
    while (currentChar == ' ' || currentChar == '\n' || currentChar == '\t' || currentChar == '\r')
        advance();
}

// =====================
// IDENTIFICAR PALAVRAS
// =====================
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

    if (strcmp(t.lexeme, "int") == 0)
        t.type = TOKEN_INT;

    return t;
}

// =====================
// PEGAR PRÓXIMO TOKEN
// =====================
Token getNextToken()
{
    Token t;
    skipSpaces();

    t.line = line;
    t.column = column;

    if (currentChar == '\0')
    {
        t.type = TOKEN_END;
        strcpy(t.lexeme, "EOF");
        return t;
    }

    if (isdigit((unsigned char)currentChar))
        return number();

    if (isalpha((unsigned char)currentChar))
        return id();

    if (currentChar == '=')
    {
        t.type = TOKEN_ASSIGN;
        strcpy(t.lexeme, "=");
        advance();
        return t;
    }

    if (currentChar == '+')
    {
        t.type = TOKEN_PLUS;
        strcpy(t.lexeme, "+");
        advance();
        return t;
    }

    if (currentChar == '-')
    {
        t.type = TOKEN_MINUS;
        strcpy(t.lexeme, "-");
        advance();
        return t;
    }

    if (currentChar == ';')
    {
        t.type = TOKEN_SEMICOLON;
        strcpy(t.lexeme, ";");
        advance();
        return t;
    }

    t.type = TOKEN_UNKNOWN;
    t.lexeme[0] = currentChar;
    t.lexeme[1] = '\0';
    advance();

    return t;
}

void initParser()
{
    initLexer(sourceCode);
    currentToken = getNextToken();
}

void match(TokenType expected)
{
    if (currentToken.type == expected)
    {
        currentToken = getNextToken();
    }
    else
    {
        printf("\n[ERRO SINTATICO]\n");
        printf("Linha: %d Coluna: %d\n", currentToken.line, currentToken.column);
        printf("Token encontrado: %s\n", currentToken.lexeme);
    }
}

void parseProgram()
{
    initParser();
    while (currentToken.type != TOKEN_END)
    {
        parseStatement();
    }
    printf("\nParsing concluido com sucesso!\n");
}

void parseStatement()
{
    if (currentToken.type == TOKEN_INT)
    {
        match(TOKEN_INT);

        char variableName[32];
        strcpy(variableName, currentToken.lexeme);

        insertHash(variableName, "int");
        bstRoot = insertBST(bstRoot, variableName, "int");
        avlRoot = insertAVL(avlRoot, variableName, "int");
        addSymbol(variableName, "int");

        match(TOKEN_ID);

        if (currentToken.type == TOKEN_ASSIGN)
        {
            match(TOKEN_ASSIGN);
            astRoot = parseExpressionAST();
        }
        match(TOKEN_SEMICOLON);
    }
    else if (currentToken.type == TOKEN_ID)
    {
        match(TOKEN_ID);
        match(TOKEN_ASSIGN);
        astRoot = parseExpressionAST();
        match(TOKEN_SEMICOLON);
    }
    else
    {
        printf("\n[ERRO] Statement invalido\n");
        advance();
    }
}

ASTNode *parseExpressionAST()
{
    ASTNode *left = NULL;

    if (currentToken.type == TOKEN_NUMBER || currentToken.type == TOKEN_ID)
    {
        left = createNode(currentToken.lexeme);
        match(currentToken.type);
    }

    while (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS)
    {
        char op[2];
        op[0] = currentToken.lexeme[0];
        op[1] = '\0';

        match(currentToken.type);

        ASTNode *right = NULL;
        if (currentToken.type == TOKEN_NUMBER || currentToken.type == TOKEN_ID)
        {
            right = createNode(currentToken.lexeme);
            match(currentToken.type);
        }

        ASTNode *parent = createNode(op);
        if (parent)
        {
            parent->left = left;
            parent->right = right;
            left = parent;
        }
    }
    return left;
}

ASTNode *createNode(const char *value)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (node)
    {
        strcpy(node->value, value);
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

void printAST(ASTNode *node, int level)
{
    if (node == NULL)
        return;

    for (int j = 0; j < level; j++)
        printf("  ");

    printf("%s\n", node->value);
    printAST(node->left, level + 1);
    printAST(node->right, level + 1);
}

// ==========================================
// ESTRUTURAS DE DADOS (HASH, BST, AVL)
// ==========================================
void initHashTable()
{
    for (int j = 0; j < HASH_SIZE; j++)
        hashTable[j] = NULL;
}

unsigned int hashFunction(char *name)
{
    unsigned int hash = 0;
    while (*name)
    {
        hash = (hash * 31) + *name;
        name++;
    }
    return hash % HASH_SIZE;
}

void insertHash(char *name, char *type)
{
    unsigned int index = hashFunction(name);
    HashNode *node = malloc(sizeof(HashNode));
    if (node)
    {
        strcpy(node->name, name);
        strcpy(node->type, type);
        node->next = hashTable[index];
        hashTable[index] = node;
    }
}

void printHashTable()
{
    printf("\n===== HASH TABLE =====\n");
    for (int j = 0; j < HASH_SIZE; j++)
    {
        HashNode *node = hashTable[j];
        while (node)
        {
            printf("%s -> %s\n", node->name, node->type);
            node = node->next;
        }
    }
}

void initBST()
{
    bstRoot = NULL;
}

BSTNode *createBSTNode(char *name, char *type)
{
    BSTNode *node = malloc(sizeof(BSTNode));
    if (node)
    {
        strcpy(node->name, name);
        strcpy(node->type, type);
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

BSTNode *insertBST(BSTNode *root, char *name, char *type)
{
    if (root == NULL)
        return createBSTNode(name, type);

    if (strcmp(name, root->name) < 0)
    {
        root->left = insertBST(root->left, name, type);
    }
    else if (strcmp(name, root->name) > 0)
    {
        root->right = insertBST(root->right, name, type);
    }
    return root;
}

void printBST(BSTNode *root)
{
    if (root == NULL)
        return;
    printBST(root->left);
    printf("%s -> %s\n", root->name, root->type);
    printBST(root->right);
}

int heightAVL(AVLNode *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

AVLNode *createAVLNode(char *name, char *type)
{
    AVLNode *node = malloc(sizeof(AVLNode));
    if (node)
    {
        strcpy(node->name, name);
        strcpy(node->type, type);
        node->left = NULL;
        node->right = NULL;
        node->height = 1;
    }
    return node;
}

AVLNode *rotateRight(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *temp = x->right;
    x->right = y;
    y->left = temp;
    y->height = 1 + max(heightAVL(y->left), heightAVL(y->right));
    x->height = 1 + max(heightAVL(x->left), heightAVL(x->right));
    return x;
}

AVLNode *rotateLeft(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *temp = y->left;
    y->left = x;
    x->right = temp;
    x->height = 1 + max(heightAVL(x->left), heightAVL(x->right));
    y->height = 1 + max(heightAVL(y->left), heightAVL(y->right));
    return y;
}

int getBalance(AVLNode *N)
{
    if (N == NULL)
        return 0;
    return heightAVL(N->left) - heightAVL(N->right);
}

AVLNode *insertAVL(AVLNode *node, char *name, char *type)
{
    if (node == NULL)
        return createAVLNode(name, type);

    if (strcmp(name, node->name) < 0)
        node->left = insertAVL(node->left, name, type);
    else if (strcmp(name, node->name) > 0)
        node->right = insertAVL(node->right, name, type);
    else
        return node;

    node->height = 1 + max(heightAVL(node->left), heightAVL(node->right));
    int balance = getBalance(node);

    if (balance > 1 && strcmp(name, node->left->name) < 0)
        return rotateRight(node);

    if (balance < -1 && strcmp(name, node->right->name) > 0)
        return rotateLeft(node);

    if (balance > 1 && strcmp(name, node->left->name) > 0)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && strcmp(name, node->right->name) < 0)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

void printAVL(AVLNode *root)
{
    if (root == NULL)
        return;
    printAVL(root->left);
    printf("%s -> %s\n", root->name, root->type);
    printAVL(root->right);
}

void initAVL()
{
    avlRoot = NULL;
}

// ===================
// EXECUÇÃO DA AST E TAC
// ===================
int evaluateAST(ASTNode *node)
{
    if (node == NULL)
        return 0;

    if (isdigit((unsigned char)node->value[0]))
    {
        return atoi(node->value);
    }
    if (strcmp(node->value, "+") == 0)
    {
        return evaluateAST(node->left) + evaluateAST(node->right);
    }
    if (strcmp(node->value, "-") == 0)
    {
        return evaluateAST(node->left) - evaluateAST(node->right);
    }
    return 0;
}

void executeCode(CompilerState *state)
{
    if (!state->compiled)
    {
        printf("\nCodigo ainda nao compilado!\n");
        return;
    }
    printf("\n===== EXECUÇÃO =====\n");
    if (astRoot == NULL)
    {
        printf("AST vazia!\n");
        return;
    }
    printf("Resultado: %d\n", evaluateAST(astRoot));
}

char *generateTAC(ASTNode *node)
{
    static char resultTemp[32];
    if (node == NULL)
        return "";

    if (node->left == NULL && node->right == NULL)
    {
        return node->value;
    }

    char leftVar[32];
    char rightVar[32];
    strcpy(leftVar, generateTAC(node->left));
    strcpy(rightVar, generateTAC(node->right));

    if (strcmp(node->value, "+") == 0 || strcmp(node->value, "-") == 0)
    {
        char temp[32];
        sprintf(temp, "t%d", tempCount++);
        sprintf(tac[tacCount++], "%s = %s %s %s", temp, leftVar, node->value, rightVar);
        strcpy(resultTemp, temp);
        return resultTemp;
    }
    return node->value;
}

void printTAC()
{
    printf("\n===== CODIGO INTERMEDIARIO (TAC) =====\n");
    for (int j = 0; j < tacCount; j++)
    {
        printf("%s\n", tac[j]);
    }
}

// =====================
// TABELA DE SÍMBOLOS BÁSICA
// =====================
void initSymbolTable()
{
    symbolCount = 0;
}

int symbolExists(const char *name)
{
    for (int j = 0; j < symbolCount; j++)
    {
        if (strcmp(symbolTable[j].name, name) == 0)
            return 1;
    }
    return 0;
}

void addSymbol(const char *name, const char *type)
{
    if (symbolExists(name))
    {
        printf("\n[SEMANTICO] Variavel '%s' ja declarada!\n", name);
        return;
    }
    strcpy(symbolTable[symbolCount].name, name);
    strcpy(symbolTable[symbolCount].type, type);
    symbolCount++;
}

void printSymbolTable()
{
    printf("\n===== TABELA DE SIMBOLOS =====\n");
    for (int j = 0; j < symbolCount; j++)
    {
        printf("%s -> %s\n", symbolTable[j].name, symbolTable[j].type);
    }
}

// =====================
// MANIPULAÇÃO DE FICHEIROS
// =====================
char *readFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Erro ao abrir ficheiro!\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(size + 1);
    if (buffer)
    {
        size_t read_bytes = fread(buffer, 1, size, file);
        buffer[read_bytes] = '\0';
    }
    fclose(file);
    return buffer;
}

void loadSourceAutomatic(CompilerState *state)
{
    int op;
    printf("\n1 - Codigo sem erro\n2 - Codigo com erro\nEscolha: ");
    if (scanf("%d", &op) != 1)
        return;

    if (sourceCode)
        free(sourceCode);

    if (op == 1)
        sourceCode = readFile("codigo_sem_erro.txt");
    else
        sourceCode = readFile("codigo_com_erro.txt");

    if (sourceCode)
    {
        state->loaded = 1;
        state->compiled = 0; // Reseta status antigo
        printf("\nCodigo carregado com sucesso!\n");
    }
}

void loadSourceManual(CompilerState *state)
{
    getchar(); // Limpa buffer
    char buffer[1000];

    printf("\nEscreva o codigo (linha unica):\n");
    if (fgets(buffer, 1000, stdin) == NULL)
        return;

    if (sourceCode)
        free(sourceCode);

    sourceCode = (char *)malloc(strlen(buffer) + 1);
    if (sourceCode)
    {
        strcpy(sourceCode, buffer);
        state->loaded = 1;
        state->compiled = 0; // Reseta status antigo
        printf("\nCodigo inserido com sucesso!\n");
    }
}

// ==========================================
// FUNÇÕES DE PONTE (WRAPPERS PARA O MAIN.C)
// ==========================================
void lexicalAnalysis()
{
    if (!sourceCode)
    {
        printf("\n[ERRO] Nenhum codigo fonte carregado.\n");
        return;
    }
    printf("\n--- Executando Analise Lexica Isolada ---\n");
    initLexer(sourceCode);
    Token t;
    do
    {
        t = getNextToken();
        printf("Token: %-10d Lexeme: %s\n", t.type, t.lexeme);
    } while (t.type != TOKEN_END);
}

void syntaxAnalysis()
{
    if (!sourceCode)
    {
        printf("\n[ERRO] Nenhum codigo fonte carregado.\n");
        return;
    }
    printf("\n--- Executando Analise Sintatica Isolada ---\n");
    parseProgram();
}

void printBSTMenu()
{
    printf("\n===== EXIBINDO ARVORE BST =====\n");
    if (bstRoot == NULL)
        printf("Arvore BST vazia!\n");
    else
        printBST(bstRoot);
}

void printAVLMenu()
{
    printf("\n===== EXIBINDO ARVORE AVL =====\n");
    if (avlRoot == NULL)
        printf("Arvore AVL vazia!\n");
    else
        printAVL(avlRoot);
}

// =====================
// COMPILAÇÃO COMPLETA
// =====================
void compileCode(CompilerState *state)
{
    if (!state->loaded || !sourceCode)
    {
        printf("\nNenhum codigo carregado!\n");
        return;
    }

    printf("\n===== COMPILACAO INICIADA =====\n");
    tacCount = 0;
    tempCount = 0;

    initSymbolTable();
    initHashTable();
    initBST();
    initAVL();

    initLexer(sourceCode);
    Token t;
    int hasError = 0;

    do
    {
        t = getNextToken();
        if (t.type == TOKEN_UNKNOWN)
            hasError = 1;
    } while (t.type != TOKEN_END);

    if (hasError)
    {
        state->hasErrors = 1;
        state->compiled = 0;
        printf("\n[ERRO] Erros lexicos encontrados! 1 - Correcao Automatica, 2 - Manual: ");
        int op;
        if (scanf("%d", &op) != 1)
            return;

        if (op == 1)
        {
            for (int j = 0; sourceCode[j] != '\0'; j++)
            {
                if (!isalnum((unsigned char)sourceCode[j]) && sourceCode[j] != ' ' &&
                    sourceCode[j] != ';' && sourceCode[j] != '+' && sourceCode[j] != '-' &&
                    sourceCode[j] != '=' && sourceCode[j] != '\n' && sourceCode[j] != '\t')
                {
                    sourceCode[j] = ' ';
                }
            }
            printf("Corrigido automaticamente. Recompile por favor.\n");
        }
        return;
    }

    parseProgram();

    if (astRoot != NULL)
    {
        generateTAC(astRoot);
    }

    state->hasErrors = 0;
    state->compiled = 1;
    printf("\nCompilacao de todas as fases concluida com sucesso!\n");
}