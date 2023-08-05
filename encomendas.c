#include "encomendas.h"
#include "utils.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

void imprime_encomenda(TEncomenda *encomenda) {

  printf("**************** ENCOMENDA **********************\n");
  printf("CODIGO: ");
  printf("%d\n", encomenda->id);

  printf("TIPO (pertencente): ");
  printf("%s\n", encomenda->tipo);

  printf("CPF (pertencente): ");
  printf("%s\n", encomenda->cpf);

  printf("CNPJ (pertencente): ");
  printf("%s\n", encomenda->cnpj);

  printf("DATA: ");
  printf("%s\n", encomenda->data);

  printf("QUANTIDADE PRE-DEFINIDA: ");
  printf("%s\n", encomenda->quantidade);
}

TEncomenda *encomenda(int id, char *tipo, char *cnpj, char *cpf, char *data,
                      char *qtde) {
  TEncomenda *encomenda = (TEncomenda *)malloc(sizeof(TEncomenda));

  if (encomenda)
    memset(encomenda, 0, sizeof(TEncomenda));

  encomenda->id = id;
  strcpy(encomenda->tipo, tipo);
  strcpy(encomenda->cpf, cpf);
  strcpy(encomenda->cnpj, cnpj);
  strcpy(encomenda->data, data);
  strcpy(encomenda->quantidade, qtde);

  return encomenda;
}

void salva_encomenda(TEncomenda *encomenda, FILE *file) {
  fwrite(&encomenda->id, sizeof(int), 1, file);
  fwrite(encomenda->tipo, sizeof(char), sizeof(encomenda->tipo), file);
  fwrite(encomenda->cpf, sizeof(char), sizeof(encomenda->cpf), file);
  fwrite(encomenda->cnpj, sizeof(char), sizeof(encomenda->cnpj), file);
  fwrite(encomenda->data, sizeof(char), sizeof(encomenda->data), file);
  fwrite(encomenda->quantidade, sizeof(char), sizeof(encomenda->quantidade),
         file);
}

TEncomenda *le_encomenda(FILE *file) {
  TEncomenda *encomenda = (TEncomenda *)malloc(sizeof(TEncomenda));
  if (0 >= fread(&encomenda->id, sizeof(int), 1, file)) {
    free(encomenda);
    return NULL;
  }

  fread(encomenda->tipo, sizeof(char), sizeof(encomenda->tipo), file);
  fread(encomenda->cpf, sizeof(char), sizeof(encomenda->cpf), file);
  fread(encomenda->cnpj, sizeof(char), sizeof(encomenda->cnpj), file);
  fread(encomenda->data, sizeof(char), sizeof(encomenda->data), file);
  fread(encomenda->quantidade, sizeof(char), sizeof(encomenda->quantidade),
        file);
  return encomenda;
}

void le_encomendas(FILE *in) {
  printf("\n\nLendo encomendas do arquivo...\n\n");
  rewind(in);
  TEncomenda encomenda;

  while (fread(&encomenda, sizeof(TEncomenda), 1, in) == 1) {
    imprime_encomenda(&encomenda);
  }
}

int tamanho_encomenda() {
  return sizeof(int)          // cod
         + sizeof(char) * 20  // tipo
         + sizeof(char) * 15  // cpf
         + sizeof(char) * 19  // cnpj
         + sizeof(char) * 11; // data
  +sizeof(char) * 50;         // qtde
}

int tamanho_arquivo_encomenda(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro_encomenda());
  return tam;
}

int tamanho_registro_encomenda() { return sizeof(TEncomenda); }

void initializeBaseDesorder_encomenda(FILE *file, int numberRecords) {
  int f[numberRecords];

  for (int i = 0; i < numberRecords; i++) {
    f[i] = i + 1;
  }
  embaralhar(f, numberRecords);
  fseek(file, 0, SEEK_SET);

  for (int i = 0; i < numberRecords; i++) {
    TEncomenda encomenda;
    encomenda.id = f[i];
    sprintf(encomenda.tipo, "Cliente");
    sprintf(encomenda.cpf, "111.111.111-11");
    sprintf(encomenda.cnpj, "11.111.111/0001-11");
    sprintf(encomenda.data, "01/01/2000");
    sprintf(encomenda.quantidade, "%d", (i + 1) * 2);
    fflush(file);
    fseek(file, (i)*tamanho_registro_encomenda(), SEEK_SET);
    salva_encomenda(&encomenda, file);
    fflush(file);
  }
}

