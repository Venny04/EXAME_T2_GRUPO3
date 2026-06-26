#ifndef COMPILER_H
#define COMPILER_H

#define HASH_SIZE 101
#define MAX_LEXEME_SIZE 64

/*==========================
 Estruturas de Controle e Símbolos
==========================*/

typedef struct
{
    int loaded;
    int compiled;
    int hasErrors;
} CompilerState;

typedef enum
{
    TOKEN_INT,
    TOKEN_ID,
    TOKEN_NUMBER,
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_SEMICOLON,
    TOKEN_UNKNOWN,
    TOKEN_END
} TokenType;

typedef struct
{
    TokenType type;
    char lexeme[MAX_LEXEME_SIZE];
    int line;
    int column;
} Token;

// Correção: Estrutura Symbol adicionada aqui!
typedef struct
{
    char name[32];
    char type[16];
} Symbol;

/*==========================
 Nós das Estruturas de Dados
==========================*/

typedef struct ASTNode
{
    char value[32];
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

typedef struct HashNode
{
    char name[32];
    char type[16];
    struct HashNode *next;
} HashNode;

typedef struct BSTNode
{
    char name[32];
    char type[16];
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

typedef struct AVLNode
{
    char name[32];
    char type[16];
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

/*==========================
 Variáveis Globais (Declarações)
==========================*/

extern ASTNode *astRoot;
extern BSTNode *bstRoot;
extern AVLNode *avlRoot;
extern HashNode *hashTable[HASH_SIZE];

/*==========================
 Funções do Sistema e Menu
==========================*/

void loadSourceAutomatic(CompilerState *state);
void loadSourceManual(CompilerState *state);
void compileCode(CompilerState *state);
void executeCode(CompilerState *state);

/*==========================
 Funções de Análise (Ponte / Wrappers)
==========================*/

void lexicalAnalysis(void);
void syntaxAnalysis(void);
void printBSTMenu(void);
void printAVLMenu(void);

/*==========================
 Lógica Interna: Lexer & Parser
==========================*/

void initLexer(const char *code);
Token getNextToken(void);
void initParser(void);
void parseProgram(void);
void parseStatement(void);
ASTNode *parseExpressionAST(void);
ASTNode *createNode(const char *value);

/*==========================
 Lógica Interna: Tabelas e Árvores
==========================*/

// Hash
void initHashTable(void);
unsigned int hashFunction(char *name);
void insertHash(char *name, char *type);
void printHashTable(void);

// BST
void initBST(void);
BSTNode *insertBST(BSTNode *root, char *name, char *type);
void printBST(BSTNode *root);

// AVL
void initAVL(void);
AVLNode *insertAVL(AVLNode *node, char *name, char *type);
void printAVL(AVLNode *root);

// Símbolos Básica e TAC
void initSymbolTable(void);
void addSymbol(const char *name, const char *type);
void printSymbolTable(void);
char *generateTAC(ASTNode *node);
void printTAC(void);

#endif /* COMPILER_H */