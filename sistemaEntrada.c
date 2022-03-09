#include <stdio.h>
#include <stdlib.h>

#include "sistemaEntrada.h"
#include "xestionErros.h"


FILE *f_codigo_fonte;

int inicializar(char *nomeFicheiro) {
    f_codigo_fonte = fopen(nomeFicheiro, "r");

    if (f_codigo_fonte == NULL) {
        notificarErro(FICHEIRO_NON_ATOPADO);
        return 1;
    }

    return 0;
}

void cargarSigBloque() {

}

void cambiarLexema() {

}

char segCaracter() {
    return (char) fgetc(f_codigo_fonte);
    // cargar bloques con fread
    // para a programación dos bufferes mirar o sistema de entrada T2
}

void devolverCaracter(char c) {

}

void pecharFicheiro() {
    free(f_codigo_fonte);
}