#include <stdlib.h>
#include <string.h>

#include "abb.h"

struct celda {
    tipoelem info;
    struct celda *izq, *der;
};


// Extráese a clave dun nodo
tipoclave _clave_elem(tipoelem *E) {
    return E->lexema;
}

// Compara dúas claves
int _comparar_claves(tipoclave cl1, tipoclave cl2) {
    return strcmp(cl1, cl2) == 0 ? 0 : strcmp(cl1, cl2) > 0 ? 1 : -1;
}

// Compara unha clave cun elemento
int _comparar_clave_elem(tipoclave cl, tipoelem E) {
    return _comparar_claves(cl, _clave_elem(&E));
}

// Devolve o mínimo da subárbore dereita borrándoo da árbore (o elemento máis á esquerda de devandita subárbore)
tipoelem _suprimir_min(abb *A) {
    abb aux;
    tipoelem ele;
    if (vacia((*A)->izq)) { // Se a esquerda está vacía, devólvese o valor do nodo actual
        ele = (*A)->info;
        aux = *A;
        *A = (*A)->der;
        free(aux);
        return ele;
    } else {
        return _suprimir_min(&(*A)->izq); // Se a esquerda non está vacía, profundízase na búsqueda
    }
}


// Créase a árbore inicializando o punteiro a NULL
void crear(abb *A) {
    *A = NULL;
}

// Destrúese a árbore, liberando a memoria
void destruir(abb *A) {
    if (*A != NULL) {
        destruir(&(*A)->izq);
        destruir(&(*A)->der);
        free((*A)->info.lexema);
        free(*A);
        *A = NULL;
    }
}

// Compróbase se a árbore é vacía
unsigned vacia(abb A) {
    return A == NULL;
}

// Le o elemento dun nodo
void ler(abb A, tipoelem *E) {
    *E = A->info;
}

// Devolve a subárbore esquerda de A
abb izq(abb A) {
    return A->izq;
}

// Devolve a subárbore dereita de A
abb der(abb A) {
    return A->der;
}

// Busca un nodo na árbore, se non o atopa devolve NULL en *nodo
void buscar_nodo(abb A, tipoclave cl, tipoelem *nodo) {
    if (vacia(A)) {
        return;
    }
    int comp = _comparar_clave_elem(cl, A->info);

    if (comp == 0) { // cl == A->info
        *nodo = A->info;
    } else if (comp < 0) { // cl < A->info
        buscar_nodo(A->izq, cl, nodo);
    } else { // cl > A->info
        buscar_nodo(A->der, cl, nodo);
    }
}

// Inserta un novo nodo na árbore (presuponse que non existe un nodo coa misma clave nesta)
void insertar(abb *A, tipoelem E) {
    if (vacia(*A)) {
        *A = (abb) malloc(sizeof(struct celda));
        (*A)->info.comp_lexico = E.comp_lexico;
        (*A)->info.lexema = malloc((strlen(E.lexema) + 1) * sizeof(char));
        strcpy((*A)->info.lexema, E.lexema);
        (*A)->info.lexema[strlen(E.lexema)] = '\0';
        (*A)->izq = NULL;
        (*A)->der = NULL;
        return;
    }
    tipoclave cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if (comp > 0) {
        insertar(&(*A)->der, E);
    } else {
        insertar(&(*A)->izq, E);
    }
}

// Elimina un elemento da árbore
void suprimir(abb *A, tipoelem E) {
    abb aux;
    if (vacia(*A)) {
        return;
    }

    tipoclave cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if (comp < 0) { // (E < (*A)->info))
        suprimir(&(*A)->izq, E);
    } else if (comp > 0) { // (E > (*A)->info))
        suprimir(&(*A)->der, E);
    } else if (vacia((*A)->izq) && vacia((*A)->der)) {
        free(*A);
        *A = NULL;
    } else if (vacia((*A)->izq)) { // Se non é vacío á dereita:
        aux = *A;
        *A = (*A)->der;
        free(aux);
    } else if (vacia((*A)->der)) { // Se non é vacío á esquerda:
        aux = *A;
        *A = (*A)->izq;
        free(aux);
    } else { // Nin dereita nin esquerda están vacíos:
        // Búscase o mínimo da subárbore dereita. No seu sitio vaise poñer ese mínimo
        (*A)->info = _suprimir_min(&(*A)->der);
    }
}

