#ifndef P1_ANALIZADORLEXICO_H
#define P1_ANALIZADORLEXICO_H


#include "definicions.h"


// Función que inicia o analizador léxico
void iniciarAnalizadorLexico(char *nomeFicheiro);

// Función que le o seguinte compoñente léxico do código fonte
CompLexico segCompLexico();

// Función que finaliza o analizador léxico
void finalizarAnalizadorLexico();


#endif
