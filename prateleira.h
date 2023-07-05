#ifndef PRATELEIRA_H_INCLUDED
#define PRATELEIRA_H_INCLUDED

#define PRATELEIRA_FILE "prateleiras.dat"

#include <stdio.h>
#include <stdlib.h>

#include "produto.h"
#include "utils.h"

typedef struct Prateleira {
  int cod;
  int quantAtual;
  int quantMax;
  TProd p;
} TPrat;

void imprime_prateleira(TPrat *prat);

TPrat *prateleira(int cod, int quantAtual, int quantMax, TProd p);

void salva_prateleira(TPrat *prat, FILE *out);

TPrat *le_prateleira(FILE *in);

void le_prateleiras(FILE *in);

int tamanho_prateleira();

int tamanho_arquivo_prateleira(FILE *arq);

int tamanho_registro_prateleira();

void initializeBaseDesorder_prateleira(FILE *file, int numberRecords);

TPrat busca_sequencial_prateleira(int cod, FILE *arq);

TPrat busca_binaria_prateleira(int cod, FILE *arq, int tam);

void insertion_sort_disco_prateleira(FILE *arq, int tam);

#endif // PRODUTO_H_INCLUDED
