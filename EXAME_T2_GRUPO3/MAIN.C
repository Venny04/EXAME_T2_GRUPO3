#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"

int main(void)
{
    // Garante que o estado comece zerado e limpo
    CompilerState state = {0, 0, 0};
    int opcao;

    do
    {
        printf("\n");
        printf("===========================================\n");
        printf("+       COMPILADOR INTELIGENTE            +\n");
        printf("===========================================\n");
        printf("1 - Inserir codigo manualmente            +\n");
        printf("2 - Analise Lexica                        +\n");
        printf("3 - Analise Sintatica                     +\n");
        printf("4 - Executar AST                          +\n");
        printf("5 - Mostrar Tabela de Simbolos            +\n");
        printf("6 - Mostrar Hash Table                    +\n");
        printf("7 - Mostrar BST                           +\n");
        printf("8 - Mostrar AVL                           +\n");
        printf("9 - Compilar Tudo                        +\n");
        printf("0 - Sair                                  +\n");
        printf("===========================================\n");
        printf("Opcao: ");

        // Protecao contra entrada invalida
        if (scanf("%d", &opcao) != 1)
        {
            printf("\nErro: Entrada invalida! Digite apenas numeros.\n");
            while (getchar() != '\n')
                ;
            opcao = -1;
            continue;
        }

        switch (opcao)
        {
        case 1:
            loadSourceManual(&state);
            break;

        case 2:
            lexicalAnalysis();
            break;

        case 3:
            syntaxAnalysis();
            break;

        case 4:
            executeCode(&state);
            break;

        case 5:
            limparTela();
            printSymbolTable();
            break;

        case 6:
            limparTela();
            printHashTable();
            break;

        case 7:
            limparTela();
            printBSTMenu(); // funcao wrapper que gerencia a raiz global
            break;

        case 8:
            limparTela();
            printAVLMenu(); // funcao wrapper que gerencia a raiz global
            break;

        case 9:
            limparTela();
            compileCode(&state);
            break;

        case 0:
            printf("\nEncerrando...\n");
            break;

        default:
            printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
