#ifndef ANALIZADORLEXICO_H
#define ANALIZADORLEXICO_H


#include "definicions.h"


// Función que inicia o analizador léxico
void iniciarAnalizadorLexico(char *nomeFicheiro);

// Función que le o seguinte compoñente léxico do código fonte
CompLexico segCompLexico();

// Función que finaliza o analizador léxico
void finalizarAnalizadorLexico();


#endif
