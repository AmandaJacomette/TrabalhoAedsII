#include "entidades.h"
#include <stdio.h>
#include <stdlib.h>

void msg_MENU_inicial() {
  printf(
      "\n----------- PROGRAMA DE GERENCIAMENTO DE SUPERMERCADO -----------\n");
  printf("OBS.: Todas as informacoes serao armazenadas em arquivos.\nBASES "
         "DISPONIVEIS:\n");
  printf("1 - Funcionarios\n2 - Clientes\n3 - Fornecedores\n4 - Prateleiras\n5 "
         "- Encomendas\n6 - Produtos\n7 - Estoque\n8 - Sair\n");
  printf("RESPOSTA: ");
}

void msg_MENU(char *tipo) {
  printf("\n----------- PROGRAMA DE GERENCIAMENTO DE %s -----------\n", tipo);
  printf("OBS.: Todas as informacoes serao armazenadas em "
         "arquivos.\n\nOPERACOES DISPONIVEIS:\n");
  printf("1 - Criar base\n2 - Ordenar\n3 - Imprimir\n4 - Pesquisar "
         "(Sequencial)\n5 - Pesquisar (Binaria)\n6 - Criar Particoes\n7 - "
         "Intercalacao\n8 - Sair\n");
  printf("RESPOSTA: ");
}

void menu_funcionario() {
    FILE *out;
    // FILE *ordenado = fopen("final_func.dat", "r+");
    FILE *ordenado = fopen("func_intercalado.dat", "r+");
    int flag = 0;
    TNomes *conjunto_particoes;

  if ((out = fopen(FUNCIONARIOS_FILE, "w+b")) == NULL) {
    printf("Erro ao abrir arquivo\n");
    exit(1);
  } else {
    int escolha = -1;
    int cod;

    apaga_particoes_existentes("particoesFunc");

    while (escolha != 8) {
      msg_MENU("FUNCIONARIO");
      scanf("%d", &escolha);
      if (escolha == 1) {
        printf("\nInforme quantos registros tera a base: ");
        int num;
        scanf("%d", &num);
        initializeBaseDesorder_funcionario(out, num);
      } else if (escolha == 2) {
        insertion_sort_disco_funcionario(out, tamanho_arquivo_funcionario(out));
        printf("\n-----------------------------Base "
               "ordenada-----------------------");
        le_funcionarios(out);
      } else if (escolha == 3) {
        if (flag) {
          if ((ordenado = fopen("func_intercalado.dat", "r+")) == NULL) {

            printf("Erro ao abrir arquivo\n");
            exit(1);
          }

          le_funcionarios(ordenado);
          printf("\nArquivo lido: Ordenado por particoes\n");

        } else {
          if (out == NULL) {
            printf("Erro ao abrir arquivo\n");
            exit(1);
          }

          le_funcionarios(out);
          printf("\nArquivo lido: Ordenado em disco\n");
        }



      } else if (escolha == 4) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n-----------------------------Busca "
               "sequencial-----------------------");
        TFunc *func1 = busca_sequencial_funcionario(cod, out);
        printf("\n");
        if (func1->cod == 0 || func1 == NULL) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_funcionario(func1);
        }

      } else if (escolha == 5) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n\n-----------------------------Busca "
               "binaria-----------------------");
        TFunc *func = busca_binaria_funcionario(
            cod, out, tamanho_arquivo_funcionario(out));
        printf("\n");

        if (func->cod == 0 || func == NULL) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_funcionario(func);
        }
      } else if (escolha == 6) {
        printf("Criando partições...");
        classificacaoSubs_func((out));
        conjunto_particoes = le_nomes_particoes_diretorio("particoesFunc");

      } else if (escolha == 7) {
        printf("Intercalando particoes...");

        if (lista_vazia(*conjunto_particoes)) {
          printf("Nao existe particoes para intercalar.\n");
          break;

        } else {
          mostrar_particoes(*conjunto_particoes);
          intercalacao_funcionarios("func_intercalado.dat", conjunto_particoes);
          // intercala_teste_1("clientes_intercalado.dat", conjunto_particoes);
          flag = 1;
        }

      } else if (escolha == 8) {
        system("cls");
        break;

      } else {
        printf("\nESCOLHA UMA OPCAO VALIDA!\n");
        break;
      }
    }
    fclose(out);
  }
}

