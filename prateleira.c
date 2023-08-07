#include "prateleira.h"
#include "produto.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

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

void classificacaoSubs_prat(FILE *arq) {
    rewind(arq); //posiciona cursor no inicio do arquivo

    int nFunc = tamanho_arquivo_prateleira(arq);
    int qtdParticoes = 0;
    char nome[40];
    char numero[3];
    char extensao[5];
    int tamRes = 5;
    int tamVet = 5;
    TPrat *v[tamVet];
    TPrat *menor;
    int congela[nFunc];
    int aux = 0, tamPart = 0, posiMenor = 0, proxArq = 5, resIt = 0, auxCong = 0, auxFimArq = 0;
    int i = 0;
    FILE *p;
    TPrat forn;

    //Preenche o vetor inicial
    while (i < tamVet){
      fseek(arq, (i) * sizeof(TPrat), SEEK_SET);
      v[i] = le_prateleira(arq);
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
            sprintf(nome, "particoesPart/particao%d", qtdParticoes);
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
            fseek(p, (tamPart) * sizeof(TPrat), SEEK_SET);
            salva_prateleira(menor, p);
            tamPart++;

            fseek(arq, (proxArq) * sizeof(TPrat), SEEK_SET);//pega o proximo elemento


            if(proxArq < nFunc){
                v[posiMenor] = le_prateleira(arq);

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
        imprime_cod_prat(p);
        fclose(p);
    }
    fclose(p);
}

void imprime_cod_prat(FILE *in){
    printf("\n\nLendo codigo prateleira da particao...\n");
    rewind(in);
    TPrat forn;

    while (fread(&forn, sizeof(TPrat), 1, in) == 1) {
        printf("\nPrateleiras de codigo %d", forn.cod);
    }
}

// Função para mesclar duas metades do array em ordem crescente
void merge_prateleiras(TVetPrat **arr, int l, int m, int r) {
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  // Criando arrays temporários para armazenar as duas metades
  TVetPrat **L = (TVetPrat **)malloc(n1 * sizeof(TVetPrat *));
  TVetPrat **R = (TVetPrat **)malloc(n2 * sizeof(TVetPrat *));

  // Copiando os dados para os arrays temporários L e R
  for (i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];

  // Mesclando os arrays temporários de volta para arr[l..r]
  i = 0;
  j = 0;
  k = l;

while (i < n1 && j < n2) {
    if (L[i]->en-> cod <= R[j]->en->cod) {

      arr[k] = L[i];
      i++;

    } else {

      arr[k] = R[j];
      j++;
    }

    k++;
  }

  // Copiando os elementos restantes de L e R, se houver
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }

  // Liberando memória dos arrays temporários
  free(L);
  free(R);

  // printf("\nMerge Finalizado.\n");
}

// Função principal do Merge Sort
void mergeSort_prateleiras(TVetPrat **arr, int l, int r) {
  // printf("\nEntrou no MergeSort.\n");
  if (l < r) {
    int m = l + (r - l) / 2;

    // Ordenando a primeira e a segunda metade do array
    mergeSort_prateleiras(arr, l, m);
    mergeSort_prateleiras(arr, m + 1, r);

    // Mesclando as duas metades ordenadas
    merge_prateleiras(arr, l, m, r);
  }
}

