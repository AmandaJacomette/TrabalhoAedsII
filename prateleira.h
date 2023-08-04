#ifndef PRATELEIRA_H_INCLUDED
#define PRATELEIRA_H_INCLUDED

#define PRATELEIRA_FILE "prateleiras.dat"

#include <stdio.h>
#include <stdlib.h>

#include "produto.h"
#include "utils.h"
#include "particoes.h"

typedef struct Prateleira {
  int cod;
  int quantAtual;
  int quantMax;
  TProd p;
} TPrat;

typedef struct vetor_prat {
  TPrat *en;
  FILE *f;
  TParticao *particao;
  int aux_p;
  int read;

} TVetPrat;

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

void classificacaoSubs_prat(FILE *arq);

void intercalacao_prateleiras(char *nome_arquivo_saida, TNomes *nome_particoes);

void processa_particoes_prateleiras(TVetPrat **v, TNomes *nome_particoes, int num_p, char *out);

void intercala_particoes_prateleiras(TVetPrat *particao, TVetPrat *particao2, TNomes *nome_particoes, int index);


#endif // PRODUTO_H_INCLUDED