void menu_cliente() {
  FILE *out_cli;
  FILE *ordenado = fopen("final_cliente.dat", "r+");
  int flag = 0;
  TNomes *conjunto_particoes;

  if ((out_cli = fopen(CLIENTES_FILE, "w+b")) == NULL) {
    printf("Erro ao abrir arquivo\n");
    exit(1);

  } else {
    int escolha = -1;
    int cod;

    apaga_particoes_existentes("particoesCli");

    while (escolha != 8) {

      msg_MENU("CLIENTE");
      scanf("%d", &escolha);
      if (escolha == 1) {
        printf("\nInforme quantos registros tera a base: ");
        int num;
        scanf("%d", &num);
        initializeBaseDesorder_cliente(out_cli, num);

      } else if (escolha == 2) {
        insertion_sort_disco_cliente(out_cli, tamanho_arquivo_cliente(out_cli));
        printf("\n-----------------------------Base "
               "ordenada-----------------------");
        le_clientes(out_cli);

      } else if (escolha == 3) {

        if (flag) {
          if ((ordenado = fopen("clientes_intercalado.dat", "r+")) == NULL) {

            printf("Erro ao abrir arquivo\n");
            exit(1);
          }

          le_clientes(ordenado);
          printf("\nArquivo lido: Ordenado por particoes\n");

        } else {
          if (out_cli == NULL) {
            printf("Erro ao abrir arquivo\n");
            exit(1);
          }

          le_clientes(out_cli);
          printf("\nArquivo lido: Ordenado em disco\n");
        }

        // le_clientes(out_cli);

      } else if (escolha == 4) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n-----------------------------Busca "
               "sequencial-----------------------");
        TCliente *c1 = busca_sequencial_cliente(cod, out_cli);

        printf("\n");

        if (c1->cod == 0 || c1 == NULL) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");

        } else {
          imprime_cliente(c1);
        }

      } else if (escolha == 5) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n\n-----------------------------Busca "
               "binaria-----------------------");

        TCliente *c2 = busca_binaria_cliente(cod, out_cli,
                                             tamanho_arquivo_cliente(out_cli));
        printf("\n");

        if (c2->cod == 0 || c2 == NULL) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");

        } else {
          imprime_cliente(c2);
        }

      } else if (escolha == 6) {
        int num_particoes;
        /*
       printf("\nInsira a quantidade de registros nos subarquivos: ");
       scanf("%d", &num_particoes);
       classificacao_interna(out_cli, num_particoes);
      */
        classificacaoSubs_cli(out_cli);
        conjunto_particoes = le_nomes_particoes_diretorio("particoesCli");

      } else if (escolha == 7) {
        printf("Intercalando particoes...");
        if (lista_vazia(*conjunto_particoes)) {
          printf("Nao existe particoes para intercalar.\n");
          break;

        } else {
          mostrar_particoes(*conjunto_particoes);
          intercalacao_clientes("clientes_intercalado.dat", conjunto_particoes);
          // intercala_teste_1("clientes_intercalado.dat", conjunto_particoes);
          flag = 1;
        }

      } else if (escolha == 8) {
        system("cls");
        break;

      } else {
        printf("\nESCOLHA UMA OPCAO VALIDA!\n");
        break;
      }
    }
    fclose(out_cli);
  }
}

