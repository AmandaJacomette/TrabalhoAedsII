#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

void embaralhar(int *vet, size_t vetSize);
void adicionar_tempo_comparacoes_sequencial(char *tipoProcurado,
                                            double tempoTotal, int comp);
void adicionar_tempo_comparacoes_binario(char *tipoProcurado, double tempoTotal,
                                         int comp);

#endif // UTILS_H_INCLUDED
