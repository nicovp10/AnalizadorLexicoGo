#include <stdio.h>

#include "xestionErros.h"


// Función que dado un código de erro imprime por pantalla unha mensaxe concreta
void lanzarErro(int codigo) {
    switch (codigo) {
        case FICHEIRO_NON_ATOPADO:
            printf("\nERRO: non se atopou o ficheiro de código fonte.\n\n");
            break;
        case LEXEMA_TAM_EXCEDIDO:
            printf("\nERRO: tamaño máximo de lexema excedido.\n\n");
            break;
        case COMENTARIO_MULTILINEA_NON_PECHADO:
            printf("\nERRO: comentario multiliña non pechado.\n\tDetalles: esperábanse os caracteres de peche '*\\'\n\n");
            break;
        case STRING_NON_PECHADO:
            printf("\nERRO: string non pechado.\n\tDetalles: esperábase o caracter de peche \" ou `\n\n");
            break;
        case BYTE_OCT_POUCOS_DIXITOS:
            printf("\nERRO: byte octal con poucos díxitos.\n\tDetalles: esperábanse 3 díxitos octais\n\n");
            break;
        case BYTE_HEX_POUCOS_DIXITOS:
            printf("\nERRO: byte hexadecimal con poucos díxitos.\n\tDetalles: esperábanse 2 díxitos hexadecimais\n\n");
            break;
        case UNICODE_PEQUENO_INVALIDO:
            printf("\nERRO: unicode pequeno inválido.\n\tDetalles: esperábanse 4 díxitos hexadecimais\n\n");
            break;
        case UNICODE_GRANDE_INVALIDO:
            printf("\nERRO: unicode grande inválido.\n\tDetalles: esperábanse 8 díxitos hexadecimais\n\n");
            break;
        case CARACTER_ESCAPADO_SECUENCIA_DESCONOCIDA:
            printf("\nERRO: secuencia de caracter escapado descoñecida.\n\n");
            break;
        case FLOAT_EXPO_MAL_FORMADO:
            printf("\nERRO: número en punto flotante mal formado.\n\tDetalles: expoñente mal formado\n\n");
            break;
        case FLOAT_HEX_EXPO_MAL_FORMADO:
            printf("\nERRO: hexadecimal en punto flotante mal formado.\n\tDetalles: esperábase como mínimo un número no expoñente\n\n");
            break;
        case IMAGINARY_MAL_FORMADO:
            printf("\nERRO: número imaxinario mal formado.\n\tDetalles: esperábase a letra final 'i'\n\n");
            break;
        case RUNA_NON_PECHADA:
            printf("\nERRO: runa non pechada.\n\tDetalles: esperábase o caracter de peche '\n\n");
            break;
        case RUNA_MOITOS_CARACTERES:
            printf("\nERRO: runa con moitos caracteres.\n\tDetalles: as runas só poden conter un caracter\n\n");
            break;
    }
}