#include <stdio.h>
#include "grafo.h"

typedef struct grafo {
  int direcionado;
  int ponderado;
  char* nome;
  long int peso_aresta;
};

char *nome_grafo(grafo g){
  return g.nome;
}

int direcionado(grafo g){
  return g.direcionado;
}

int ponderado(grafo g){
  return g.ponderado;
}

typedef struct vertice *vertice{
  char* nome;
}

char *nome_vertice(vertice v){
  return v.nome;
}

int destroi_grafo(void *g){
  free(g);
}
