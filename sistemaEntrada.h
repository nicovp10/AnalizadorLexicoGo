#ifndef P1_SISTEMAENTRADA_H
#define P1_SISTEMAENTRADA_H


#include "definicions.h"


void iniciarSistemaEntrada(char *nomeFicheiro);

char segCaracter();

void devolverCaracter();

void aceptarLexema(CompLexico *comp);

void finalizarSistemaEntrada();


#endif