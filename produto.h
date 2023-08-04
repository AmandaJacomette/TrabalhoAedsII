#ifndef PRODUTO_H_INCLUDED
#define PRODUTO_H_INCLUDED

#define PRODUTOS_FILE "produtos.dat"

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "particoes.h"

typedef struct Produto {
  int cod;
  char nome[50];
  char valor[50];
} TProd;

typedef struct vetor_prod {
  TProd *en;
  FILE *f;
  TParticao *particao;
  int aux_p;
  int read;

} TVetProd;

void imprime_produto(TProd *prod);

TProd *produto(int cod, char *nome, char *valor);

void salva_produto(TProd *prod, FILE *out);

TProd *le_produto(FILE *in);

void le_produtos(FILE *in);

int tamanho_produto();

int tamanho_arquivo_produto(FILE *arq);

int tamanho_registro_produto();

void initializeBaseDesorder_produto(FILE *file, int numberRecords);

TProd busca_sequencial_produto(int cod, FILE *arq);

TProd busca_binaria_produto(int cod, FILE *arq, int tam);

void insertion_sort_disco_produto(FILE *arq, int tam);

void classificacaoSubs_prod(FILE *arq);

void intercalacao_produtos(char *nome_arquivo_saida, TNomes *nome_particoes);

void processa_particoes_produtos(TVetProd **v, TNomes *nome_particoes, int num_p, char *out);

void intercala_particoes_produtos(TVetProd *particao, TVetProd *particao2, TNomes *nome_particoes, int index);



#endif // PRODUTO_H_INCLUDED
