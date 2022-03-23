#include <stdio.h>

#include "xestionErros.h"


void lanzarErro(int codigo) {
    switch (codigo) {
        case FICHEIRO_NON_ATOPADO:
            printf("\nERRO: non se atopou o ficheiro de código fonte.\n\n");
            break;
        case LEXEMA_DESCONOCIDO:
            printf("\nERRO: lexema descoñecido.\n\n");
            break;
        case LEXEMA_TAM_EXCEDIDO:
            printf("\nERRO: tamaño máximo de lexema excedido.\n\n");
            break;
        case COMENTARIO_MULTILINEA_NON_PECHADO:
            printf("\nERRO: comentario multiliña non pechado.\n\n");
            break;
        case STRING_NON_PECHADO:
            printf("\nERRO: string non pechado.\n\n");
            break;
        case BYTE_OCT_POUCOS_DIXITOS:
            printf("\nERRO: byte octal con poucos díxitos.\n\n");
            break;
        case BYTE_HEX_POUCOS_DIXITOS:
            printf("\nERRO: byte hexadecimal con poucos díxitos.\n\n");
            break;
        case UNICODE_PEQUENO_INVALIDO:
            printf("\nERRO: unicode pequeno inválido.\n\n");
            break;
        case UNICODE_GRANDE_INVALIDO:
            printf("\nERRO: unicode grande inválido.\n\n");
            break;
        case CARACTER_ESCAPADO_SECUENCIA_DESCONOCIDA:
            printf("\nERRO: secuencia de caracter escapado descoñecida.\n\n");
            break;
        case INT_MAL_FORMADO:
            printf("\nERRO: enteiro mal formado, esperábase un número tras a _.\n\n");
            break;
        case RUNA_NON_PECHADA:
            printf("\nERRO: runa non pechada.\n\n");
            break;
        case RUNA_MOITOS_CARACTERES:
            printf("\nERRO: runa con moitos caracteres.\n\n");
            break;
    }
}