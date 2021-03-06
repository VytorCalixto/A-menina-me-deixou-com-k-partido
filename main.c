#include <stdio.h>
#include <stdlib.h>
#include <graphviz/cgraph.h>
#include "lista.h"
#include "grafo.h"

int main(void) {

  char input[128];
  printf("input: ");
  scanf("%s", input);
  printf("%s\n", input);
  FILE *f = fopen(input, "r");
  if(!f) {
      puts("arquivo de entrada nulo");
      return 1;
  }

  grafo g = le_grafo(f);
  if(!g) {
      puts("g nulo");
      return 1;
  }

  FILE *out = fopen("output", "w");

  escreve_grafo(out, g);
  printf("Cordal? %d\n", cordal(g));

  destroi_grafo(g);

  return agerrors();
}
