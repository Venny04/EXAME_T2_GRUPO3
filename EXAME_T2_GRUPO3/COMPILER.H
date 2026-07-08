#ifndef COMPILER_H
#define COMPILER_H

#define HASH_SIZE 101
#define MAX_LEXEME_SIZE 64

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

typedef struct
{
    char name[32];
    char type[16];
} Symbol;

typedef struct ASTNode
{
    char value[32];
    struct ASTNode *left, *right;
} ASTNode;
typedef struct HashNode
{
    char name[32], type[16];
    struct HashNode *next;
} HashNode;
typedef struct BSTNode
{
    char name[32], type[16];
    struct BSTNode *left, *right;
} BSTNode;
typedef struct AVLNode
{
    char name[32], type[16];
    int height;
    struct AVLNode *left, *right;
} AVLNode;

// Globais compartilhadas
extern ASTNode *astRoot;
extern BSTNode *bstRoot;
extern AVLNode *avlRoot;
extern HashNode *hashTable[HASH_SIZE];

// Funcoes de controle
void loadSourceAutomatic(CompilerState *state);
void loadSourceManual(CompilerState *state);
void compileCode(CompilerState *state);
void executeCode(CompilerState *state);
void lexicalAnalysis(void);
void syntaxAnalysis(void);
void printBSTMenu(void);
void printAVLMenu(void);

// Lexer e parser
void initLexer(const char *code);
Token getNextToken(void);
void advance(void);
void initParser(void);
void parseProgram(void);
void parseStatement(void);
ASTNode *parseExpressionAST(void);

// AST
ASTNode *createNode(const char *value);
void printAST(ASTNode *node, int level);
int evaluateAST(ASTNode *node);

// Hash/BST/AVL
void initHashTable(void);
void insertHash(const char *name, const char *type);
void printHashTable(void);
void initBST(void);
BSTNode *insertBST(BSTNode *root, char *name, char *type);
void printBST(BSTNode *root);
void initAVL(void);
AVLNode *insertAVL(AVLNode *node, char *name, char *type);
void printAVL(AVLNode *root);

// Simbolos e TAC
void initSymbolTable(void);
void addSymbol(const char *name, const char *type);
void printSymbolTable(void);
char *generateTAC(ASTNode *node);
void printTAC(void);
void limparTela(void);

#endif
