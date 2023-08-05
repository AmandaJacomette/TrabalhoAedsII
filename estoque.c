#include "estoque.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

void imprime_estoque(TEstoque *estoque) {
  printf("\n*************** ESTOQUE *****************");
  printf("\nCODIGO: ");
  printf("%d", estoque->cod);
  printf("\nATUAL: ");
  printf("%d", estoque->atual);
  printf("\nCAPACIDADE: ");
  printf("%d", estoque->capacidade);
  printf("\nMINIMO: ");
  printf("%d", estoque->min);
}

TEstoque *estoque(int cod, int atual, int capacidade, int min) {
  TEstoque *estoque = (TEstoque *)malloc(sizeof(TEstoque));

  if (estoque)
    memset(estoque, 0, sizeof(TEstoque));

  estoque->cod = cod;
  estoque->atual = atual;
  estoque->capacidade = capacidade;
  estoque->min = min;

  return estoque;
}

void salva_estoque(TEstoque *estoque, FILE *out) {
  fwrite(&estoque->cod, sizeof(int), 1, out);
  fwrite(&estoque->atual, sizeof(int), 1, out);
  fwrite(&estoque->capacidade, sizeof(int), 1, out);
  fwrite(&estoque->min, sizeof(int), 1, out);
}

TEstoque *le_estoque(FILE *in) {
  TEstoque *estoque = (TEstoque *)malloc(sizeof(TEstoque));

  if (0 >= fread(&estoque->cod, sizeof(int), 1, in)) {
    free(estoque);
    return NULL;
  }
  fread(&estoque->atual, sizeof(int), 1, in);
  fread(&estoque->capacidade, sizeof(int), 1, in);
  fread(&estoque->min, sizeof(int), 1, in);

  return estoque;
}

void le_estoques(FILE *in) {
  printf("\n\nLendo estoques do arquivo...\n\n");
  rewind(in);
  TEstoque estoque;

  while (fread(&estoque, sizeof(TEstoque), 1, in) == 1) {
    imprime_estoque(&estoque);
  }
}

int tamanho_estoque() {
  return sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int);
}

int tamanho_arquivo_estoque(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro_estoque());
  return tam;
}

int tamanho_registro_estoque() { return sizeof(TEstoque); }

void initializeBaseDesorder_estoque(FILE *file, int numberRecords) {

  int f[numberRecords];
  for (int i = 0; i < numberRecords; i++) {
    f[i] = i + 1;
  }
  embaralhar(f, numberRecords);
  fseek(file, 0, SEEK_SET);

  for (int i = 0; i < numberRecords; i++) {
    TEstoque estoque;
    estoque.cod = f[i];
    estoque.atual = i + 25;
    estoque.capacidade = 1000;
    estoque.min = 350;
    fseek(file, (i)*tamanho_registro_estoque(), SEEK_SET);
    salva_estoque(&estoque, file);
  }
}

TEstoque busca_sequencial_estoque(int cod, FILE *arq) {
  double tempoTotal = 0;
  int comp = 0;
  int i = 0;
  clock_t inicio = clock();

  rewind(arq);
  TEstoque estoque;

  while (fread(&estoque, sizeof(TEstoque), 1, arq) == 1) {
    if (cod == estoque.cod) {
      clock_t fim = clock();
      tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return estoque;
      break;
    } else {
      comp++;
    }
  }
  printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
  printf("\nComparacoes = %d\n", comp);
  return estoque;
}

TEstoque busca_binaria_estoque(int cod, FILE *arq, int tam) {
  int left = 0, right = tam - 1, comp = 0;
  double tempoTotal = 0;
  TEstoque estoque;

  rewind(arq);
  clock_t inicio = clock();

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_estoque(), SEEK_SET);
    fread(&estoque, sizeof(TEstoque), 1, arq);
    if (cod == estoque.cod) {
      clock_t fim = clock();
      tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return estoque;
    } else if (estoque.cod < cod) {
      left = middle + 1;
      comp++;
    } else {
      right = middle - 1;
      comp++;
    }
  }

  printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
  printf("\nComparacoes = %d\n", comp);
  return estoque;
}

