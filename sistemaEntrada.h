#ifndef P1_SISTEMAENTRADA_H
#define P1_SISTEMAENTRADA_H


#include "definicions.h"


// Función que inicia o sistema de entrada
void iniciarSistemaEntrada(char *nomeFicheiro);

// Función que obtén seguinte caracter da memoria intermedia
char segCaracter();

// Función que ignora un caracter, saltándoo na memoria intermedia
void ignorarCaracter();

// Función que devolve un caracter, retrocedendo na memoria intermedia
void devolverCaracter();

// Función que acepta o lexema que se está lendo actualmente
void aceptarLexema(CompLexico *comp);

// Función que salta o lexema que se está lendo actualmente
void saltarLexema();

// Función que finaliza o sistema de entrada
void finalizarSistemaEntrada();


#endif