#include <stdio.h>
#include <stdlib.h>

#include "sistemaEntrada.h"
#include "xestionErros.h"

#define TAMBUFFERS 20


FILE *f_codigo_fonte;

char *B1, *B2;
char *inicio, *dianteiro;
int ult_bloque;


void inicializarFicheiro(char *nomeFicheiro) {
    f_codigo_fonte = fopen(nomeFicheiro, "r");

    if (f_codigo_fonte == NULL) {
        notificarErro(FICHEIRO_NON_ATOPADO);
        exit(EXIT_FAILURE);
    }

    B1 = malloc(sizeof(char) * TAMBUFFERS);
    B2 = malloc(sizeof(char) * TAMBUFFERS);

    ult_bloque = 2;
    cargarSigBloque();
}

void cargarSigBloque() {
    if (ult_bloque == 1) {

    } else {

    }
}

void cambiarLexema() {

}

char segCaracter() {

}

void devolverCaracter(char c) {

}

void finalizarFicheiro() {
    free(B1);
    free(B2);
    free(inicio);
    free(dianteiro);
    free(f_codigo_fonte);
}