void insertion_sort_disco_estoque(FILE *arq, int tam) {
  int i;
  for (int j = 2; j <= tam; j++) {
    fseek(arq, (j - 1) * tamanho_registro_estoque(), SEEK_SET);
    TEstoque *fj = le_estoque(arq);
    i = j - 1;
    fseek(arq, (i - 1) * tamanho_registro_estoque(), SEEK_SET);
    TEstoque *fi = le_estoque(arq);
    while ((i > 0) && (fi->cod > fj->cod)) {
      fseek(arq, i * tamanho_registro_estoque(), SEEK_SET);
      salva_estoque(fi, arq);
      i = i - 1;
      fseek(arq, (i - 1) * tamanho_registro_estoque(), SEEK_SET);
      fi = le_estoque(arq);
    }
    fseek(arq, (i)*tamanho_registro_estoque(), SEEK_SET);
    salva_estoque(fj, arq);
  }
  fflush(arq);
}

void classificacaoSubs_estoq(FILE *arq) {
    rewind(arq); //posiciona cursor no inicio do arquivo

    int nFunc = tamanho_arquivo_estoque(arq);
    int qtdParticoes = 0;
    char nome[40];
    char numero[3];
    char extensao[5];
    int tamRes = 5;
    int tamVet = 5;
    TEstoque *v[tamVet];
    TEstoque *menor;
    int congela[nFunc];
    int aux = 0, tamPart = 0, posiMenor = 0, proxArq = 5, resIt = 0, auxCong = 0, auxFimArq = 0;
    int i = 0;
    FILE *p;
    TEstoque forn;

    //Preenche o vetor inicial
    while (i < tamVet){
      fseek(arq, (i) * sizeof(TEstoque), SEEK_SET);
      v[i] = le_estoque(arq);
      i++;
    }

    i = 0;

    while (i < tamVet){
      congela[i] =0;
      i++;
    }
    i = 0;

    while(proxArq < nFunc || aux < nFunc){
        while (i < tamVet){
            if(congela[i] != 0){
                auxCong++;
            }
            i++;
        }
        i = 0;
        if(proxArq == 5 || auxCong != 0){
            //Cria partição
            sprintf(nome, "particoesEstoq/particao%d", qtdParticoes);
            char* fim = ".dat";
            strcat(nome, fim);
            tamPart = 0;

            if ((p = fopen(nome, "wb+")) == NULL) {
                printf("Erro criar arquivo de saida\n");
            }
        }

        auxCong = 0;
        while (i < tamVet){
            congela[i] = -1;
            i++;
        }

        while((auxCong < tamVet && proxArq < nFunc) || (auxCong < tamVet && aux < nFunc)){
            aux++;
            menor->cod = INT_MAX;
            posiMenor = nFunc-1;
            for (int j = 0; j < tamVet; j++) {
                if (v[j]->cod < menor->cod && congela[j] == -1 && v[j]->cod != -1) {
                    menor = v[j];
                    posiMenor = j;
                }
            }

            //salva o menor elemento na partição
            fseek(p, (tamPart) * sizeof(TEstoque), SEEK_SET);
            salva_estoque(menor, p);
            tamPart++;

            fseek(arq, (proxArq) * sizeof(TEstoque), SEEK_SET);//pega o proximo elemento


            if(proxArq < nFunc){
                v[posiMenor] = le_estoque(arq);

                if (v[posiMenor]->cod < menor->cod){
                    //verifica se é menor e poe no reservatio
                    congela[posiMenor] = posiMenor;
                    auxCong++;
                }
            } else {
                congela[posiMenor] = posiMenor;
                auxCong++;
                v[posiMenor]->cod = -1;
            }
            proxArq++;

            if(auxCong == tamVet){
                qtdParticoes++;
            }

        }
        imprime_cod_est(p);
        fclose(p);
    }
    fclose(p);
}

void imprime_cod_est(FILE *in){
    printf("\n\nLendo codigo estoque da particao...\n");
    rewind(in);
    TEstoque forn;

    while (fread(&forn, sizeof(TEstoque), 1, in) == 1) {
        printf("\nEstoque de codigo %d", forn.cod);
    }
}