void menu_fornecedor() {
    FILE *out;
    FILE *ordenado = fopen("forn_intercalado.dat", "r+");
    TNomes *conjunto_particoes;
    int flag = 0;

  if ((out = fopen(FORNECEDORES_FILE, "w+b")) == NULL) {
    printf("Erro ao abrir arquivo\n");
    exit(1);
  } else {
    int escolha = -1;
    int cod;

    apaga_particoes_existentes("particoesForn");

    while (escolha != 8) {
      msg_MENU("FORNECEDORES");
      scanf("%d", &escolha);
      if (escolha == 1) {
        printf("\nInforme quantos registros tera a base: ");
        int num;
        scanf("%d", &num);
        initializeBaseDesorder_fornecedor(out, num);
      } else if (escolha == 2) {
        insertion_sort_disco_fornecedor(out, tamanho_arquivo_fornecedor(out));
        printf("\n-----------------------------Base "
               "ordenada-----------------------");
        le_fornecedores(out);
      } else if (escolha == 3) {
        if (flag) {
          if ((ordenado = fopen("forn_intercalado.dat", "r+")) == NULL) {

            printf("Erro ao abrir arquivo\n");
            exit(1);
          }

          le_fornecedores(ordenado);
          printf("\nArquivo lido: Ordenado por particoes\n");

        } else {
          if (out == NULL) {
            printf("Erro ao abrir arquivo\n");
            exit(1);
          }

          le_fornecedores(out);
          printf("\nArquivo lido: Ordenado em disco\n");
        }



      } else if (escolha == 4) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n-----------------------------Busca "
               "sequencial-----------------------");
        TForn forn = busca_sequencial_fornecedor(cod, out);
        printf("\n");

        if (forn.cod != cod) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_fornecedor(&forn);
        }

      } else if (escolha == 5) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n\n-----------------------------Busca "
               "binaria-----------------------");
        TForn forn =
            busca_binaria_fornecedor(cod, out, tamanho_arquivo_fornecedor(out));
        printf("\n");
        if (forn.cod != cod) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_fornecedor(&forn);
        }
      } else if (escolha == 6) {
        printf("Criando partições...");
        classificacaoSubs_forn((out));
        conjunto_particoes = le_nomes_particoes_diretorio("particoesForn");

      } else if (escolha == 7) {
        printf("Intercalando particoes...");

        if (lista_vazia(*conjunto_particoes)) {
          printf("Nao existe particoes para intercalar.\n");
          break;

        } else {
          mostrar_particoes(*conjunto_particoes);
          intercalacao_fornecedores("forn_intercalado.dat", conjunto_particoes);
          // intercala_teste_1("clientes_intercalado.dat", conjunto_particoes);
          flag = 1;
        }

      } else if (escolha == 8) {
        system("cls");
        break;

      } else {
        printf("\nESCOLHA UMA OPCAO VALIDA!\n");
        break;
      }
    }
    fclose(out);
  }
}

