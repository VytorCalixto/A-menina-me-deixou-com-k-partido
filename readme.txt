Trabalho de Implementação 1
Algoritmo e Teoria dos Grafos
Prof. Renato Carmo

Alunos:
    Victor Luis Perszel - GRR20144615
    Vytor dos Santos Bezerra Calixto - GRR20144464

A struct grafo contém:
    - int direcionado;  // 1 se direcionado, 0 se não
    - int ponderado;    // 1 se ponderado, 0 se não
    - char* nome;       // O nome do grafo
    - lista vertices;   // Lista contendo todos os vérices do grafo

A struct vertice contém:
    - char* nome;           // O nome do vértice
    - unsigned int grau;    // Grau para grafos não-direcionados
    - unsigned int grau_ent;// Grau de entrada para grafos direcionados
    - unsigned int grau_sai;// Grau de saída para grafos direcionados
    - lista arestas;        // A lista de todas as arestas que SAEM do vértice

Além dessas structs, criamos a struct aresta, que contém:
    - long int peso;    // O peso da aresta, caso seja ponderada (0 caso contrário)
    - vertice *destino; // Para qual vértice a aresta está apontando

Importante: NÃO duplicamos as arestas; Se uma aresta é 'a -- c', por exemplo, ela estará somente na lista de arestas de 'a' e não de 'c'.
Continuando no exemplo, para verificar, então, se existe uma aresta de 'c' para 'a', devemos passar por todo o grafo, porém, com a vantagem de não precisarmos guardar os valores duas vezes.

Uma função extra foi colocada em lista.c, a "pertence".
Ela recebe como parâmetros uma função de comparação (void*, void*), um conteudo (void*) e uma lista.
A ideia é verificar se um conteúdo qualquer está em um nodo da lista.
A função faz isso passando por todos os nodos do grafo e usando a função passada para comparar o conteúdo do nodo com o conteúdo fornecido.
A "pertence" é utilizada na verificação de clique, no qual comparamos se cada nodo está na lista de seus vizinhos.

---------------- Explicação de como cada função foi implementada ----------------

nome_vertice, nome_grafo, direcionado e ponderado só acessam diretamente a struct e retornam o pedido.

n_vertices passa por todos os nós incrementando uma soma, retornando-a;

n_arestas passa pela lista de arestas de cada um dos vertices, incrementando uma soma, retornando-a;

le_grafo "malloca" uma struct grafo, utilizando-se do libcgraph para construir primeiramente a lista de vertices. Logo depois passamos por cada um dos vértices, verificando questões de ponderamento e indicando o destino de cada uma das arestas;

destroi_aresta é uma função nova que apenas desaloca uma aresta da memória;

destroi_vertice primeiro destroi a lista de arestas do vértice e depois desaloca o vértice;

destroi_referencias é utilizada na função cordal para apagar referências a um vértice. A função percorre o grafo removendo todas as arestas cujo destino é o vértice informado;

destroi_grafo destroi a lista de vértices (utilizando destroi_lista e destroi_vertice) e depois desaloca o grafo;

escreve_grafo escreve no arquivo passado como parâmetro o grafo como especificado;

copia_grafo começa copiando os vértices do grafo e depois "monta" as listas de aresta de cada vértice;

vizinhanca passa primeiro por todos os vértices procurando o vértice que se deseja saber a vizinhança (que chamarei aqui de v, para ser mais fácil de explicar). Depois analiza-se o valor de direção, se for 0 (não-direcionado), ele passa pela lista de arestas de TODOS os nos do grafo. No caso, temos duas opções para ser vizinho: caso o nodo que estamos é o pŕoprio v, todos os destinos das suas arestas são seu vizinhos. Ou, caso estejamos em outro vértice, mas o destino da aresta desse vértice é v, este vértice também é vizinho. Caso direção seja 1, só precisamos achar v e indicar todas suas arestas (lembrando que o vértice não grava as arestas que chegam nele). Caso seja -1, passamos pelas arestas de TODOS os vertices, só que dessa vez só inserindo aquelas cujo destino é v; 

grau apenas encontra o vértice que gostaríamos e retorna grau, grau_ent ou grau_sai, conforme especificado acima;

compara_vertice compara se dois vértices são iguais;

clique monta a vizinhança de cada vértice na lista l e compara se os vértices na lista l são vizinhos;

simplicial monta a vizinhança do vértice e ve se essa vizinhança é clique;

cordal ve se um vértice é simplicial, se sim, excluimos ele até sobrar um só (é cordal). Quando o vértice não é simplicial passamos para o próximo no grafo. Caso cheguemos ao fim do grafo, assumimos que ele não é cordal. Isso ocorre quando adentramos uma sequência de vértices que continham vértices simpliciais;

Foi utilizada a ferramenta valgrind para checar por vazamentos de memória e corrigidos todos menos três indicados pelo valgrind: todos eles envolvendo a liberação de memória causada por listas. Nas funções de clique e simplicial são criadas listas para facilitar a abstração do algoritmo, entretanto estas listas tem como conteúdo em seus nó ponteiros para vértices, ponteiros estes que apontam para o mesmo lugar que os ponteiros do grafo. Logo, ao desalocar a lista, acabávamos desalocando também vértices pertencentes ao grafo, o que causava falha de segmentação, já que outras partes do código acabavam tentando acessar uma posição que já havia sido liberada na memória.