void intercalacao_prateleiras(char *nome_arquivo_saida, TNomes *nome_particoes) {
    int fim = 0;
    int num_p = nome_particoes->tam;
    FILE *out;
    int i = 0;

  printf("Checkpoint intercalacao 1\n");

  if ((out = fopen(nome_arquivo_saida, "a+")) == NULL) { // arquivo final aberto em modo "append"
    printf("Erro ao abrir arquivo de saida");
    return;

  }

    TVetPrat **v = (TVetPrat **)malloc(num_p * sizeof(TVetPrat)); // matriz de ponteiros para os vetores da entidade
    TParticao *aux_p = nome_particoes->primeiro->prox; // ponteiro para particoes
    printf("Checkpoint intercalacao 2: inicializando vetor\n\n");

    while (aux_p != NULL) { // verifica a primeira entidade em todas as particoes

      v[i] = (TVetPrat *)malloc(sizeof(TVetPrat));
      v[i]->f = fopen(aux_p->nome, "r");
      printf("\nAbriu o arquivo: %s\n", aux_p->nome);

      v[i]->particao = aux_p;
      printf("Associou particao\n");

      if (v[i]->f != NULL) {
        fseek(v[i]->f, 0, SEEK_SET);
        TPrat *c = le_prateleira(v[i]->f);

        printf("Leu entidade\n");
        if (c == NULL) { // indicativo do fim de clientes para leitura
            TProd *p = produto(INT_MAX, " ", "0");
            v[i]-> en = prateleira(INT_MAX, 0, 0, *p);
        } else {
          v[i]-> en = c;
        }
        v[i]->aux_p = 0;

      } else { // indicativo do fim das particoes para leitura
        printf("Nao foi possivel abrir o arquivo.\n\n");
        fim = 1;
      }
      i++;
      // printf("\n%s\n", aux_p -> nome);
      aux_p = aux_p->prox; // vai para o proximo arquivo
    }

    printf("Checkpoint intercalacao 3: vetor preenchido\n");

    // aqui, o procedimento irá retornar um TVet v com num_p primeiros clientes
    // e seus arquivos nessa parte, vai ser ordenado o vetor TVet v

    int n = num_p;
    mergeSort_prateleiras(v, 0, n - 1);

    printf("Checkpoint intercalacao 4: mergesort\n");

    // intercala as entidades
    processa_particoes_prateleiras(v, nome_particoes, num_p, nome_arquivo_saida);

    // no final da função de processamento, é importante que tenha como abrir o arquivo "nome_arquivo_saida" novamente, para indicar que ocorreu tudo certo
    /*
        if ((out = fopen(nome_arquivo_saida, "r+")) == NULL) {
        printf("Erro ao abrir arquivo de saida");
        return;

    }
        int tamanho_arq = tamanho_arquivo_funcionario(out);
        insertion_sort_disco_funcionario(out, tamanho_arq);
    */

    printf("Checkpoint intercalacao 5: fim da escrita no arquivo final\n");

    free(aux_p);
    printf("Intercalacao completa.\n");

}

