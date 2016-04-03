#include <stdio.h>
#include <graphviz/cgraph.h>
#include "grafo.h"

typedef struct grafo {
  int direcionado;
  int ponderado;
  char* nome;
  lista vertices;
  lista arestas;
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
  unsigned int grau; // Para grafos não-direcionados
  unsigned int grau_ent; // Entrada dos direcionados
  unsigned int grau_sai; // Saída dos direcionados

  //TODO: vizinhanca entrada / saida
};

char *nome_vertice(vertice v){
  return v->nome;
}

grafo le_grafo(FILE *input){
    grafo gf;
    Agraph_t *g = agread(input, NULL);
    if(!g) {
        return NULL;
    }
    return g;

  //Pia, to com mto sono enquanto escrevo isso, n me julgue aoshfasfj
  // char buff[255];
  // grafo g;
  // fscanf(input, "%s", buff); // le a primeira palavra
  // if(buff == "digraph") // direcionado?
  //   g->direcionado = 1;
  // else
  //   g->direcionado = 0;
  // fscanf(input, "%s", buff); // le a segunda palavra
  // g->nome = buff; // que é o nome do grafo

  // TODO: Ler vertices, arestas e pesos;
}

int destroi_grafo(void *g){
  free(g);
  g = NULL;
  return 1;
  // N tem como retornar 0 caso ocorra erro
  // Depois de um free ocorre undefined behaviour
  //
  // TODO: Passar por todos os vertices desalocando
}

lista vizinhanca(vertice v, int direcao, grafo g){
  no atual = g->vertices->primeiro;

  while((vertice)conteudo(atual) != v){
    atual = vertice->proximo;
    // TODO: verificar caso o vertice nao esteja no grafo
  }

  switch (direcao) {
    case 0:
      return (vertice)conteudo(atual)->grau;
    break;
    case -1:
      return (vertice)conteudo(atual)->grau_ent;
    break;
    case 1:
      return (vertice)conteudo(atual)->grau_sai;
    break;
    default:
    // TODO: retorno de erro
  }
}
