#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compiler.h"

void limparTela(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
// Globais de execução e símbolo
static Symbol symbolTable[100];
static int symbolCount = 0;
static char tac[100][64];
static int tacCount = 0;
static int tempCount = 0;
char *sourceCode = NULL;
Token currentToken;

void initSymbolTable() { symbolCount = 0; }

int symbolExists(const char *name)
{
    for (int j = 0; j < symbolCount; j++)
        if (strcmp(symbolTable[j].name, name) == 0) return 1;
    return 0;
}

void addSymbol(const char *name, const char *type)
{
    if (symbolExists(name)) { printf("\n[SEMANTICO] Variavel '%s' ja declarada!\n", name); return; }
    strcpy(symbolTable[symbolCount].name, name);
    strcpy(symbolTable[symbolCount].type, type);
    symbolCount++;
}

void printSymbolTable()
{
    printf("\n===== TABELA DE SIMBOLOS =====\n");
    for (int j = 0; j < symbolCount; j++) printf("%s -> %s\n", symbolTable[j].name, symbolTable[j].type);
}

char *generateTAC(ASTNode *node)
{
    static char resultTemp[32];
    if (node == NULL) return "";
    if (node->left == NULL && node->right == NULL) return node->value;
    char leftVar[32], rightVar[32];
    strcpy(leftVar, generateTAC(node->left));
    strcpy(rightVar, generateTAC(node->right));
    if (strcmp(node->value, "+") == 0 || strcmp(node->value, "-") == 0) {
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
    for (int j = 0; j < tacCount; j++) printf("%s\n", tac[j]);
}

void executeCode(CompilerState *state)
{
    if (!state->compiled) { printf("\nCodigo ainda nao compilado!\n"); return; }
    printf("\n===== EXECUÇÃO =====\n");
    if (astRoot == NULL) { printf("AST vazia!\n"); return; }
    printf("Resultado: %d\n", evaluateAST(astRoot));
}

void compileCode(CompilerState *state)
{
    if (!state->loaded || !sourceCode) { printf("\nNenhum codigo carregado!\n"); return; }
    printf("\n===== COMPILACAO INICIADA =====\n");
    tacCount = 0; tempCount = 0;
    initSymbolTable(); initHashTable(); initBST(); initAVL();
    initLexer(sourceCode);
    Token t;
    int hasError = 0;
    do { t = getNextToken(); if (t.type == TOKEN_UNKNOWN) hasError = 1; } while (t.type != TOKEN_END);
    if (hasError) {
        state->hasErrors = 1; state->compiled = 0;
        printf("\n[ERRO] Erros lexicos! 1 - Correcao Automatica, 2 - Manual: ");
        int op; if (scanf("%d", &op) == 1 && op == 1) {
            for (int j = 0; sourceCode[j] != '\0'; j++)
                if (!isalnum((unsigned char)sourceCode[j]) && sourceCode[j] != ' ' && sourceCode[j] != ';' && sourceCode[j] != '+' && sourceCode[j] != '-' && sourceCode[j] != '=' && sourceCode[j] != '\n' && sourceCode[j] != '\t')
                    sourceCode[j] = ' ';
            printf("Corrigido automaticamente. Recompile.\n");
        }
        return;
    }
    parseProgram();
    if (astRoot != NULL) generateTAC(astRoot);
    state->hasErrors = 0; state->compiled = 1;
    printf("\nCompilacao de todas as fases concluida com sucesso!\n");
}

// Funções utilitárias mantidas em Compiler.c para encapsulamento
char *readFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) { printf("Erro ao abrir ficheiro!\n"); return NULL; }
    fseek(file, 0, SEEK_END); long size = ftell(file); rewind(file);
    char *buffer = (char *)malloc(size + 1);
    if (buffer) { size_t read_bytes = fread(buffer, 1, size, file); buffer[read_bytes] = '\0'; }
    fclose(file); return buffer;
}

void loadSourceAutomatic(CompilerState *state)
{
    int op; printf("\n1 - Codigo sem erro\n2 - Codigo com erro\nEscolha: ");
    if (scanf("%d", &op) != 1) return;
    if (sourceCode) free(sourceCode);
    sourceCode = (op == 1) ? readFile("codigo_sem_erro.txt") : readFile("codigo_com_erro.txt");
    if (sourceCode) { state->loaded = 1; state->compiled = 0; printf("\nCodigo carregado!\n"); }
}

void loadSourceManual(CompilerState *state)
{
    getchar(); char buffer[1000]; printf("\nEscreva o codigo (linha unica):\n");
    if (fgets(buffer, 1000, stdin) == NULL) return;
    if (sourceCode) free(sourceCode);
    sourceCode = (char *)malloc(strlen(buffer) + 1);
    if (sourceCode) { strcpy(sourceCode, buffer); state->loaded = 1; state->compiled = 0; printf("\nCodigo inserido!\n"); }
}

void lexicalAnalysis()
{
    if (!sourceCode) { printf("\n[ERRO] Nenhum codigo fonte carregado.\n"); return; }
    printf("\n--- Executando Analise Lexica ---\n");
    initLexer(sourceCode); Token t;
    do { t = getNextToken(); printf("Token: %-10d Lexeme: %s\n", t.type, t.lexeme); } while (t.type != TOKEN_END);
}

void syntaxAnalysis()
{
    if (!sourceCode) { printf("\n[ERRO] Nenhum codigo fonte carregado.\n"); return; }
    printf("\n--- Executando Analise Sintatica ---\n");
    parseProgram();
}

void printBSTMenu() { printf("\n===== EXIBINDO ARVORE BST =====\n"); (bstRoot == NULL) ? printf("Arvore vazia!\n") : printBST(bstRoot); }
void printAVLMenu() { printf("\n===== EXIBINDO ARVORE AVL =====\n"); (avlRoot == NULL) ? printf("Arvore vazia!\n") : printAVL(avlRoot); }
