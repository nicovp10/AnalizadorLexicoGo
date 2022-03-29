#ifndef ABB_H
#define ABB_H


#include "definicions.h"


// TIPO DE DATO OPACO DUNHA ÁRBORE AVL


typedef char *tipoclave;
typedef CompLexico tipoelem;

typedef void *avl;


// Función que crea a árbore inicializando o punteiro a NULL
void crear(avl *A);

// Función que destrúe a árbore, liberando a memoria
void destruir(avl *A);

// Función que comproba se a árbore está vacía
unsigned vacia(avl A);

// Función que le o elemento dun nodo
void ler(avl A, tipoelem *E);

// Función que devolve a subárbore esquerda de A
avl izq(avl A);

// Función que devolve a subárbore dereita de A
avl der(avl A);

// Función que busca un nodo na árbore. Se non o atopa devolve NULL en *nodo
void buscar_nodo(avl A, tipoclave cl, tipoelem *nodo);

// Función que inserta un novo nodo na árbore (presuponse que non existe un nodo coa misma clave nesta)
void insertar(avl *A, tipoelem E);


#endif