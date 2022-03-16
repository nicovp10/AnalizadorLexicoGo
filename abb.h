#ifndef ABB_H
#define ABB_H


#include "definicions.h"


typedef char *tipoclave;

typedef CompLexico tipoelem;

typedef struct celda *abb; // Tipo opaco


void crear(abb *A);

void destruir(abb *A);

unsigned es_vacio(abb A);

void buscar_nodo(abb A, tipoclave cl, tipoelem *nodo);

void insertar(abb *A, tipoelem E);

void suprimir(abb *A, tipoelem E);


#endif