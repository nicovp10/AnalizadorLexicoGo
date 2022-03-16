#include <stdio.h>

#include "xestionErros.h"


void lanzarErro(int codigo) {
    switch (codigo) {
        case FICHEIRO_NON_ATOPADO:
            printf("Non se atopou o ficheiro de código fonte.\n");
            break;
        default:
            printf("Nunca debería chegar aquí");
    }
}