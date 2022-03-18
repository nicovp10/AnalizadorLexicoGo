#include <stdio.h>

#include "xestionErros.h"


void lanzarErro(int codigo) {
    switch (codigo) {
        case FICHEIRO_NON_ATOPADO:
            printf("\nNon se atopou o ficheiro de código fonte.\n\n");
            break;
        case LEXEMA_TAM_EXCEDIDO:
            printf("\nTamaño máximo de lexema excedido.\n\n");
            break;
        case LEXEMA_MAL_FORMADO:
            printf("\nDetectouse un lexema mal formado.\n\n");
            break;
        default:
            printf("Nunca debería chegar aquí");
    }
}