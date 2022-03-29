#include <stdlib.h>
#include <string.h>


#include "definicions.h"


/*
 * Optouse por unha implementación dunha árbore AVL para conseguir así unha mellor eficiencia computacional
 * nas búsquedas de lexemas na táboa de símbolos.
 *
 * Para a reestruturación da AVL usouse un factor de equilibrio (FE) en cada nodo, que será o valor resultante da operación:
 * altura(subarbore_dereita) - altura(subarbore_esquerda). Desta maneira saberase cando é necesario reestruturar a árbore.
 * Para a reestruturación xógase co desapilamento das chamadas recursivas á función insertar(), propagando o reaxuste dos
 * FE correspondentes cara arriba ata acadar un equilibrio. Tamén se empregou un punteiro ao pai de cada nodo para realizar as rotacións.
 */


typedef char *tipoclave;
typedef CompLexico tipoelem;

struct celda {
    tipoelem info;
    int fe; // Factor de equilibio do AVL
    struct celda *pai, *izq, *der;
};

typedef struct celda *avl;


int equilibrar; // Variable usada para saber cando se ten que deixar de propagar o equilibrio cara arriba


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


// Función auxiliar que realiza unha rotación DD a partir do nodo A
void _DD(avl *A) {
    avl n_pai = (*A)->pai;
    avl n1 = (*A)->der;
    avl n1_izq = n1->izq;

    (*A)->der = n1_izq; // n->der = n1->izq
    if (n1_izq != NULL) {
        n1_izq->pai = *A; // Se había un nodo en n1->izq, modifícase o pai de n1->izq ao novo (n)
    }

    n1->izq = *A; // n1->izq = n
    (*A)->pai = n1; // Modifícase o pai de n ao novo (n1)

    n1->pai = n_pai; // Modifícase o pai de n1 ao novo (pai de n)
    // n = n1
    if (n_pai == NULL) {
        *A = n1; // Se n non tiña pai, cámbiase n a n1
    } else {
        if (n_pai->izq == *A) { // Se o fillo esquerdo do pai de n era n, cámbiase a n1
            n_pai->izq = n1;
        } else {                // Se o fillo esquerdo do pai de n non era n, entón cámbiase o fillo dereito a n1
            n_pai->der = n1;
        }
    }

    // Modifícanse os factores de equilibrio
    (*A)->fe = 0;
    (*A)->izq->fe = 0;
}

// Función auxiliar que realiza unha rotación DI a partir do nodo A
void _DI(avl *A) {
    avl n_pai = (*A)->pai;
    avl n = *A;
    avl n2 = (*A)->der->izq; // n2 é o fillo esquerdo de n1, que á súa vez é o fillo dereito de n
    avl n2_der = n2->der;
    avl n2_izq = n2->izq;

    (*A)->der->izq = n2_der; // n1->izq = n2->der
    if (n2_der != NULL) {
        n2_der->pai = (*A)->der; // Se había un nodo en n2->der, modifícase o pai de n2->der ao novo (n1)
    }

    n2->der = (*A)->der; // n2->der = n1
    (*A)->der->pai = n2; // Modifícase o pai de n1 ao novo (n2)

    n->der = n2_izq; // n->der = n2->izq
    if (n2_izq != NULL) {
        n2_izq->pai = *A; // Se había un nodo en n2->izq, modifícase o pai de n2->izq ao novo (n)
    }

    n2->izq = n; // n2->izq = n
    (*A)->pai = n2; // Modifícase o pai de n ao novo (n2)

    n2->pai = n_pai; // Modifícase o pai de n2 ao novo (pai de n)
    // n = n2
    if (n_pai == NULL) {
        *A = n2; // Se n non tiña pai, cámbiase n a n2
    } else {
        if (n_pai->izq == *A) { // Se o fillo esquerdo do pai de n era n2, cámbiase a n2
            n_pai->izq = n2;
        } else {                // Se o fillo esquerdo do pai de n non era n2, entón cámbiase o fillo dereito a n2
            n_pai->der = n2;
        }
    }

    // Modifícanse os factores de equilibrio en función do resultado
    if ((*A)->fe == 1) {        // Se n2 (o novo n) ten FE = 1:
        (*A)->izq->fe = -1;
        (*A)->der->fe = 0;
    } else if ((*A)->fe == -1){ // Se non se ten FE = -1:
        (*A)->izq->fe = 0;
        (*A)->der->fe = 1;
    } else {                    // Se non se ten FE = 0:
        (*A)->izq->fe = 0;
        (*A)->der->fe = 0;
    }
    (*A)->fe = 0;
}

// Función auxiliar que realiza unha rotación II a partir do nodo A
void _II(avl *A) {
    avl n_pai = (*A)->pai;
    avl n1 = (*A)->izq;
    avl n1_der = n1->der;

    (*A)->izq = n1_der; // n->izq = n1->der
    if (n1_der != NULL) {
        n1_der->pai = *A; // Se había un nodo en n1->der, modifícase o pai de n1->der ao novo (n)
    }

    n1->der = *A; // n1->der = n
    (*A)->pai = n1; // Modifícase o pai de n ao novo (n1)

    n1->pai = n_pai; // Modifícase o pai de n1 ao novo (pai de n)
    // n = n1
    if (n_pai == NULL) {
        *A = n1; // Se n non tiña pai, cámbiase n a n1
    } else {
        if (n_pai->izq == *A) { // Se o fillo esquerdo do pai de n era n, cámbiase a n1
            n_pai->izq = n1;
        } else {                // Se o fillo esquerdo do pai de n non era n, entón cámbiase o fillo dereito a n1
            n_pai->der = n1;
        }
    }

    // Modifícanse os factores de equilibrio
    (*A)->fe = 0;
    (*A)->der->fe = 0;
}

