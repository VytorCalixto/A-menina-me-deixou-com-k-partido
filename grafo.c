#include <stdio.h>
#include <graphviz/cgraph.h>
#include "grafo.h"

typedef struct grafo {
  int direcionado;
  int ponderado;
  char* nome;
  long int peso_aresta;
};

char *nome_grafo(grafo g){
  return g->nome;
}

int direcionado(grafo g){
  return g->direcionado;
}

int ponderado(grafo g){
  return g->ponderado;
}

typedef struct vertice{
  char* nome;
};

char *nome_vertice(vertice v){
  return v->nome;
}

grafo le_grafo(FILE *input){
  //Pia, to com mto sono enquanto escrevo isso, n me julgue aoshfasfj
  char buff[255];
  grafo g;
  fscanf(input, "%s", buff); // le a primeira palavra
  if(buff == "digraph") // direcionado?
    g->direcionado = 1;
  else
    g->direcionado = 0;
  fscanf(input, "%s", buff); // le a segunda palavra
  g->nome = buff; // que é o nome do grafo

  // TODO: Ler vertices, arestas e pesos;
}

int destroi_grafo(void *g){
  free(g);
  g = NULL;
  return 1;
  // N tem como retornar 0 caso ocorra erro
  // Depois de um free ocorre undefined behaviour
}
