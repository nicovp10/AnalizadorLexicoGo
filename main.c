#include <stdio.h>
#include <stdlib.h>

#include "analizadorSintactico.h"
#include "taboaSimbolos.h"


// Só empeza e termina, inicializando, chamando ao sintáctico que leva o control de tod e limpiando e acabando despois
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("\nNúmero de parámetros incorrecto.\n\tUsa %s [FICHEIRO_CODIGO_FONTE]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    iniciarTS();
    imprimirTS();

    analizadorSintactico(argv[1]);

    imprimirTS();
    finalizarTS();

    exit(EXIT_SUCCESS);
}
