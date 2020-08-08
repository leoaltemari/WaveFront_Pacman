typedef struct v Vertice;
typedef struct g Grafo;
typedef struct p Posicao;

int criaGrafo(Grafo **g, int dimensao);
void imprimeGrafo(Grafo *g);
void imprimeGrafoValorado(Grafo *g);
int adicionaPacman(Grafo *g, int x, int y);
int adicionaFantasma(Grafo *g, int x, int y);
void moverPacman(Grafo *g);
int moverFantasma(Grafo *g);
int getStatus(Grafo *g);
void destroiGrafo(Grafo **g);
