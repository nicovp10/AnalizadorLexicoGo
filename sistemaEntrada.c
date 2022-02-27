#include <stdio.h>
#include <stdlib.h>
#include "sistemaEntrada.h"


FILE *f_codigo_fonte;

int abrirFicheiro(char *nomeFicheiro) {
    f_codigo_fonte = fopen(nomeFicheiro, "r");

    if (f_codigo_fonte == NULL) {
        printf("Non se puido abrir o ficheiro co código fonte correctamente.\n");
        return 1;
    }

    return 0;
}

char segCaracter() {
    return (char) fgetc(f_codigo_fonte);
}

void pecharFicheiro() {
    free(f_codigo_fonte);
}