#include <stdio.h>
#include <graphviz/cgraph.h>
#include "grafo.h"
#include "lista.h"

typedef struct grafo {
  int direcionado;
  int ponderado;
  char* nome;
  lista vertices;
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

  lista arestas;
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

    gf.nome = agnameof(g);
    gf.direcionado = agisdirected(g);
    gf.ponderado = 0;

    // Insere todos os vértices
    for(Agnode_t *n=agfstnode(g, n); n; n=agnxtnode(g, n)) {
        vertice v;
        v.nome = agnameof(n);
        v.grau = (unsigned int) agdegree(g, n, TRUE, TRUE);
        v.grau_ent = (unsigned int) agdegree(g, n, TRUE, FALSE);
        v.grau_sai = (unsigned int) agdegree(g, n, FALSE, TRUE);
        insere_lista(v, gf.vertices);
    }

    // Para cada vértice insere suas arestas
    for(no *n=primeiro_no(gf.vertices); n; n=n->proximo) {
        vertice v = (vertice) conteudo(no);
        Agnote_t node = agnode(g, v->nome, FALSE);
        for(Agedge_t *e=agfstout(g, node); e; e=agnxtedge(g, e)) {
            if(!g.ponderado && agget(a, (char *)"peso") != NULL) {
                h.ponderado = 1;
            }
            insere_lista(e, v.vertices);
        }
    }

    agclose(g);
    return gf;
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