void processa_particoes_prateleiras(TVetPrat **v, TNomes *nome_particoes, int num_p, char *out) {
  int index = 0;
  int fim = 0;
  int read_count = 0;
  int last;
  int j = 1;

  printf("\nCheckpoint 1: entrou na funcao processar\n");

  for (int i = 0; i < num_p; i += 2) { // inicia intercalando em pares os vetores/particoes
    if ((v[j]) == NULL || j >= num_p) {
      break;
    }
    intercala_particoes_prateleiras(v[i], v[j], nome_particoes, (num_p + index));
    index++;
    j += 2;
  }

   printf("\nCheckpoint 4: saiu da intercalacao inicial do processar\n");

    int flag1 = 0;
    int flag2 = 0;
    char *p_nome;
    int count_sentinel = 0;

  while (!fim) { // nessa segunda iteração, irá verificar e intercalar os vetores que não tiverem sinalizados de "read" (lidos)
    TVetPrat *pt1 = NULL;
    TVetPrat *pt2 = NULL;
    int pos1 = -1;
    int pos2 = -1;
    read_count = 0;

    for (int i = 0; i < num_p; i++) {
      if (v[i]-> read != 1 && pt1 == NULL) { // o valor do vetor 1 só será atribuido se ele for nulo/vazio
        pt1 = v[i];
        pos1 = i;
        continue;
      }
      if (v[i]-> read != 1 && pt1 != NULL) { // o valor do vetor 2 só será atribuido se tiver um valor no vetor 1
        if (v[i] -> particao -> nome != pt1 -> particao -> nome){
             pt2 = v[i];
            pos2 = i;
            break;
        }
      }
    }

    if (pt1 == NULL){ // se o vetor 1 estiver vazio, aciona a flag 1
        flag1 = 1;
        printf("Acionada flag 1\n");
    }
    if (pt1 != NULL && pt2 == NULL){ // se o vetor 2 estiver vazio, aciona a flag 2
        flag2 = 1;
        printf("Acionada flag 1\n");
    }

    // nesse caso, todas as particoes foram lidas. entretanto, ainda há particoes para intercalar (mesmo que lidas)
    // então, iremos verificar se o numero da particao é menor que 0 ao invés de só verificar se ela foi lida

    if (flag1 == 1){
        for (int i = 0; i < num_p; i++) {
            if (v[i] -> particao -> num > 0 && pt1 == NULL){
                pt1 = v[i];
                pos1 = i;
                continue;
            }
            if (v[i] -> particao -> num > 0 && pt1 != NULL){
                if (v[i] -> particao -> nome != pt1 -> particao -> nome){
                        pt2 = v[i];
                        pos2 = i;
                        break;
                }
            }
        }
        printf("Tentativa de resolver flag 1\n");
    }

    if (flag2 == 1){
        for (int i = 0; i < num_p; i++) {
            if (v[i] -> particao -> num > 0 && pt1 != NULL){
                if (v[i] -> particao -> nome != pt1 -> particao -> nome){
                        pt2 = v[i];
                        pos2 = i;
                        break;
                }
            }
        }
         printf("Tentativa de resolver flag 2\n");
    }

    printf("\nCheckpoint 5: definiu os vetores para serem intercalados\n");
    if (pos1 > 0 && pos2 > 0){
        printf("1 nome: %s\n", v[pos1] -> particao -> nome);
        printf("2 nome: %s\n", v[pos2] -> particao -> nome);
    }


    // irá intercalar os vetores encontrado se eles não estiverem vazios
    // as flags são resetadas para verificar a proxima iteração, caso tiver

    if (pt1 != NULL && pt2 != NULL && pos2 != -1) {
      printf("\nCheckpoint ??: ha vetores para intercalar\n");
      // intercala_particoes_funcionarios(v[pos1], v[pos2], nome_particoes, (num_p + index));
      intercala_particoes_prateleiras(v[pos1], v[pos2], nome_particoes, (num_p + index));
      last = num_p + index; // salva o indice da ultima particao criada
      index++; // incrementa o indice caso precise criar mais uma particao posteriormente
    } else  {

        printf("\nCheckpoint 6: verifica se vetores foram intercalados\n");
        // irá verificar se todos os vetores foram intercalados
        for (int i = 0; i < num_p; i++) {
            if (v[i]->read == 1) {
                if (v[i] -> particao -> num < 0){
                    read_count++;
                }
            }
        }
        // se todos forem intercalados, irá finalizar a iteração
        if (read_count == num_p || read_count == num_p - 1) {
            for (int i = 0; i < num_p; i++){
                if (v[i] -> particao -> num != -1){
                    p_nome = v[i] -> particao -> nome;
                    count_sentinel ++;
                }
            }
            free(pt1);
            free(pt2);
            fim = 1;
        }

    }
        flag1 = 0;
        flag2 = 0;
        free(pt1);
        free(pt2);
    }

    printf("\nUltima particao: %s, Count: %d\n", p_nome, count_sentinel);

   printf("\nCheckpoint 7: terminou a intercalacao dos vetores\n");

  // nessa parte, será salvo o nome da ultima partição para o nome do arquivo final escolhido

  char *particao_final = malloc(40 * sizeof(char));
  sprintf(particao_final, "particoesPrat/particao%d", last);
  char *fim_ext = ".dat";
  strcat(particao_final, fim_ext);

    remove(out);
    rename(particao_final, out);
    printf("Remove checkpoint: renomeou arquivo\n");

    printf("Fim do processo de mesclagem.\n");
}

