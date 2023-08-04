#include "funcionario.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

// Imprime funcionario
void imprime_funcionario(TFunc *func) {
  printf("\n**********************************************");
  printf("\nFuncionario de codigo ");
  printf("%d", func->cod);
  printf("\nNome: ");
  printf("%s", func->nome);
  printf("\nCPF: ");
  printf("%s", func->cpf);
  printf("\nData de Nascimento: ");
  printf("%s", func->data_nascimento);
  printf("\nSalario: ");
  printf("%.2f", func->salario);
  printf("\n**********************************************");
}

// Cria funcionario. Lembrar de usar free(func)
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento,
                   double salario) {
  TFunc *func = (TFunc *)malloc(sizeof(TFunc));
  // inicializa espaço de memória com ZEROS
  if (func)
    memset(func, 0, sizeof(TFunc));
  // copia valores para os campos de func
  func->cod = cod;
  strcpy(func->nome, nome);
  strcpy(func->cpf, cpf);
  strcpy(func->data_nascimento, data_nascimento);
  func->salario = salario;
  return func;
}

// Salva funcionario no arquivo out, na posicao atual do cursor
void salva_funcionario(TFunc *func, FILE *out) {
  fwrite(&func->cod, sizeof(int), 1, out);
  // func->nome ao invés de &func->nome, pois string já é ponteiro
  fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
  fwrite(func->cpf, sizeof(char), sizeof(func->cpf), out);
  fwrite(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento),
         out);
  fwrite(&func->salario, sizeof(double), 1, out);
}

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFunc *le_funcionario(FILE *in) {
  TFunc *func = (TFunc *)malloc(sizeof(TFunc));
  if (0 >= fread(&func->cod, sizeof(int), 1, in)) {
    free(func);
    return NULL;
  }
  fread(func->nome, sizeof(char), sizeof(func->nome), in);
  fread(func->cpf, sizeof(char), sizeof(func->cpf), in);
  fread(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), in);
  fread(&func->salario, sizeof(double), 1, in);
  return func;
}

void le_funcionarios(FILE *in) {
  printf("\n\nLendo funcionarios do arquivo...\n\n");
  rewind(in);
  TFunc *f = (TFunc *)malloc(sizeof(TFunc));
  while ((f = le_funcionario(in)) != NULL) {
    imprime_funcionario(f);
    free(f);
  }
}

// Retorna tamanho do funcionario em bytes
int tamanho_funcionario() {
  return sizeof(int)         // cod
         + sizeof(char) * 50 // nome
         + sizeof(char) * 15 // cpf
         + sizeof(char) * 11 // data_nascimento
         + sizeof(double);   // salario
}

int tamanho_arquivo_funcionario(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro_funcionario());
  return tam;
}

int tamanho_registro_funcionario() { return sizeof(TFunc); }

void initializeBaseDesorder_funcionario(FILE *file, int numberRecords) {
  int f[numberRecords];

  for (int i = 0; i < numberRecords; i++) {
    f[i] = i+1;
  }
  embaralhar(f, numberRecords);
  fseek(file, 0, SEEK_SET);

  for (int i = 0; i < numberRecords; i++) {
    TFunc func;
    func.cod = f[i];
    sprintf(func.data_nascimento, "01/01/2000");
    sprintf(func.cpf, "111.111.111-11");
    sprintf(func.nome, "Funcionario %d", f[i]);
    func.salario = 1000.2 * i;
    fseek(file, (i)* tamanho_registro_funcionario(), SEEK_SET);
    salva_funcionario(&func, file);
  }
}

TFunc *busca_sequencial_funcionario(int cod, FILE *arq) {
  double tempoTotal = 0;
  int comp = 0;
  int i = 0;

  clock_t inicio = clock();
  for (i = 0; i < tamanho_arquivo_funcionario(arq); i++) {
    fseek(arq, i * tamanho_funcionario(), SEEK_SET);
    TFunc *func = le_funcionario(arq);
    if (cod == func->cod) {
      clock_t fim = clock();
      tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return func;
      break;
    } else {
      comp++;
    }
  }
}

TFunc *busca_binaria_funcionario(int cod, FILE *arq, int tam) {
  int left = 0, right = tam - 1, comp = 0;
  double tempoTotal = 0;

  clock_t inicio = clock();

  while (left <= right) {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro_funcionario(), SEEK_SET);
    TFunc *func = le_funcionario(arq);
    if (cod == func->cod) {
      clock_t fim = clock();
      tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
      printf("\nTempo da busca sequencial = %f segundos\n", tempoTotal);
      printf("\nComparacoes = %d\n", comp);
      return func;
    } else if (func->cod < cod) {
      left = middle + 1;
      comp++;
    } else {
      right = middle - 1;
      comp++;
    }
  }

  return NULL;
}

void insertion_sort_disco_funcionario(FILE *arq, int tam) {
  int i;
  for (int j = 2; j <= tam; j++) {
    fseek(arq, (j - 1) * tamanho_registro_funcionario(), SEEK_SET);
    TFunc *fj = le_funcionario(arq);
    // printf("\n********* Funcionario atual: %d\n", fj->cod);
    i = j - 1;
    fseek(arq, (i - 1) * tamanho_registro_funcionario(), SEEK_SET);
    TFunc *fi = le_funcionario(arq);
    // printf("fi = %d\n", fi->cod);
    while ((i > 0) && (fi->cod > fj->cod)) {
      fseek(arq, i * tamanho_registro_funcionario(), SEEK_SET);
      // printf("Salvando funcionario %d na posicao %d\n", fi->cod, i+1);
      salva_funcionario(fi, arq);
      i = i - 1;
      fseek(arq, (i - 1) * tamanho_registro_funcionario(), SEEK_SET);
      fi = le_funcionario(arq);
      // printf("fi = %d; i = %d\n", fi->cod, i);
    }
    fseek(arq, (i)*tamanho_registro_funcionario(), SEEK_SET);
    // printf("*** Salvando funcionario %d na posicao %d\n", fj->cod, i+1);
    salva_funcionario(fj, arq);
  }
  fflush(arq);
}

void classificacaoSubs_func(FILE *arq) {
    rewind(arq); //posiciona cursor no inicio do arquivo

    int reg = 0;
    int nFunc = tamanho_arquivo_funcionario(arq);
    int qtdParticoes = 0;
    int t = tamanho_funcionario();
    char nome[40];
    char numero[3];
    char extensao[5];
    int tamRes = 5;
    int tamVet = 5;
    TFunc *v[tamVet];
    TFunc *reservatorio[tamRes];
    TFunc *menor;
    int congela[nFunc];
    TFunc *f;
    int aux = 0, tamPart = 0, posiMenor = 0, proxArq = 5, resIt = 0, auxCong = 0, auxFimArq = 0;
    int i = 0;
    FILE *p;

    //Preenche o vetor inicial
    while (i < tamVet){
      fseek(arq, (i) * tamanho_funcionario(), SEEK_SET);
      v[i] = le_funcionario(arq);
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
            sprintf(nome, "particoesFunc/particao%d", qtdParticoes);
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
            fseek(p, (tamPart) * tamanho_funcionario(), SEEK_SET);
            salva_funcionario(menor, p);
            tamPart++;

            fseek(arq, (proxArq) * tamanho_funcionario(), SEEK_SET);//pega o proximo elemento


            if(proxArq < nFunc){
                v[posiMenor] = le_funcionario(arq);

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
                fclose(p);
                qtdParticoes++;
            }

        }
        fclose(p);
    }
    fclose(p);
}
