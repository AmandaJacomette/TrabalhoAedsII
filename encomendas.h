#ifndef ENCOMENDA_H
#define ENCOMENDA_H

#include "utils.h"
#include "particoes.h"
#include <stdio.h>
#include <stdlib.h>

#define ENCOMENDAS_FILE "encomendas.dat"

typedef struct Encomenda {
  int id;
  char tipo[20];
  char cpf[15];
  char cnpj[19];
  char data[11];
  char quantidade[50];
  // TLista produtos;

} TEncomenda;

typedef struct vetor_enc {
  TEncomenda *en;
  FILE *f;
  TParticao *particao;
  int aux_p;
  int read;

} TVetEncomenda;

void imprime_encomenda(TEncomenda *encomenda);
TEncomenda *encomenda(int id, char *tipo, char *cnpj, char *cpf, char *data,
                      char* qtde);
void salva_encomenda(TEncomenda *encomenda, FILE *file);
TEncomenda *le_encomenda(FILE *in);
void le_encomendas(FILE *in);
int tamanho_encomenda();
int tamanho_arquivo_encomenda(FILE *arq);
int tamanho_registro_encomenda();
void initializeBaseDesorder_encomenda(FILE *file, int numberRecords);
TEncomenda busca_sequencial_encomenda(int cod, FILE *arq);
TEncomenda busca_binaria_encomenda(int cod, FILE *arq, int tam);
void insertion_sort_disco_encomenda(FILE *arq, int tam);
void classificacaoSubs_encs(FILE *arq);
void intercalacao_encomendas(char *nome_arquivo_saida, TNomes *nome_particoes);
void processa_particoes_encomendas(TVetEncomenda **v, TNomes *nome_particoes, int num_p, char *out);
void intercala_particoes_encomendas(TVetEncomenda *particao, TVetEncomenda *particao2, TNomes *nome_particoes, int index);

#endif
