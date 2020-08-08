#include "grafo.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

// Estrutura de um ponto (x, y)
struct p {
    int x;
    int y;
};

// Estrutura de um vertice do Grafo
// Todo vertice eh composto por um ponto (x, y) e por um valor
struct v {
    Posicao pos;
    int valor;
};

/* Estrutura de um Grafo valorado que simula o mapa do jogo PACMAN
* Cada posicao do mapa eh um vertice do grafo
* Os atributos 'pacman' e 'fantasma' guardam as posicoes do pacman e do fantasma, respectivamente, no mapa.
* O valor 'dimensao' guarda a dimensao do mapa do jogo
* Status se refere ao andamento do jogo:
*       0  - o jogo ainda nao terminou.
*       1  - Pacman venceu o jogo.
*       -1 - Fantasma venceu o jogo.
*/
struct g {
    Vertice **grafo;
    Posicao pacman;
    Posicao fantasma;
    int dimensao;
    int status;
};


/*
 * A funcao criaGrafo() eh responsavel por alocar a estrutura do grafo na memoria, alocando
 * tambem o mapa do jogo, dado uma dimensao passado por parametro.
 * Parametros:
 *      Grafo **g : endereco do ponteiro do grafo passado por referencia.
 *      int dimensao : numero inteiro que representa a dimensao do mapa, se a dimensao passada
 *      for N, entao o grafo tera N² vertices.
 * Retorno:
 *      Retorna 1 em caso de sucesso da alocacao de memoria.
 *      Retorna 0 em caso de falha na alocacao de memoria.
*/
int criaGrafo(Grafo **g, int dimensao) {
    // Aloca o grafo
    (*g) = malloc(sizeof(Grafo));
    (*g)->dimensao = dimensao;

    // Aloca os vertices do grafo
    (*g)->grafo = malloc(sizeof(Vertice *) * (uint)dimensao);
    for(int i = 0; i < dimensao; i++) {
        (*g)->grafo[i] = malloc(sizeof(Vertice) * (uint)dimensao);
    }

    // Inicia os atributos do grafo
    (*g)->pacman.x = -1;
    (*g)->pacman.y = -1;
    (*g)->fantasma.x = -1;
    (*g)->fantasma.y = -1;
    (*g)->status = 0;

    // Preenche os valores de cada vertice
    for(int i = 0; i < dimensao; i++) {
        for(int j = 0; j < dimensao; j++) {
            (*g)->grafo[i][j].pos.x = i;
            (*g)->grafo[i][j].pos.y = j;
            (*g)->grafo[i][j].valor = -1;
        }
    }

    if((*g) == NULL)
        return 0;
    return 1;
}

/*
 * A funcao adicionaPacman() eh responsavel por adicionar o pacman no mapa do jogo, dado uma
 * determinada posicao (x, y).
 * Parametros:
 *      Grafo *g : ponteiro para o grafo.
 *      int x : valor da posicao X do pacman no mapa.
 *      int y : valor da posicao Y do pacman no mapa.
 * Retorno:
 *      Retorna 1 caso o pacman seja adicionado no mapa.
 *      Retorna 0 caso as posicoes passadas sejam invalidas.
*/
int adicionaPacman(Grafo *g, int x, int y) {
    // Testa posicoes
    if(x < 0 || x > g->dimensao)
        return 0;
    if(y < 0 || y > g->dimensao)
        return 0;

    // Adiciona pacman no mapa
    g->pacman.x = x;
    g->pacman.y = y;

    return 1;
}

/*
 * A funcao adicionaFantasma() eh responsavel por adicionar o fantasma no mapa do jogo, dado uma
 * determinada posicao (x, y).
 * Parametros:
 *      Grafo *g : ponteiro para o grafo.
 *      int x : valor da posicao X do fantasma no mapa.
 *      int y : valor da posicao Y do fantasma no mapa.
 * Retorno:
 *      Retorna 1 caso o fantasma seja adicionado no mapa.
 *      Retorna 0 caso as posicoes passadas sejam invalidas.
*/
int adicionaFantasma(Grafo *g, int x, int y) {
    // Testa posicoes
    if(x < 0 || x > g->dimensao)
        return 0;
    if(y < 0 || y > g->dimensao)
        return 0;

    // Adiciona fantasma no mapa
    g->fantasma.x = x;
    g->fantasma.y = y;

    return 1;
}

