#include <stdio.h>

#include "xestionErros.h"


void notificarErro(int codigo) {
    switch (codigo) {
        case FICHEIRO_NON_ATOPADO:
            printf("Non se atopou o ficheiro de código fonte.");
        default:
            printf("Nunca debería chegar aquí");
    }
}