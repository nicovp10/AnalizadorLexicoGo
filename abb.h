#ifndef ABB_H
#define ABB_H


#include "definicions.h"


typedef char *tipoclave;
typedef CompLexico tipoelem;

typedef struct celda *abb;


// Función que crea a árbore inicializando o punteiro a NULL
void crear(abb *A);

// Función que destrúe a árbore, liberando a memoria
void destruir(abb *A);

// Función que comproba se a árbore está vacía
unsigned vacia(abb A);

// Función que le o elemento dun nodo
void ler(abb A, tipoelem *E);

// Función que devolve a subárbore esquerda de A
abb izq(abb A);

// Función que devolve a subárbore dereita de A
abb der(abb A);

// Función que busca un nodo na árbore. Se non o atopa devolve NULL en *nodo
void buscar_nodo(abb A, tipoclave cl, tipoelem *nodo);

// Función que inserta un novo nodo na árbore (presuponse que non existe un nodo coa misma clave nesta)
void insertar(abb *A, tipoelem E);

// Función que elimina un elemento da árbore
void suprimir(abb *A, tipoelem E);


#endif