/*
 * A funcao waveFront() eh reponsavel por valorar o mapa dado o algoritmo de WaveFront de
 * forma recursiva. Dado uma posicao inicial o algoritmo propaga sempre para SUL, NORTE, OESTE, LESTE
 * referente a posicao passada.
 * Parametros:
        Grafo *g : ponteiro para o grafo, para conseguir valora-lo
        int i : posicao inicial em X da propagação;
        int j : posicao inicial em Y da propagação;
        int _valor : valor inicial da propagação (Sempre deve-se passar 0 quando chamar essa funcao).
 * Retorno:
 *      retorna 0 em caso de estrapolar as dimensoes do mapa ou quando voltar em uma posicao ja mapeada.
 *      retorna 1 quando o mapa todo ja estiver valorado.
*/
int waveFront(Grafo *g, int i, int j, int _valor) {
    // Casos bases
    if(i >= g->dimensao || i < 0)
        return 0;
    if(j >= g->dimensao || j < 0)
        return 0;
    if(g->grafo[i][j].valor == 0 || g->grafo[i][j].valor == -2)
        return 0;
    if((g->grafo[i][j].valor != -1) && (g->grafo[i][j].valor < _valor))
        return 0;

    // Muda o valor para nao visitar grafos já visitados
    g->grafo[i][j].valor = -2;


    // Recursao
    waveFront(g, i+1, j, _valor+1); // SUL
    waveFront(g, i-1, j, _valor+1); // NORTE
    waveFront(g, i, j+1, _valor+1); // OESTE
    waveFront(g, i, j-1, _valor+1); // LESTE

    // Backtracking na arvore do grafo
    g->grafo[i][j].valor = _valor;

    return 1;
}

/*
 * A funcao moverP() eh auxiliar e eh responsavel por atualizar o valor da posicao do pacman, sendo
 * esse valor passado por parametro. Eh checado a 4-vizinhanca da posicao passada para
 * ver qual possui menor valor, a que tiver menor valor vai ser para qual posicao o pacman
 * vai, sempre checando SUL, NORTE, OESTE, LESTE.
 * Parametros:
 *      Grafo *g : ponteiro para o grafo.
 *      int x : posicao em X em que o pacman se encontra.
 *      int y : posicao em Y em que o pacman se encontra.
 * Retorno :
 *      O pacman SEMPRE vai se movimentar entao nao ha retorno na funcao.
*/
void moverP(Grafo *g, int x, int y) {
    int menor = g->grafo[x][y].valor;  // Variavel para guardar o menor vizinho dentre os 4

    // Pocicao para qual o pacman ira'
    int posX = 0;
    int posY = 0;


    // Testa 4-vizinhanca para achar o menor valor

        // Vizinho de BAIXO
    if(x+1 >= 0 && x+1 < g->dimensao) {
        if(g->grafo[x+1][y].valor < menor) {
            menor = g->grafo[x+1][y].valor;
            posX = x+1;
            posY= y;
        }
    }

        // Vizinho de CIMA
    if(x-1 >= 0 && x-1 < g->dimensao) {
        if(g->grafo[x-1][y].valor < menor) {
            menor = g->grafo[x-1][y].valor;
            posX = x-1;
            posY= y;
        }
    }

        // Vizinho da DIREITA
    if(y+1 >= 0 && y+1 < g->dimensao) {
        if(g->grafo[x][y+1].valor < menor) {
            menor = g->grafo[x][y+1].valor;
            posX = x;
            posY= y+1;
        }
    }

        // Vizinho da ESQUERDA
    if(y-1 >= 0 && y-1 < g->dimensao) {
        if(g->grafo[x][y-1].valor < menor) {
            menor = g->grafo[x][y-1].valor;
            posX = x;
            posY= y-1;
        }
    }

    // Move o pacman para o vizinho de menor valor
    g->pacman.x = posX;
    g->pacman.y = posY;

    return;
}

