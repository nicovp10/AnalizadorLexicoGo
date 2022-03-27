#include <stdlib.h>
#include <string.h>


#include "definicions.h"


typedef char *tipoclave;
typedef CompLexico tipoelem;

struct celda {
    tipoelem info;
    int fe; // Factor de equilibio do AVL
    struct celda *pai, *izq, *der;
};

typedef struct celda *avl;


// Función auxilizar que extrae a clave dun nodo
tipoclave _clave_elem(tipoelem *E) {
    return E->lexema;
}

// Función auxiliar que compara dúas claves
int _comparar_claves(tipoclave cl1, tipoclave cl2) {
    return strcmp(cl1, cl2) == 0 ? 0 : strcmp(cl1, cl2) > 0 ? 1 : -1;
}

// Función auxiliar que compara unha clave calquera ca dun elemento
int _comparar_clave_elem(tipoclave cl, tipoelem E) {
    return _comparar_claves(cl, _clave_elem(&E));
}


// Función que crea a árbore inicializando o punteiro a NULL
void crear(avl *A) {
    *A = NULL;
}

// Función que destrúe a árbore, liberando a memoria
void destruir(avl *A) {
    if (*A != NULL) {
        //destruir(&(*A)->pai); // TODO ao mellor non é necesario
        destruir(&(*A)->izq);
        destruir(&(*A)->der);
        free((*A)->info.lexema);
        free(*A);
        *A = NULL;
    }
}

// Función que comproba se a árbore está vacía
unsigned vacia(avl A) {
    return A == NULL;
}

// Función que le o elemento dun nodo
void ler(avl A, tipoelem *E) {
    *E = A->info;
}

// Función que devolve a subárbore esquerda de A
avl izq(avl A) {
    return A->izq;
}

// Función que devolve a subárbore dereita de A
avl der(avl A) {
    return A->der;
}

// Función que busca un nodo na árbore. Se non o atopa devolve NULL en *nodo
void buscar_nodo(avl A, tipoclave cl, tipoelem *nodo) {
    if (vacia(A)) {
        return;
    }
    int comp = _comparar_clave_elem(cl, A->info);

    if (comp == 0) {        // cl == A->info
        *nodo = A->info;
    } else if (comp < 0) {  // cl < A->info
        buscar_nodo(A->izq, cl, nodo);
    } else {                // cl > A->info
        buscar_nodo(A->der, cl, nodo);
    }
}


// Función auxiliar para a reestruturación dun nodo da AVL, de ser necesario
void _reestruturar(avl *A) {
    /*
     * Só é necesaria a reestruturación nos seguintes casos:
     *      - O nodo pai (n) ten FE = 2 e o propio nodo (n1) ten FE >= 0: rotación DD
     *      - O nodo pai (n) ten FE = 2 e o propio nodo (n1) ten FE < 0: rotación DI
     *      - O nodo pai (n) ten FE = -2 e o propio nodo (n1) ten FE <= 0: rotación II
     *      - O nodo pai (n) ten FE = -2 e o propio nodo (n1) ten FE > 0: rotación ID
     */
    if ((*A)->pai->fe == 2) {
        if ((*A)->fe >= 0) { // Rotación DD
            (*A)->pai->der = (*A)->izq; // n->der = n1->izq;
            (*A)->izq = (*A)->pai;      // n1->izq = n;
            (*A)->pai = *A;             // n = n1;
            (*A)->pai->fe = 0;
            (*A)->fe = 0;
        } else {             // Rotación DI
            // Nota: n2 é o fillo esquerdo de n1
            (*A)->izq = (*A)->izq->der;      // n1->izq = n2->der;
            (*A)->izq->der = *A;             // n2->der = n1;
            (*A)->pai->der = (*A)->izq->izq; // n->der = n2->izq;
            (*A)->izq->izq = (*A)->pai;      // n2->izq = n;
            (*A)->pai = (*A)->izq;           // n = n2;
            (*A)->pai->fe = 0;
            (*A)->fe = 0;
        }
    } else if ((*A)->pai->fe == -2) {
        if ((*A)->fe <= 0) { // Rotación II
            (*A)->pai->izq = (*A)->der; // n->izq = n1->der;
            (*A)->der = (*A)->pai;      // n1->der = n;
            (*A)->pai = *A;             // n = n1;
            (*A)->pai->fe = 0;
            (*A)->fe = 0;
        } else {             // Rotación ID
            // Nota: n2 é o fillo dereito de n1
            (*A)->der = (*A)->der->izq;      // n1->der = n2->izq;
            (*A)->der->izq = *A;             // n2->izq = n1;
            (*A)->pai->izq = (*A)->der->der; // n->izq = n2->der;
            (*A)->der->der = (*A)->pai;      // n2->der = n;
            (*A)->pai = (*A)->der;           // n = n2;
            (*A)->pai->fe = 0;
            (*A)->fe = 0;
        }
    }
}


// Función que inserta un novo nodo na árbore (presuponse que non existe un nodo coa misma clave nesta)
void insertar(avl *A, tipoelem E) {
    if (vacia(*A)) { // Se é o primeiro nodo, corresponde á raíz
        *A = (avl) malloc(sizeof(struct celda));
        (*A)->info.comp_lexico = E.comp_lexico;
        (*A)->info.lexema = malloc((strlen(E.lexema) + 1) * sizeof(char));
        //  Súmaselle 1 para o '\0' que inserta a función strcpy()
        strcpy((*A)->info.lexema, E.lexema);
        // O pai inicialízase a NULL e especificarase cando se desapile esta chamada á función
        (*A)->pai = NULL;
        (*A)->izq = NULL;
        (*A)->der = NULL;
        (*A)->fe = 0;       // Ao ser un nodo folla, o FE deste será 0
        return;
    }

    // Aprovéitase o apilamento de chamadas a insertar() de forma recursiva para especificar o FE e o pai de cada nodo

    tipoclave cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if (comp > 0) {
        insertar(&(*A)->der, E);
        // Se non ten pai asignado (o caso dun nodo insertado na chamada á función anterior), engádeselle o nodo desta chamada á función
        if ((*A)->der->pai == NULL) {
            (*A)->der->pai = *A;
        }
        (*A)->fe++; // Se se inserta á dereita, súmase 1 ao FE do pai

        // Compróbase que o pai non sexa nulo, xa que para reestruturar a árbore debe ter mínimo altura 2
        //  Con altura 1, o pai do raíz é nulo
        if ((*A)->pai != NULL) {
            //_reestruturar(A);
        }
    } else {
        insertar(&(*A)->izq, E);
        // Se non ten pai asignado (o caso dun nodo insertado na chamada á función anterior), engádeselle o nodo desta chamada á función
        if ((*A)->izq->pai == NULL) {
            (*A)->izq->pai = *A;
        }
        (*A)->fe--; // Se se inserta á dereita, réstase 1 ao FE do pai

        // Compróbase que o pai non sexa nulo, xa que para reestruturar a árbore debe ter mínimo altura 2
        //  Con altura 1, o pai do raíz é nulo
        if ((*A)->pai != NULL) {
            //_reestruturar(A);
        }
    }
}