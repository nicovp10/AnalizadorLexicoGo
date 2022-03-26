#include <stdio.h>
#include <stdlib.h>

#include "analizadorSintactico.h"
#include "taboaSimbolos.h"


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("\nERRO: número de parámetros incorrecto.\n\tUsa %s [FICHEIRO_CODIGO_FONTE]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    iniciarTS();
    imprimirTS();

    analizadorSintactico(argv[1]);

    imprimirTS();
    finalizarTS();

    exit(0);
}
