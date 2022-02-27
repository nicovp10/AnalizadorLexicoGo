#include <stdio.h>

#include "sistemaEntrada.h"


FILE *codigo_fonte;


void abrirFicheiro(char* nomeFicheiro) {
    codigo_fonte = fopen(nomeFicheiro, "r");

    if (codigo_fonte == NULL) {
        printf("Non se puido abrir o ficheiro co código fonte correctamente.\n");
    }
}

char segCaracter() {
    return (char) fgetc(codigo_fonte);
}