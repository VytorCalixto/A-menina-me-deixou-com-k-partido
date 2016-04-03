#include <stdio.h>
#include <stdlib.h>
#include <graphviz/cgraph.h>
#include <string.h>
#include "grafo.h"
#include "lista.h"

typedef struct grafo {
    int direcionado;
    int ponderado;
    char* nome;
    lista vertices;
};

typedef struct vertice{
    char* nome;
    unsigned int grau; // Para grafos não-direcionados
    unsigned int grau_ent; // Entrada dos direcionados
    unsigned int grau_sai; // Saída dos direcionados

    lista arestas;
};

typedef struct aresta{
    long int peso;
    vertice *destino;
} *aresta;

char *nome_vertice(vertice v){
    return v->nome;
}

char *nome_grafo(grafo g){
    return g->nome;
}

int direcionado(grafo g){
    return g->direcionado;
}

int ponderado(grafo g){
    return g->ponderado;
}

unsigned int n_vertices(grafo g) {
    unsigned int soma = 0;
    for(no n=primeiro_no(g->vertices); n; n=proximo_no(n)) {
        ++soma;
    }

    return soma;
}

unsigned int n_arestas(grafo g) {
    unsigned int soma = 0;
    for(no n=primeiro_no(g->vertices); n; n=proximo_no(n)) {
        vertice v = (vertice) conteudo(n);
        for(no p=primeiro_no(v->arestas); p; p=proximo_no(p)) {
            ++soma;
        }
    }

    return soma;
}

grafo le_grafo(FILE *input){
    grafo gf = malloc(sizeof(struct grafo));
    Agraph_t *g = agread(input, NULL);
    if(!g) {
        return NULL;
    }

    gf->nome = malloc(1 + strlen(agnameof(g)));
    strcpy(gf->nome, agnameof(g));
    printf("nome: %s\n", gf->nome);
    gf->direcionado = (agisdirected(g) ? 1 : 0);
    gf->ponderado = 0;

    // Insere todos os vértices
    gf->vertices = constroi_lista();
    for(Agnode_t *n=agfstnode(g); n; n=agnxtnode(g, n)) {
        vertice v = malloc(sizeof(struct vertice));
        v->nome = agnameof(n);
        v->grau = (unsigned int) agdegree(g, n, TRUE, TRUE);
        v->grau_ent = (unsigned int) agdegree(g, n, TRUE, FALSE);
        v->grau_sai = (unsigned int) agdegree(g, n, FALSE, TRUE);
        v->arestas = constroi_lista();
        insere_lista(v, gf->vertices);
    }

    // Para cada vértice insere suas arestas
    for(no n=primeiro_no(gf->vertices); n; n=proximo_no(n)) {
        vertice v = (vertice) conteudo(n);
        Agnode_t *node = agnode(g, v->nome, FALSE);
        for(Agedge_t *e=agfstout(g, node); e; e=agnxtout(g, e)) {
            if(!gf->ponderado && agget(e, (char *)"peso") != NULL) {
                gf->ponderado = 1;
            }
            for(no p=primeiro_no(gf->vertices); p; p=proximo_no(p)) {
                vertice w = (vertice) conteudo(p);
                if(strcmp(nome_vertice(w), agnameof(e->node)) == 0) {
                    aresta a = malloc(sizeof(struct aresta));
                    a->peso = (long int) agget(e, (char *)"peso");
                    a->destino = w;
                    insere_lista(a, v->arestas);
                }
            }
        }
    }

    agclose(g);
    return gf;
}

int destroi_grafo(void *g){
    for(no n=primeiro_no(((grafo) g)->vertices); n; n=proximo_no(n)) {
        vertice v = (vertice) conteudo(n);
        //TODO: função para passar
        destroi_lista(v->arestas, NULL);
    }
    //TODO: função
    destroi_lista(((grafo) g)->vertices, NULL);

    free(g);
    // g = NULL;
    return 1;
}

grafo escreve_grafo(FILE *output, grafo g){
    fprintf(output, "strict %s",direcionado(g) ? "digraph " : "graph ");
    fprintf(output, "\"%s\" {\n\n",nome_grafo(g));
    for(no n=primeiro_no(((grafo) g)->vertices); n; n=proximo_no(n)) {
        fprintf(output, "\"%s\"\n",nome_vertice((vertice)conteudo(n)));
    }
    for(no n=primeiro_no(((grafo) g)->vertices); n; n=proximo_no(n)) {
        vertice v = (vertice) conteudo(n);
        for(no p = primeiro_no(v->arestas); p; p=proximo_no(p)) {
            aresta a = (aresta) conteudo(p);
            fprintf(output, "\"%s\"", nome_vertice(v));
            fprintf(output, "%s", direcionado(g) ? " -> " : " -- ");
            fprintf(output, "\"%s\"", nome_vertice(a->destino));
            if(ponderado(g)) fprintf(output, " [peso = %li]", a->peso);
            fprintf(output, "\n");
        }
    }
    fprintf(output, "\n}");
    return g;
}