void intercala_particoes_prateleiras(TVetPrat *particao,TVetPrat *particao2, TNomes *nome_particoes, int index) {
  int p = 0, i = 0, signal = 0;
  int fim = 0;
  int numRegistros = NUM_REGISTROS;
  int num_p = nome_particoes->tam;

   printf("\nCheckpoint 2: entrou na funcao intercalar\n");
   print_particao(*particao -> particao);
   print_particao(*particao2 -> particao);

    // é criado o novo arquivo de partição para salvar o resultado da intercalação dos dois vetores
  char *nome = malloc(40 * sizeof(char));
  sprintf(nome, "particoesPrat/particao%d", index);
  char *fim_ext = ".dat";
  strcat(nome, fim_ext);
  FILE *out = fopen(nome, "wb+");

    printf("\nCheckpoint 2.2: criou o arquivo de particao.\n");

  int aux = 0;
  int tam_reg = tamanho_registro_prateleira();

    // são feitos dois vetores do tipo escolhido da entidade para comparar para escrita no arquivo final
  TPrat *p1[10];
  TPrat *p2[10];

  if(particao-> f == NULL){
    particao -> f = fopen(particao -> particao -> nome, "r+");
  }

  if(particao2 -> f == NULL){
    particao2 -> f = fopen(particao2 -> particao -> nome, "r+");
  }

  if (particao2 -> f == NULL || particao -> f == NULL){
    printf("ERROR: Nao foi possivel abrir arquivo de particao.\n");
    return;
  }

  fseek(particao-> f, 0 * tam_reg, SEEK_SET);
  fseek(particao2-> f, 0 * tam_reg, SEEK_SET);

    printf("\nCheckpoint 3: definindo os vetores da entidade..\n");
  for (i = 0; i < 10; i++) {
    p1[i] = le_prateleira(particao->f);
    if (p1[i] == NULL) {
        TProd *p = produto(INT_MAX, " ", "0");
          p1[i] = prateleira(INT_MAX, 0, 0, *p); // se não encontrado, define com INT_MAX o codigo

    }

    fseek(particao->f, i * tam_reg, SEEK_SET);
    particao->aux_p++;
  }

  for (i = 0; i < 10; i++) {
    p2[i] = le_prateleira(particao2->f);
    if (p2[i] == NULL) {
        TProd *p = produto(INT_MAX, " ", "0");
          p2[i] = prateleira(INT_MAX, 0, 0, *p);// se não encontrado, define com INT_MAX o codigo

    }
    fseek(particao2->f, i * tam_reg, SEEK_SET);
    particao2->aux_p++;
  }

printf("\nCheckpoint 3: definiu os vetores da entidade.\n");

  fseek(particao->f, 0 * tam_reg, SEEK_SET);
  fseek(particao2->f, 0 * tam_reg, SEEK_SET);
  fseek(out, 0 * tam_reg, SEEK_SET);

  int n_aux = 0; // variavel para armazenar o numero da particao correspondente

  while (!fim) { // escreve no arquivo de saida
    int menor = INT_MAX;
    int pos_menor = -1;


    for (int j = 0; j < 10; j++) { // irá verificar se algum codigo do primeiro vetor é menor que o salvo
      if (p1[j]->cod < menor) {
        menor = p1[j]->cod;
        n_aux = 1;
      }
    }

    for (int j = 0; j < 10; j++) { // depois, irá verificar se algum codigo do segundo vetor é menor que o menor do primeiro
      if (p2[j]->cod < menor) {
        menor = p2[j]->cod;
        n_aux = 2;
      }
    }

    if (pos_menor != -1) { // verifica se alguma entidade foi encontrada

      fseek(out, aux * tam_reg, SEEK_SET); // ajusta a posição para escrita no arquivo de saída
      if (n_aux == 1) {
        fseek(out, aux * tam_reg, SEEK_SET);
        salva_prateleira(p1[pos_menor], out);
        particao->aux_p++; // incrementa ponteiro para referencia da posição no arquivo da particao

        fseek(particao->f, particao->aux_p * tam_reg, SEEK_SET); // salva o proximo cliente do arquivo do ultimo salvo no vetor
        particao->en = le_prateleira(particao->f);

        if (particao->en == NULL) {
            TProd *p = produto(INT_MAX, " ", "0");
          particao->en = prateleira(INT_MAX, 0, 0, *p); // se não encontrado, define com INT_MAX o codigo
        }

        p1[pos_menor] = particao->en; //salva a entidade encontrada no vetor
        aux++; // incrementa ponteiro para referencia da posição no arquivo de saida

      } else if (n_aux == 2) {
        fseek(out, aux * tam_reg, SEEK_SET);
        salva_prateleira(p2[pos_menor], out);
        particao2->aux_p++;

        fseek(particao2->f, particao2->aux_p * tam_reg, SEEK_SET);
        particao2->en = le_prateleira(particao2->f);

        if (particao2->en == NULL) {
            TProd *p = produto(INT_MAX, " ", "0");
          particao2->en = prateleira(INT_MAX, 0, 0, *p);
        }

        p2[pos_menor] = particao2->en;
        aux++;
      }

    } else {
      fim = 1; // se o menor codigo por INT_MAX, significa que acabou a leitura das particoes
    }
  }

  fclose(particao->f);
  fclose(particao2->f);

// define o novo arquivo dos vetores como a ultima particao criada
  particao->f = out;
  particao2->f = out;

// reinicia o ponteiro para leitura
  particao->aux_p = 0;
  particao2->aux_p = 0;

// redefine o nome e indice de referencia da particao nos vetores

  particao->particao->nome = nome;
  particao->particao->num = index;

  particao2->particao->nome = nome;
  particao2->particao->num = -1;


// define como lido um dos vetores
  particao2->read = 1;

   printf("\nCheckpoint 3: saiu da funcao intercalar\n\n");
}