// Función auxiliar que realiza unha rotación ID a partir dun nodo A
void _ID(avl *A) {
    avl n_pai = (*A)->pai;
    avl n = *A;
    avl n2 = (*A)->izq->der; // n2 é o fillo dereito de n1, que á súa vez é o fillo esquerdo de n
    avl n2_der = n2->der;
    avl n2_izq = n2->izq;

    (*A)->izq->der = n2_izq; // n1->der = n2->izq
    if (n2_izq != NULL) {
        n2_izq->pai = (*A)->izq; // Se había un nodo en n2->izq, modifícase o pai de n2->izq ao novo (n1)
    }

    n2->izq = (*A)->izq; // n2->izq = n1
    (*A)->izq->pai = n2; // Modifícase o pai de n1 ao novo (n2)

    n->izq = n2_der; // n->izq = n2->der
    if (n2_der != NULL) {
        n2_der->pai = *A; // Se había un nodo en n2->der, modifícase o pai de n2->der ao novo (n)
    }

    n2->der = n; // n2->der = n
    (*A)->pai = n2; // Modifícase o pai de n ao novo (n2)

    n2->pai = n_pai; // Modifícase o pai de n2 ao novo (pai de n)
    // n = n2
    if (n_pai == NULL) {
        *A = n2; // Se n non tiña pai, cámbiase n a n2
    } else {
        if (n_pai->izq == *A) { // Se o fillo esquerdo do pai de n era n, cámbiase a n2
            n_pai->izq = n2;
        } else {                // Se o fillo esquerdo do pai de n non era n, entón cámbiase o fillo dereito a n2
            n_pai->der = n2;
        }
    }

    // Modifícanse os factores de equilibrio en función do resultado
    if ((*A)->fe == 1) {        // Se n2 (o novo n) ten FE = 1:
        (*A)->izq->fe = -1;
        (*A)->der->fe = 0;
    } else if ((*A)->fe == -1){ // Se non se ten FE = -1:
        (*A)->izq->fe = 0;
        (*A)->der->fe = 1;
    } else {                    // Se non se ten FE = 0:
        (*A)->izq->fe = 0;
        (*A)->der->fe = 0;
    }
    (*A)->fe = 0;
}

// Función auxiliar para a reestruturación dun nodo da AVL, de ser necesario
void _reestruturar(avl *A) {
    /*
     * Só é necesaria a reestruturación nos seguintes casos:
     *      - O propio nodo (n) ten FE = 2 e o nodo fillo dereito (n1) ten FE >= 0: rotación DD
     *      - O propio nodo (n) ten FE = 2 e o nodo fillo dereito (n1) ten FE < 0: rotación DI
     *      - O propio nodo (n) ten FE = -2 e o nodo fillo esquerdo (n1) ten FE <= 0: rotación II
     *      - O propio nodo (n) ten FE = -2 e o nodo fillo esquerdo (n1) ten FE > 0: rotación ID
     */
    if ((*A)->fe == 2) {
        if ((*A)->der->fe >= 0) { // Rotación DD
            _DD(A);
        } else {                  // Rotación DI
            _DI(A);
        }
    } else if ((*A)->fe == -2) {
        if ((*A)->izq->fe <= 0) { // Rotación II
            _II(A);
        } else {                  // Rotación ID
            _ID(A);
        }
    }
}


// Función que inserta un novo nodo na árbore (presuponse que non existe un nodo coa misma clave nesta)
void insertar(avl *A, tipoelem E) {
    if (vacia(*A)) {
        *A = malloc(sizeof(struct celda));
        (*A)->info.comp_lexico = E.comp_lexico;
        (*A)->info.lexema = malloc((strlen(E.lexema) + 1) * sizeof(char));
        //  Súmaselle 1 para o '\0' que inserta a función strcpy()
        strcpy((*A)->info.lexema, E.lexema);
        (*A)->pai = NULL; // O pai inicialízase a NULL e especificarase cando se desapile esta chamada á función
        (*A)->izq = NULL;
        (*A)->der = NULL;
        (*A)->fe = 0; // Ao ser un nodo folla, o FE deste será 0
        equilibrar = 1; // Ao insertarse un nodo actívase o propagación para equilibrar os FE
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

        // Se hai que equilibrar, modifícase o FE
        if (equilibrar) {
            (*A)->fe++;
        }

        _reestruturar(A);

        // Se xa se chegou ao equilibrio, non se seguirá propagando o reaxuste cara arriba
        if ((*A)->fe == 0) {
            equilibrar = 0;
        }
    } else {
        insertar(&(*A)->izq, E);
        // Se non ten pai asignado (o caso dun nodo insertado na chamada á función anterior), engádeselle o nodo desta chamada á función
        if ((*A)->izq->pai == NULL) {
            (*A)->izq->pai = *A;
        }

        // Se hai que equilibrar, modifícase o FE
        if (equilibrar) {
            (*A)->fe--;
        }

        _reestruturar(A);

        // Se xa se chegou ao equilibrio, non se seguirá propagando o reaxuste cara arriba
        if ((*A)->fe == 0) {
            equilibrar = 0;
        }
    }

}