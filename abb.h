#ifndef ABB_H
#define ABB_H


#include "definicions.h"


typedef char *tipoclave;

typedef CompLexico tipoelem;

typedef struct celda *abb;


void crear(abb *A);

void destruir(abb *A);

unsigned vacia(abb A);

void ler(abb A, tipoelem *E);

abb izq(abb A);

abb der(abb A);

void buscar_nodo(abb A, tipoclave cl, tipoelem *nodo);

void insertar(abb *A, tipoelem E);

void suprimir(abb *A, tipoelem E);


#endif