/*
 * A funcao moverPacman() eh a que eh chamada na main para atualizar a posicao do pacman
 * para uma das 4-vizinhancas. Primeiro eh aplicado o algoritmo de WaveFront no mapa,
 * levando em consideracao a posicao do fantasma, depois disso eh decidido para qual posicao
 * o pacman ira.
 * Parametros:
 *      Grafo *g : ponteiro para o grado, este ponteiro ja armazena a posicao atual do pacman
 *                 e a posicao atual do fantasma.
 * Retorno :
 *      O pacman sempre vai se locomover, portanto nao ha retorno.
*/
void moverPacman(Grafo *g) {
    waveFront(g, g->fantasma.x, g->fantasma.y, 0);
    moverP(g, g->pacman.x, g->pacman.y);
    if((g->fantasma.x == g->pacman.x) && (g->fantasma.y == g->pacman.y)) {
        g->status = 1;
    }
}


// Funcao auxiliar que passa todos os valores do mapa para -1
// Ela eh utilizada para aplicar o algoritmo de WaveFront depois de movimentar o fantasma.
void zeraMapa(Grafo *g) {
    for(int i = 0; i < g->dimensao; i++) {
        for(int j = 0; j < g->dimensao; j++) {
            g->grafo[i][j].valor = -1;
        }
    }
}

/*
 * A funcao moverF() eh uma funcao auxiliar e eh responsavel por mover o fantasma para uma posicao
 * que eh passada por parametro, sempre verificando se a posicao passada nao eh invalida(tenta acessar uma aresta
 * que nao existe).
 * Parametros:
 *      Grafo *g : ponteiro para o grafo.
 *      int x : posicao do vertice em X;
 *      int y : posicao do vertice em Y;
 * Retorno:
 *      Retorna 0 quando nao possui arestas que levem ao vertice.
 *      Retorna 1 quando consegue chegar no vertice.
*/
int moverF(Grafo *g, int x, int y) {
    int posX;
    int posY;

    // Checa possibilidade de nao se mover pra um vertice
    if(x >= 0 && x < g->dimensao) {
        posX = x;
    } else {
        return 0;
    }
    if(y >= 0 && y < g->dimensao){
        posY = y;
    } else {
        return 0;
    }

    //Move para o vertice passado
    g->fantasma.x = posX;
    g->fantasma.y = posY;
    return 1;
}

/*
 * A funcao moverFantasma() eh a que eh chamada na main para movimentar o fantasma, dado que o ponteiro para o
 * grafo ja contem a posicao atual dele. Leva em consideracao a posibilidade de andar ou nao, para isso
 * eh gerado um aleatorio de 0 a 100 e se ele estiver :
 *          entre ]0 e 20]: anda para cima.
 *          entre ]20 e 40]: anda para  baixo.
 *          entre ]40 e 60]: anda para direita.
 *          entre ]60 e 80]: anda para esquerda.
 *          entre ]80 e 100]: NAO ANDA.
 * Caso o vertice que ele ira se locomover nao exista(nao possui aresta) o aleatorio eh recalculado
 * e ele testa denovo as posicao ate dar certo.
 * Parametros:
 *          Grafo *g: ponteiro para o grafo.
 * Retorno:
 *          Retorna 0 quando ele nao se movimenta.
 *          Retorna 1 quando ele se movimenta.
*/
int moverFantasma(Grafo *g) {
    srand((uint)time(NULL));

    // Flag para saber se o fantasma conseguiu ir para um vertice ou nao
    int movimentou = 0;

    // Posicao atual do fantasma
    int fantasmaX = g->fantasma.x;
    int fantasmaY = g->fantasma.y;

    // Tenta acessar um vertice
    while(movimentou == 0) {
        // Movimenta o fantasma
        int random = rand()%101;

            // Para cima
        if(random > 0 && random <= 20) {
            if(moverF(g, fantasmaX-1, fantasmaY)) {
                g->grafo[fantasmaX][fantasmaY].valor = 1;
                g->grafo[fantasmaX-1][fantasmaY].valor = 0;
                movimentou = 1;
            }
        }
            // Para baixo
        else if(random > 20 && random <= 40) {
            if(moverF(g, fantasmaX+1, fantasmaY)) {
                g->grafo[fantasmaX][fantasmaY].valor = 1;
                g->grafo[fantasmaX+1][fantasmaY].valor = 0;
                movimentou = 1;
            }

            // Para direita
        else if(random > 60 && random <= 80) {
            if(moverF(g, fantasmaX, fantasmaY+1)) {
                g->grafo[fantasmaX][fantasmaY].valor = 1;
                g->grafo[fantasmaX][fantasmaY+1].valor = 0;
                movimentou = 1;
            }
        }

        }   // Para esquerda
        else if(random > 40 && random <= 60) {
            if(moverF(g, fantasmaX, fantasmaY-1)) {
                g->grafo[fantasmaX][fantasmaY].valor = 1;
                g->grafo[fantasmaX][fantasmaY-1].valor = 0;
                movimentou = 1;
            }
        }

            // Fica parado
        else {
            return 0;
        }
    }

    // Atualiza os valores dos certices, dado que o fantasma se movimentou;
    zeraMapa(g);
    waveFront(g, g->fantasma.x, g->fantasma.y, 0);

    // Se o fantasma se movimentar para uma casa que esteja o pacman ele GANHA(status = -1)
    if((g->fantasma.x == g->pacman.x) && (g->fantasma.y == g->pacman.y)) {
        g->status = -1;
    }
    return 1;
}