grafo copia_grafo(grafo g) {
    grafo gf = malloc(sizeof(struct grafo));
    gf->nome = malloc(1 + strlen(g->nome));
    strcpy(gf->nome, g->nome);
    gf->direcionado = g->direcionado;
    gf->ponderado = g->ponderado;
    gf->vertices = constroi_lista();
    //copia os vértices
    for(no n=primeiro_no(g->vertices); n; n=proximo_no(n)) {
        vertice v = (vertice) conteudo(n);
        vertice w = malloc(sizeof(struct vertice));
        w->nome = malloc(1 + strlen(v->nome));
        strcpy(w->nome, v->nome);
        w->grau = v->grau;
        w->grau_ent = v->grau_ent;
        w->grau_sai = v->grau_sai;
        w->arestas = constroi_lista();
        insere_lista(w, gf->vertices);
    }

    for(no n=primeiro_no(gf->vertices); n; n=proximo_no(n)) {
        //vértice atual do grafo de cópia
        vertice v = (vertice) conteudo(n);
        for(no p=primeiro_no(g->vertices); p; p=proximo_no(p)) {
            // vértice do grafo original
            vertice w = (vertice) conteudo(p);
            // se os vértices são os mesmos, começamos a cópia das arestas
            if(strcmp(nome_vertice(w), nome_vertice(v)) == 0) {
                for(no t=primeiro_no(w->arestas); t; t=proximo_no(t)) {
                    aresta a = (aresta) conteudo(p);
                    aresta b = malloc(sizeof(struct aresta));
                    b->peso = a->peso;
                    vertice dest = a->destino;
                    // Precisamos encontrar o destino entre os vértices do grafo
                    //      de cópia
                    for(no r=primeiro_no(gf->vertices); r; r=proximo_no(r)) {
                        vertice y = (vertice) conteudo(r);
                        if(strcmp(nome_vertice(dest), nome_vertice(y)) == 0) {
                            puts("vertices iguais");
                            b->destino = y;
                            insere_lista(b, v->arestas);
                        }
                    }
                }
            }
        }
    }

    return gf;
}

lista vizinhanca(vertice v, int direcao, grafo g){
    no atual = primeiro_no(g->vertices);

    while(((vertice)conteudo(atual) != v) || atual == NULL){
        atual = proximo_no(atual);
    }

    lista l = constroi_lista();

    switch (direcao) {
        case -1:
        case 0:
          for(no p = primeiro_no(((vertice)atual)->arestas); p; p=proximo_no(p)) {
              aresta a = (aresta) conteudo(p);
              insere_lista(a->destino,l);
          }
          return l;
        break;
        case 1:
          for(no n=primeiro_no(((grafo) g)->vertices); n; n=proximo_no(n)) {
              vertice vert = (vertice) conteudo(n);
            for(no p = primeiro_no(vert->arestas); p; p=proximo_no(p)) {
                aresta a = (aresta) conteudo(p);
                if(nome_vertice(*(a->destino)) == nome_vertice(v))
                  insere_lista(a->destino,l);
            }
          }
          return l;
        break;
        default:
        break;
        // TODO: retorno de erro
    }
}

unsigned int grau(vertice v, int direcao, grafo g){
    no atual = primeiro_no(g->vertices);

    while(((vertice)conteudo(atual) != v) || atual == NULL){
        atual = proximo_no(atual);
    }

    switch (direcao) {
        case 0:
          return (((vertice)conteudo(atual))->grau);
        break;
        case -1:
          return (((vertice)conteudo(atual))->grau_ent);
        break;
        case 1:
          return (((vertice)conteudo(atual))->grau_sai);
        break;
        default:
        break;
        // TODO: retorno de erro
    }
}

int compara_vertice(void* a, void* b){
  if ((vertice) a == (vertice) b)
    return 1;
  else
    return 0;
}

int clique(lista l, grafo g) {
    for(no n=primeiro_no(l); n; n=proximo_no(n)) {
        vertice v = (vertice) conteudo(n);
        lista vizinhos = vizinhanca(v, 0, g);
        // Percorremos a lista l. Se todo elemento de l estiver na vizinhança de v,
        // então v é vizinho de todos os vértices em l
        for(no p=primeiro_no(l); p; p=proximo_no(p)) {
            vertice w = (vertice) conteudo(p);
            if(w != v) {
                if(!pertence(*compara_vertice, w, vizinhos)) return 1;
            }
        }
    }
    return 0;
}

int simplicial(vertice v, grafo g){

  lista l = vizinhanca(v,0,g); // Tanto faz se 0 ou 1

  return (clique(l,g));
}
