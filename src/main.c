#include <stdio.h>
#include"grafo.h"

/*
 * Desafio de programacao
 * Disciplina: SCC0216 - Modelagem computacional em Grafos.
 * Turma: 1/A(Terça feira).
 *
 * Grupo:
 *          Nome                                numUSP
 *          Leonardo Altemari Nogueira          10284883
 *          Pedro Henrique Magalhães Cisdeli    10289804
 *          Leonardo Gabriel Fusineli Silva     11218841
*/

/*
 * Trabalho:
 *  Implementacao de um grafo valorado, aplicado ao jogo pacman e utilizando
 *  o algoritmo WaveFront para aplicar as movimentacoes do personagem no mapa.
 *  Pacman eh representado pelo caracter '@', o fantasma eh representado pelo caracter '$'.
*/
int main() {
    // Mapa do jogo
    int N;
    printf("Entre com a dimensao N do mapa(mapa sera NxN): ");
    scanf("%d", &N);

    // Cria um gravo de N² vertices
    Grafo *g;
    criaGrafo(&g, N);
    if(g != NULL ){
        printf("Mapa criado: \n");
        imprimeGrafo(g);
    }

    // Posicao do PACMAN
    int pacX;
    int pacY;
    do {
        // Entrada
        printf("Entre com a posição (x, y) do PACMAN: ");
        scanf("%d %d", &pacX, &pacY);

        // Tratamento de entrada invalida
        if(pacX < 0 || pacX >= N || pacY < 0 || pacY >= N) {
            printf("Valores (x, y) para o pacman sao incorretos\n");
        }

    } while(pacX < 0 || pacX >= N ||
            pacY < 0 || pacY >= N); // Ira sair do loop quando a posicao for valida

    // Posicao do FANTASMA
    int ghostX;
    int ghostY;
    do {
        // Entrada
        printf("Entre com a posição (x, y) do FANTASMA: ");
        scanf("%d %d", &ghostX, &ghostY);

        // Tratamento de entrada invalida
        if(ghostX < 0 || ghostX >= N || ghostY < 0 || ghostY >= N) {
            printf("Valores (x, y) para o fantasma sao incorretos\n");
        }

    } while(ghostX < 0 || ghostX >= N ||
            ghostY < 0 || ghostY >= N);    // Ira sair do loop quando a posicao for valida

    // Tendo as posicoes validas elas sao adicionadas no grafo
    adicionaPacman(g, pacX, pacY);
    adicionaFantasma(g, ghostX, ghostY);

    // Imprime o grafo com as posicoes do pacman e do fantasma
    imprimeGrafo(g);

    int op;
    // Lopp para movimentar o pacman ate alguem ganhar
    while(getStatus(g) == 0) {
        printf("Escolha uma opcao: \n");
        printf("1 - Mover o PACMAN\n");
        printf("2 - Visualizar o grafo valorado\n");
        printf("Opcao: ");
        scanf("%d", &op);

        switch (op) {
        case 1:

            printf("**********************\n");
            printf("** JOGADA DO PACMAN **\n");
            printf("**********************\n");
            moverPacman(g);
            imprimeGrafo(g);

            // Se o pacman nao ganhou o jogo na jogada dele, faz o movimento do fantasma
            if(getStatus(g) == 0) {
                printf("************************\n");
                printf("** JOGADA DO FANTASMA **\n");
                printf("************************\n");
                moverFantasma(g);
                imprimeGrafo(g);
            }
            break;

        case 2:
            imprimeGrafoValorado(g);
            break;
        }
    }

    if(getStatus(g) == 1) {
        printf("Pacman GANHOU!!!!\n");
        imprimeGrafoValorado(g);
    }
    if(getStatus(g) == -1) {
        printf("Pacman PERDEU, voce foi comido!!!!\n");
        imprimeGrafoValorado(g);
    }


    // Desaloca grafo da memoria, finaliza o programa.
    destroiGrafo(&g);
    return 0;
}