void menu_encomenda() {
    FILE *out;
    FILE *ordenado = fopen("enc_intercalado.dat", "r+");
    TNomes *conjunto_particoes;
    int flag = 0;

  if ((out = fopen(ENCOMENDAS_FILE, "w+b")) == NULL) {
    printf("Erro ao abrir arquivo\n");
    exit(1);
  } else {
    int escolha = -1;
    int cod;

      apaga_particoes_existentes("particoesEncom");

    while (escolha != 8) {
      msg_MENU("ENCOMENDAS");
      scanf("%d", &escolha);
      if (escolha == 1) {
        printf("\nInforme quantos registros tera a base (encomendas): ");
        int num;
        scanf("%d", &num);
        initializeBaseDesorder_encomenda(out, num);
      } else if (escolha == 2) {
        insertion_sort_disco_encomenda(out, tamanho_arquivo_encomenda(out));
        printf("\n-----------------------------Base "
               "ordenada-----------------------");
        le_encomendas(out);
      } else if (escolha == 3) {
        if (flag) {
          if ((ordenado = fopen("enc_intercalado.dat", "r+")) == NULL) {

            printf("Erro ao abrir arquivo\n");
            exit(1);
          }

          le_encomendas(ordenado);
          printf("\nArquivo lido: Ordenado por particoes\n");

        } else {
          if (out == NULL) {
            printf("Erro ao abrir arquivo\n");
            exit(1);
          }

          le_encomendas(out);
          printf("\nArquivo lido: Ordenado em disco\n");
        }



      }
       else if (escolha == 4) {
        printf("\nInforme o codigo a ser buscado: ");
        scanf("%d", &cod);

        printf("\n-----------------------------Busca "
               "sequencial-----------------------");
        TEncomenda enc = busca_sequencial_encomenda(cod, out);
        printf("\n");
        if (enc.id != cod) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_encomenda(&enc);
        }

      } else if (escolha == 5) {
        printf("\nInforme o codigo a ser buscado: ");
        scanf("%d", &cod);

        printf("\n\n-----------------------------Busca "
               "binaria-----------------------");
        TEncomenda enc =
            busca_binaria_encomenda(cod, out, tamanho_arquivo_encomenda(out));
        printf("\n");

        if (enc.id != cod) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_encomenda(&enc);
        }

      } else if (escolha == 6) {
        printf("Criando partições...");
        classificacaoSubs_encom((out));
        conjunto_particoes = le_nomes_particoes_diretorio("particoesEncom");

      } else if (escolha == 7) {
        printf("Intercalando particoes...");

        if (lista_vazia(*conjunto_particoes)) {
          printf("Nao existe particoes para intercalar.\n");
          break;

        } else {
          mostrar_particoes(*conjunto_particoes);
          intercalacao_encomendas("enc_intercalado.dat", conjunto_particoes);
          // intercala_teste_1("clientes_intercalado.dat", conjunto_particoes);
          flag = 1;
        }

      } else if (escolha == 8) {
        system("cls");
        break;

      } else {
        printf("\nESCOLHA UMA OPCAO VALIDA!\n");
        break;
      }
    }
    fclose(out);
  }
}

void menu_produto() {
    FILE *out_cli;
    FILE *ordenado = fopen("prod_intercalado.dat", "r+");
    TNomes *conjunto_particoes;
    int flag = 0;

  if ((out_cli = fopen(PRODUTOS_FILE, "w+b")) == NULL) {
    printf("Erro ao abrir arquivo\n");
    exit(1);
  } else {
    int escolha = -1;
    int cod;

      apaga_particoes_existentes("particoesProd");

    while (escolha != 8) {
      msg_MENU("PRODUTO");
      scanf("%d", &escolha);
      if (escolha == 1) {
        printf("\nInforme quantos registros tera a base: ");
        int num;
        scanf("%d", &num);
        initializeBaseDesorder_produto(out_cli, num);
      } else if (escolha == 2) {
        insertion_sort_disco_produto(out_cli, tamanho_arquivo_produto(out_cli));
        printf("\n-----------------------------Base "
               "ordenada-----------------------");
        le_produtos(out_cli);
      } else if (escolha == 3) {
        if (flag) {
          if ((ordenado = fopen("prod_intercalado.dat", "r+")) == NULL) {

            printf("Erro ao abrir arquivo\n");
            exit(1);
          }

          le_produtos(ordenado);
          printf("\nArquivo lido: Ordenado por particoes\n");

        } else {
          if (out_cli == NULL) {
            printf("Erro ao abrir arquivo\n");
            exit(1);
          }

          le_produtos(out_cli);
          printf("\nArquivo lido: Ordenado em disco\n");
        }



      }

       else if (escolha == 4) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n-----------------------------Busca "
               "sequencial-----------------------");
        TProd prod = busca_sequencial_produto(cod, out_cli);
        printf("\n");
        if (prod.cod != cod) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_produto(&prod);
        }

      } else if (escolha == 5) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n\n-----------------------------Busca "
               "binaria-----------------------");
        TProd prod = busca_binaria_produto(cod, out_cli,
                                           tamanho_arquivo_produto(out_cli));
        printf("\n");

        if (prod.cod != cod) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_produto(&prod);
        }
      } else if (escolha == 6) {
        printf("Criando partições...");
        classificacaoSubs_prod((out_cli));
        conjunto_particoes = le_nomes_particoes_diretorio("particoesProd");

      } else if (escolha == 7) {
        printf("Intercalando particoes...");

        if (lista_vazia(*conjunto_particoes)) {
          printf("Nao existe particoes para intercalar.\n");
          break;

        } else {
          mostrar_particoes(*conjunto_particoes);
          intercalacao_produtos("prod_intercalado.dat", conjunto_particoes);
          // intercala_teste_1("clientes_intercalado.dat", conjunto_particoes);
          flag = 1;
        }

      } else if (escolha == 8) {
        system("cls");
        break;

      } else {
        printf("\nESCOLHA UMA OPCAO VALIDA!\n");
        break;
      }
    }
    fclose(out_cli);
  }
}

