#include "prateleira.h"
#include "produto.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void imprime_prateleira(TPrat *prat) {
  printf("\n************* PRATELEIRA ***************");
  printf("\nCodigo ");
  printf("%d", prat->cod);
  printf("\nQuantidade de produtos atual: ");
  printf("%d", prat->quantAtual);
  printf("\nQuantidade de produtos maxima: ");
  printf("%d", prat->quantMax);
  printf("\nProduto na prateleira:\n ");
  imprime_produto(&prat->p);
}
TPrat *prateleira(int cod, int quantAtual, int quantMax, TProd p) {
  TPrat *prat = (TPrat *)malloc(sizeof(TPrat));
  if (prat)
    memset(prat, 0, sizeof(TPrat));
  prat->cod = cod;
  prat->quantAtual = quantAtual;
  prat->quantMax = quantMax;
  prat->p = p;
  return prat;
}

void salva_prateleira(TPrat *prat, FILE *out) {
  fwrite(&prat->cod, sizeof(int), 1, out);
  fwrite(&prat->quantAtual, sizeof(int), 1, out);
  fwrite(&prat->quantMax, sizeof(int), 1, out);
  fwrite(&prat->p.cod, sizeof(int), 1, out);
  fwrite(prat->p.nome, sizeof(char), sizeof(prat->p.nome), out);
  fwrite(prat->p.valor, sizeof(char), sizeof(prat->p.valor), out);
  // salva_produto(&prat->p, out);
}

TPrat *le_prateleira(FILE *in) {
  TPrat *prat = (TPrat *)malloc(sizeof(TPrat));
  if (0 >= fread(&prat->cod, sizeof(int), 1, in)) {
    free(prat);
    return NULL;
  }
  fread(&prat->quantAtual, sizeof(int), 1, in);
  fread(&prat->quantMax, sizeof(int), 1, in);
  fread(&prat->p, sizeof(TProd), 1, in);
  return prat;
}

void le_prateleiras(FILE *in) {
  printf("\n\nLendo prateleiras do arquivo...\n\n");
  rewind(in);
  TPrat prat;

  while (fread(&prat, sizeof(TPrat), 1, in) == 1) {
    imprime_prateleira(&prat);
  }
}

int tamanho_prateleira() {
  return sizeof(int)                    // cod
         + sizeof(int)                  // nome
         + sizeof(int) + sizeof(TProd); // valor
}

int tamanho_arquivo_prateleira(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro_prateleira());
  return tam;
}

int tamanho_registro_prateleira() { return sizeof(TPrat); }

void initializeBaseDesorder_prateleira(FILE *file, int numberRecords) {
  int f[numberRecords];
  for (int i = 0; i < numberRecords; i++) {
    f[i] = i + 1;
  }
  embaralhar(f, numberRecords);
  fseek(file, 0, SEEK_SET);

  for (int i = 0; i < numberRecords; i++) {
    TPrat prat;
    TProd *prod;
    prat.cod = f[i];
    prat.quantAtual = 25;
    prat.quantMax = 50;
    prod = produto(66, "Produto 66", "12.50");
    prat.p = *prod;
    fseek(file, (i)*tamanho_registro_prateleira(), SEEK_SET);
    salva_prateleira(&prat, file);
  }
}

TPrat busca_sequencial_prateleira(int cod, FILE *arq) {
  double tempoTotal = 0;
  int comp = 0;
  int i = 0;
  clock_t inicio = clock();

  rewind(arq);
  TPrat prat;

  while (fread(&prat, sizeof(TPrat), 1, arq) == 1) {
    if (cod == prat.cod) {
      clock_t fim = clock();
      tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return prat;
      break;
    } else {
      comp++;
    }
  }

  return prat;
}

TPrat busca_binaria_prateleira(int cod, FILE *arq, int tam) {
  int left = 0, right = tam - 1, comp = 0;
  double tempoTotal = 0;
  TPrat prat;

  rewind(arq);
  clock_t inicio = clock();

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_prateleira(), SEEK_SET);
    fread(&prat, sizeof(TPrat), 1, arq);
    if (cod == prat.cod) {
      clock_t fim = clock();
      tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return prat;
    } else if (prat.cod < cod) {
      left = middle + 1;
      comp++;
    } else {
      right = middle - 1;
      comp++;
    }
  }

  return prat;
}

void insertion_sort_disco_prateleira(FILE *arq, int tam) {
  int i;
  for (int j = 2; j <= tam; j++) {
    fseek(arq, (j - 1) * tamanho_registro_prateleira(), SEEK_SET);
    TPrat *cli = le_prateleira(arq);
    // printf("\n********* Funcionario atual: %d\n", fj->cod);
    i = j - 1;
    fseek(arq, (i - 1) * tamanho_registro_prateleira(), SEEK_SET);
    TPrat *cli1 = le_prateleira(arq);
    // printf("fi = %d\n", fi->cod);
    while ((i > 0) && (cli1->cod > cli->cod)) {
      fseek(arq, i * tamanho_registro_prateleira(), SEEK_SET);
      // printf("Salvando funcionario %d na posicao %d\n", fi->cod, i+1);
      salva_prateleira(cli1, arq);
      i = i - 1;
      fseek(arq, (i - 1) * tamanho_registro_prateleira(), SEEK_SET);
      cli1 = le_prateleira(arq);
      // printf("fi = %d; i = %d\n", fi->cod, i);
    }
    fseek(arq, (i)*tamanho_registro_prateleira(), SEEK_SET);
    // printf("*** Salvando funcionario %d na posicao %d\n", fj->cod, i+1);
    salva_prateleira(cli, arq);
  }
  fflush(arq);
}
