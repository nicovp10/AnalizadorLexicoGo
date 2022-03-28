#ifndef ANALIZADORLEXICO_H
#define ANALIZADORLEXICO_H


#include "definicions.h"


// Función que devolve a línea actual de análise
int obterLineaActual();

// Función que inicia o analizador léxico
void iniciarAnalizadorLexico(char *nomeFicheiro);

// Función que le o seguinte compoñente léxico do código fonte
CompLexico segCompLexico();

// Función que finaliza o analizador léxico
void finalizarAnalizadorLexico();


#endif