void menu_prateleira() {
    FILE *out_cli;
    FILE *ordenado = fopen("prat_intercalado.dat", "r+");
    TNomes *conjunto_particoes;
    int flag = 0;

  if ((out_cli = fopen(PRATELEIRA_FILE, "w+b")) == NULL) {
    printf("Erro ao abrir arquivo\n");
    exit(1);
  } else {
    int escolha = -1;
    int cod;

      apaga_particoes_existentes("particoesPrat");

    while (escolha != 8) {
      msg_MENU("PRATELEIRAS");
      scanf("%d", &escolha);
      if (escolha == 1) {
        printf("\nInforme quantos registros tera a base: ");
        int num;
        scanf("%d", &num);
        initializeBaseDesorder_prateleira(out_cli, num);
      } else if (escolha == 2) {
        insertion_sort_disco_prateleira(out_cli,
                                        tamanho_arquivo_prateleira(out_cli));
        printf("\n-----------------------------Base "
               "ordenada-----------------------");
        le_prateleiras(out_cli);
      } else if (escolha == 3) {
        if (flag) {
          if ((ordenado = fopen("prat_intercalado.dat", "r+")) == NULL) {

            printf("Erro ao abrir arquivo\n");
            exit(1);
          }

          le_prateleiras(ordenado);
          printf("\nArquivo lido: Ordenado por particoes\n");

        } else {
          if (out_cli == NULL) {
            printf("Erro ao abrir arquivo\n");
            exit(1);
          }

          le_prateleiras(out_cli);
          printf("\nArquivo lido: Ordenado em disco\n");
        }



      }

       else if (escolha == 4) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n-----------------------------Busca "
               "sequencial-----------------------");
        TPrat prat = busca_sequencial_prateleira(cod, out_cli);
        printf("\n");

        if (prat.cod != cod) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_prateleira(&prat);
        }

      } else if (escolha == 5) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n\n-----------------------------Busca "
               "binaria-----------------------");
        TPrat prat = busca_binaria_prateleira(
            cod, out_cli, tamanho_arquivo_prateleira(out_cli));
        printf("\n");
        if (prat.cod != cod) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_prateleira(&prat);
        }
      } else if (escolha == 6) {
        printf("Criando partições...");
        classificacaoSubs_prat((out_cli));
        conjunto_particoes = le_nomes_particoes_diretorio("particoesPrat");

      } else if (escolha == 7) {
        printf("Intercalando particoes...");

        if (lista_vazia(*conjunto_particoes)) {
          printf("Nao existe particoes para intercalar.\n");
          break;

        } else {
          mostrar_particoes(*conjunto_particoes);
          intercalacao_prateleiras("prat_intercalado.dat", conjunto_particoes);
          // intercala_teste_1("clientes_intercalado.dat", conjunto_particoes);
          flag = 1;
        }

      } else if (escolha == 8) {
        system("cls");
        break;

      } else {
        printf("\nESCOLHA UMA OPCAO VALIDA!\n");
        break;
      }
    }
    fclose(out_cli);
  }
}

