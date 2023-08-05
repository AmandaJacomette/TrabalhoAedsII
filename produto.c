#include "produto.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

void imprime_produto(TProd *prod) {
  printf("**********************************************");
  printf("\nProduto de codigo ");
  printf("%d", prod->cod);
  printf("\nNome: ");
  printf("%s", prod->nome);
  printf("\nValor: R$");
  printf("%s", prod->valor);
  printf("\n**********************************************");
}
TProd *produto(int cod, char *nome, char *valor) {
  TProd *prod = (TProd *)malloc(sizeof(TProd));
  if (prod)
    memset(prod, 0, sizeof(TProd));
  prod->cod = cod;
  strcpy(prod->nome, nome);
  strcpy(prod->valor, valor);
  return prod;
}

void salva_produto(TProd *prod, FILE *out) {
  fwrite(&prod->cod, sizeof(int), 1, out);
  fwrite(prod->nome, sizeof(char), sizeof(prod->nome), out);
  fwrite(prod->valor, sizeof(char), sizeof(prod->valor), out);
}

TProd *le_produto(FILE *in) {
  TProd *prod = (TProd *)malloc(sizeof(TProd));
  if (0 >= fread(&prod->cod, sizeof(int), 1, in)) {
    free(prod);
    return NULL;
  }
  fread(prod->nome, sizeof(char), sizeof(prod->nome), in);
  fread(prod->valor, sizeof(char), sizeof(prod->valor), in);
  return prod;
}

void le_produtos(FILE *in) {
  printf("\n\nLendo produtos do arquivo...\n\n");
  rewind(in);
  TProd prod;

  while (fread(&prod, sizeof(TProd), 1, in) == 1) {
    imprime_produto(&prod);
  }
}

int tamanho_produto() {
  return sizeof(int)           // cod
         + (sizeof(char) * 50) // nome
         + sizeof(char) * 50;  // valor
}

int tamanho_arquivo_produto(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro_produto());
  return tam;
}

int tamanho_registro_produto() { return sizeof(TProd); }

void initializeBaseDesorder_produto(FILE *file, int numberRecords) {
  int f[numberRecords];
  for (int i = 0; i < numberRecords; i++) {
    f[i] = i + 1;
  }
  embaralhar(f, numberRecords);
  fseek(file, 0, SEEK_SET);

  for (int i = 0; i < numberRecords; i++) {
    TProd prod;
    prod.cod = f[i];
    sprintf(prod.nome, "Produto %d", f[i]);
    sprintf(prod.valor, "%.2f", (2.5 * (i + 1)));
    fseek(file, (i)*tamanho_registro_produto(), SEEK_SET);
    salva_produto(&prod, file);
  }
}

TProd busca_sequencial_produto(int cod, FILE *arq) {
  double tempoTotal = 0;
  int comp = 0;
  int i = 0;
  clock_t inicio = clock();

  rewind(arq);
  TProd prod;

  while (fread(&prod, sizeof(TProd), 1, arq) == 1) {
    if (cod == prod.cod) {
      clock_t fim = clock();
      tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return prod;
      break;
    } else {
      comp++;
    }
  }

  return prod;
}

TProd busca_binaria_produto(int cod, FILE *arq, int tam) {
  int left = 0, right = tam - 1, comp = 0;
  double tempoTotal = 0;
  TProd prod;

  rewind(arq);
  clock_t inicio = clock();

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_produto(), SEEK_SET);
    fread(&prod, sizeof(TProd), 1, arq);
    if (cod == prod.cod) {
      clock_t fim = clock();
      tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return prod;
    } else if (prod.cod < cod) {
      left = middle + 1;
      comp++;
    } else {
      right = middle - 1;
      comp++;
    }
  }

  return prod;
}

void insertion_sort_disco_produto(FILE *arq, int tam) {
  int i;
  for (int j = 2; j <= tam; j++) {
    fseek(arq, (j - 1) * tamanho_registro_produto(), SEEK_SET);
    TProd *cli = le_produto(arq);
    // printf("\n********* Funcionario atual: %d\n", fj->cod);
    i = j - 1;
    fseek(arq, (i - 1) * tamanho_registro_produto(), SEEK_SET);
    TProd *cli1 = le_produto(arq);
    // printf("fi = %d\n", fi->cod);
    while ((i > 0) && (cli1->cod > cli->cod)) {
      fseek(arq, i * tamanho_registro_produto(), SEEK_SET);
      // printf("Salvando funcionario %d na posicao %d\n", fi->cod, i+1);
      salva_produto(cli1, arq);
      i = i - 1;
      fseek(arq, (i - 1) * tamanho_registro_produto(), SEEK_SET);
      cli1 = le_produto(arq);
      // printf("fi = %d; i = %d\n", fi->cod, i);
    }
    fseek(arq, (i)*tamanho_registro_produto(), SEEK_SET);
    // printf("*** Salvando funcionario %d na posicao %d\n", fj->cod, i+1);
    salva_produto(cli, arq);
  }
  fflush(arq);
}

void classificacaoSubs_prod(FILE *arq) {
    rewind(arq); //posiciona cursor no inicio do arquivo

    int nFunc = tamanho_arquivo_produto(arq);
    int qtdParticoes = 0;
    char nome[40];
    char numero[3];
    char extensao[5];
    int tamRes = 5;
    int tamVet = 5;
    TProd *v[tamVet];
    TProd *menor;
    int congela[nFunc];
    int aux = 0, tamPart = 0, posiMenor = 0, proxArq = 5, resIt = 0, auxCong = 0, auxFimArq = 0;
    int i = 0;
    FILE *p;
    TProd forn;

    //Preenche o vetor inicial
    while (i < tamVet){
      fseek(arq, (i) * sizeof(TProd), SEEK_SET);
      v[i] = le_produto(arq);
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
            sprintf(nome, "particoesProd/particao%d", qtdParticoes);
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
            fseek(p, (tamPart) * sizeof(TProd), SEEK_SET);
            salva_produto(menor, p);
            tamPart++;

            fseek(arq, (proxArq) * sizeof(TProd), SEEK_SET);//pega o proximo elemento


            if(proxArq < nFunc){
                v[posiMenor] = le_produto(arq);

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
        imprime_cod_prod(p);
        fclose(p);
    }
    fclose(p);
}

void imprime_cod_prod(FILE *in){
    printf("\n\nLendo codigo produto da particao...\n");
    rewind(in);
    TProd forn;

    while (fread(&forn, sizeof(TProd), 1, in) == 1) {
        printf("\nProduto de codigo %d", forn.cod);
    }
}