// Retorna o atributo status
int getStatus(Grafo *g) {
    return g->status;
}


/*
 * A funcao imprimeGrafo() eh responsavel por imprimir o mapa do jogo, levando em consideracao
 * a posicao do pacman e do fantasma. O pacman eh representado pelo caracter '@' e o fantasma pelo
 * caracter '$'.
 * Parametros:
 *      Grafo *g : ponteiro para o grafo.
 * Retorno:
 *      Nao ha.
*/
void imprimeGrafo(Grafo *g) {
    int tam = g->dimensao;

    for(int i = 0; i < tam; i++ ) {
        printf("+------");
    } printf("+\n");

    for(int i = 0; i < tam; i++) {
        for(int j = 0; j < tam; j++) {
            // Posicao do pacman
            if(i == g->pacman.x && j == g->pacman.y) {
                printf("|  @   ");
            }
            // Posicao do fantasma
            else if(i == g->fantasma.x && j == g->fantasma.y) {
                printf("|  $   ");
            // Posicao vazia
            } else {
                printf("|      ");
            }
        }
        printf("|\n");
        for(int i = 0; i < tam; i++ ) {
            printf("+------");
        } printf("+\n");
    }
}

/*
 * A funcao imprimeGrafoValorado() eh responsavel por imprimir o mapa do jogo, levando em consideracao
 * a posicao do pacman e do fantasma. O pacman eh representado pelo caracter '@' e o fantasma pelo
 * caracter '$'. Imprime cada posicao com o valor dela, levando em consideracao o algoritmo de WaveFront
 * aplicaddo.
 * Parametros:
 *      Grafo *g : ponteiro para o grafo.
 * Retorno:
 *      Nao ha.
*/
void imprimeGrafoValorado(Grafo *g) {
    int tam = g->dimensao;

    waveFront(g, g->fantasma.x, g->fantasma.y, 0);

    for(int i = 0; i < tam; i++ ) {
        printf("+-------");
    } printf("+\n");
    for(int i = 0; i < tam; i++) {
        for(int j = 0; j < tam; j++) {
            // Posicao do pacman
            if(i == g->pacman.x && j == g->pacman.y) {
                printf("|   @   ");
            }
            // Posicao do fantasma
            else if(i == g->fantasma.x && j == g->fantasma.y) {
                printf("|   $   ");
            }
            // Posicao vazia, printa o valor dela.
            else if(g->grafo[i][j].valor/10 >= 1 ){ // Numero com duas casas, para manter formato do mapa
                printf("|  %d   ", g->grafo[i][j].valor);
            } else {
                printf("|   %d   ", g->grafo[i][j].valor);
            }
        }
        printf("|\n");
        for(int i = 0; i < tam; i++ ) {
            printf("+-------");
        } printf("+\n");
    }
}

/*
 * A funcao destroiGrafo() eh responsavel por desalocar a estrutura do grafo da memoria
 * desalocando primeiro seus vertices.
 * Parametros:
 *      Grafo **g : ponteiro para o endereco de memoria do grafo.
 * Retorno:
 *      Nao ha.
*/
void destroiGrafo(Grafo **g) {
    // Desaloca os vertices
    for(int i = 0; i < (*g)->dimensao; i++) {
        free((*g)->grafo[i]);
    }
    free((*g)->grafo);

    // Desaloca estrutura do grafo
    free(*g);
    *g = NULL;
    return;
}










