#include <stdio.h>
#include <stdlib.h>

#include "sistemaEntrada.h"
#include "taboaSimbolos.h"
#include "analizadorSintactico.h"


// Só empeza e termina, chamando ao sintáctico que leva o control de tod e limpiando e acabando despois
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("\nNúmero de parámetros incorrecto.\n\tUsa %s [FICHEIRO_CODIGO_FONTE]\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    if (inicializar(argv[1]) == 1) {
        exit(EXIT_FAILURE);
    }



    pecharFicheiro();

    exit(EXIT_SUCCESS);
}
