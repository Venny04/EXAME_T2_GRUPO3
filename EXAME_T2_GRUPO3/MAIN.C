#include <stdio.h>
#include "compiler.h"

int main(void)
{
    // Correção/Melhoria: Garante que o estado comece zerado e limpo
    CompilerState state = {0, 0, 0};
    int opcao;

    do
    {
        printf("\n");
        printf("=========================================\n");
        printf("        COMPILADOR INTELIGENTE\n");
        printf("=========================================\n");
        printf("1 - Carregar ficheiro automaticamente\n");
        printf("2 - Inserir codigo manualmente\n");
        printf("3 - Analise Lexica\n");
        printf("4 - Analise Sintatica\n");
        printf("5 - Executar AST\n");
        printf("6 - Mostrar Tabela de Simbolos\n");
        printf("7 - Mostrar Hash Table\n");
        printf("8 - Mostrar BST\n");
        printf("9 - Mostrar AVL\n");
        printf("10 - Compilar Tudo\n");
        printf("0 - Sair\n");
        printf("=========================================\n");
        printf("Opcao: ");

        // Proteção contra entrada inválida (ex: se digitarem uma letra, não quebra o menu)
        if (scanf("%d", &opcao) != 1)
        {
            printf("\nErro: Entrada invalida! Digite apenas numeros.\n");
            while (getchar() != '\n')
                ; // Limpa o buffer do teclado
            opcao = -1;
            continue;
        }

        switch (opcao)
        {
        case 1:
            loadSourceAutomatic(&state);
            break;

        case 2:
            loadSourceManual(&state);
            break;

        case 3:
            lexicalAnalysis();
            break;

        case 4:
            syntaxAnalysis();
            break;

        case 5:
            executeCode(&state);
            break;

        case 6:
            printSymbolTable();
            break;

        case 7:
            printHashTable();
            break;

        case 8:
            printBSTMenu(); // Alterado para a função wrapper que gerencia a raiz global
            break;

        case 9:
            printAVLMenu(); // Alterado para a função wrapper que gerencia a raiz global
            break;

        case 10:
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