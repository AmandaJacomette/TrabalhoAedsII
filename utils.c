#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils.h"

void embaralhar(int *vet, size_t vetSize) {
  time_t t;
  srand((unsigned)time(&t));

  if (vetSize > 1) {
    size_t i;
    for (i = 0; i < vetSize - 1; i++) {
      size_t j = i + rand() / (RAND_MAX / (vetSize - i) + 1);
      int num = vet[j];
      vet[j] = vet[i];
      vet[i] = num;
    }
  }
}

void adicionar_tempo_comparacoes_sequencial(char *tipoProcurado,
                                            double tempoTotal, int comp) {
  FILE *file;
  file = fopen("busca_comparacoes.dat", "a");

  if (file == NULL) {
    printf("\nERRO: Nao foi possivel abrir o arquivo busca_comparacoes.dat");
  }

  char *head;
  char divisao1[] = "***************** ";
  char divisao2[] = " *****************";
  char *tempo, *comparacoes;

  strcpy(head, divisao1);
  strcpy(head, tipoProcurado);
  strcpy(head, divisao2);
  sprintf(&tempo, "\nTempo da busca sequencial = %f segundos\n", tempoTotal);
  sprintf(&comparacoes, "\nComparacoes = %d\n", comp);

  fprintf(file, "\n");
  fwrite(head, sizeof(char), sizeof(head), file);
  fwrite(tempo, sizeof(char), sizeof(tempo), file);
  fwrite(comparacoes, sizeof(char), sizeof(comparacoes), file);

  fclose(file);
}

void adicionar_tempo_comparacoes_binario(char *tipoProcurado, double tempoTotal,
                                         int comp) {
  FILE *file;
  file = fopen("busca_comparacoes.dat", "a");

  if (file == NULL) {
    printf("\nERRO: Nao foi possivel abrir o arquivo busca_comparacoes.dat");
  }

  char *head;
  char divisao1[] = "***************** ";
  char divisao2[] = " *****************";
  char *tempo, *comparacoes;

  strcpy(head, divisao1);
  strcpy(head, tipoProcurado);
  strcpy(head, divisao2);
  sprintf(&tempo, "\nTempo da busca binaria = %f segundos\n", tempoTotal);
  sprintf(&comparacoes, "\nComparacoes = %d\n", comp);

  fprintf(file, "\n");
  fwrite(head, sizeof(char), sizeof(head), file);
  fwrite(tempo, sizeof(char), sizeof(tempo), file);
  fwrite(comparacoes, sizeof(char), sizeof(comparacoes), file);

  fclose(file);
}