TEncomenda busca_sequencial_encomenda(int cod, FILE *arq) {
  double tempoTotal = 0;
  int comp = 0;
  int i = 0;
  clock_t inicio = clock();

  rewind(arq);
  TEncomenda encomenda;

  while (fread(&encomenda, sizeof(TEncomenda), 1, arq) == 1) {
    if (cod == encomenda.id) {
      clock_t fim = clock();
      tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return encomenda;
      break;
    } else {
      comp++;
    }
  }

  printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
  printf("\nComparacoes = %d\n", comp);

  return encomenda;
}

TEncomenda busca_binaria_encomenda(int cod, FILE *arq, int tam) {
  int left = 0, right = tam - 1, comp = 0;
  double tempoTotal = 0;

  TEncomenda encomenda;

  rewind(arq);
  clock_t inicio = clock();

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_encomenda(), SEEK_SET);
    fread(&encomenda, sizeof(TEncomenda), 1, arq);
    if (cod == encomenda.id) {
      clock_t fim = clock();
      tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return encomenda;
    } else if (encomenda.id < cod) {
      left = middle + 1;
      comp++;
    } else {
      right = middle - 1;
      comp++;
    }
  }

  printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
  printf("\nComparacoes = %d\n", comp);
  return encomenda;
}

void insertion_sort_disco_encomenda(FILE *arq, int tam) {
  int i;
  for (int j = 2; j <= tam; j++) {
    fseek(arq, (j - 1) * tamanho_registro_encomenda(), SEEK_SET);
    TEncomenda *fj = le_encomenda(arq);
    i = j - 1;
    fseek(arq, (i - 1) * tamanho_registro_encomenda(), SEEK_SET);
    TEncomenda *fi = le_encomenda(arq);
    while ((i > 0) && (fi->id > fj->id)) {
      fseek(arq, i * tamanho_registro_encomenda(), SEEK_SET);
      salva_encomenda(fi, arq);
      i = i - 1;
      fseek(arq, (i - 1) * tamanho_registro_encomenda(), SEEK_SET);
      fi = le_encomenda(arq);
    }
    fseek(arq, (i)*tamanho_registro_encomenda(), SEEK_SET);
    salva_encomenda(fj, arq);
  }
  fflush(arq);
}

void classificacaoSubs_encom(FILE *arq) {
    rewind(arq); //posiciona cursor no inicio do arquivo

    int nFunc = tamanho_arquivo_encomenda(arq);
    int qtdParticoes = 0;
    char nome[40];
    char numero[3];
    char extensao[5];
    int tamRes = 5;
    int tamVet = 5;
    TEncomenda *v[tamVet];
    TEncomenda *menor;
    int congela[nFunc];
    int aux = 0, tamPart = 0, posiMenor = 0, proxArq = 5, resIt = 0, auxCong = 0, auxFimArq = 0;
    int i = 0;
    FILE *p;
    TEncomenda forn;

    //Preenche o vetor inicial
    while (i < tamVet){
      fseek(arq, (i) * sizeof(TEncomenda), SEEK_SET);
      v[i] = le_encomenda(arq);
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
            //Cria parti��o
            sprintf(nome, "particoesEncom/particao%d", qtdParticoes);
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
            menor->id = INT_MAX;
            posiMenor = nFunc-1;
            for (int j = 0; j < tamVet; j++) {
                if (v[j]->id < menor->id && congela[j] == -1 && v[j]->id != -1) {
                    menor = v[j];
                    posiMenor = j;
                }
            }

            //salva o menor elemento na parti��o
            fseek(p, (tamPart) * sizeof(TEncomenda), SEEK_SET);
            salva_encomenda(menor, p);
            tamPart++;

            fseek(arq, (proxArq) * sizeof(TEncomenda), SEEK_SET);//pega o proximo elemento


            if(proxArq < nFunc){
                v[posiMenor] = le_encomenda(arq);

                if (v[posiMenor]->id < menor->id){
                    //verifica se � menor e poe no reservatio
                    congela[posiMenor] = posiMenor;
                    auxCong++;
                }
            } else {
                congela[posiMenor] = posiMenor;
                auxCong++;
                v[posiMenor]->id = -1;
            }
            proxArq++;

            if(auxCong == tamVet){
                qtdParticoes++;
            }

        }
        imprime_cod_enc(p);
        fclose(p);
    }
    fclose(p);
}

void imprime_cod_enc(FILE *in){
    printf("\n\nLendo codigo emcomenda da particao...\n");
    rewind(in);
    TEncomenda forn;

    while (fread(&forn, sizeof(TEncomenda), 1, in) == 1) {
        printf("\nEncomenda de codigo %d", forn.id);
    }
}