void menu_estoque() {
    FILE *out_cli;
    FILE *ordenado = fopen("estq_intercalado.dat", "r+");
    TNomes *conjunto_particoes;
    int flag = 0;

  if ((out_cli = fopen(ESTOQUE_FILE, "w+b")) == NULL) {
    printf("Erro ao abrir arquivo\n");
    exit(1);
  } else {
    int escolha = -1;
    int cod;

      apaga_particoes_existentes("particoesEstoq");

    while (escolha != 8) {
      msg_MENU("ESTOQUE");
      scanf("%d", &escolha);
      if (escolha == 1) {
        printf("\nInforme quantos registros tera a base: ");
        int num;
        scanf("%d", &num);
        initializeBaseDesorder_estoque(out_cli, num);
      } else if (escolha == 2) {
        insertion_sort_disco_estoque(out_cli, tamanho_arquivo_estoque(out_cli));
        printf("\n-----------------------------Base "
               "ordenada-----------------------");
        le_estoques(out_cli);
      } else if (escolha == 3) {
        if (flag) {
          if ((ordenado = fopen("estq_intercalado.dat", "r+")) == NULL) {

            printf("Erro ao abrir arquivo\n");
            exit(1);
          }

          le_estoques(ordenado);
          printf("\nArquivo lido: Ordenado por particoes\n");

        } else {
          if (out_cli == NULL) {
            printf("Erro ao abrir arquivo\n");
            exit(1);
          }

          le_estoques(out_cli);
          printf("\nArquivo lido: Ordenado em disco\n");
        }



      }

       else if (escolha == 4) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n-----------------------------Busca "
               "sequencial-----------------------");
        TEstoque est = busca_sequencial_estoque(cod, out_cli);
        printf("\n");

        if (est.cod != cod) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_estoque(&est);
        }

      } else if (escolha == 5) {
        printf("\nInforme o codigo a ser buscado: ");

        scanf("%d", &cod);

        printf("\n\n-----------------------------Busca "
               "binaria-----------------------");
        TEstoque est = busca_binaria_estoque(cod, out_cli,
                                             tamanho_arquivo_estoque(out_cli));
        printf("\n");
        if (est.cod != cod) {
          printf("Nao foi possivel encontrar o codigo solicitado.\n");
        } else {
          imprime_estoque(&est);
        }
      } else if (escolha == 6) {
        printf("Criando partições...");
        classificacaoSubs_estoq((out_cli));
        conjunto_particoes = le_nomes_particoes_diretorio("particoesEstoq");

      } else if (escolha == 7) {
        printf("Intercalando particoes...");

        if (lista_vazia(*conjunto_particoes)) {
          printf("Nao existe particoes para intercalar.\n");
          break;

        } else {
          mostrar_particoes(*conjunto_particoes);
          intercalacao_estoque("estq_intercalado.dat", conjunto_particoes);
          // intercala_teste_1("clientes_intercalado.dat", conjunto_particoes);
          flag = 1;
        }

      } else if (escolha == 8) {
        system("cls");
        break;

      } else {
        printf("\nESCOLHA UMA OPCAO VALIDA!\n");
        break;
      }
    }
    fclose(out_cli);
  }
}

int main(int argc, char **argv) {
  int option = -1;

  while (option != 8) {
    msg_MENU_inicial();
    scanf("%d", &option);

    if (option == 1) {
      menu_funcionario();

    } else if (option == 2) {
      menu_cliente();

    } else if (option == 3) {
      menu_fornecedor();

    } else if (option == 4) {
      menu_prateleira();

    } else if (option == 5) {
      menu_encomenda();

    } else if (option == 6) {
      menu_produto();

    } else if (option == 7) {
      menu_estoque();

    } else if (option == 8) {
      printf("\nFIM DO PROGRAMA!\n");
      system("cls");
      break;
    } else {
      printf("\nESCOLHA UMA OPCAO VALIDA!\n");
      break;
    }
  }
}
