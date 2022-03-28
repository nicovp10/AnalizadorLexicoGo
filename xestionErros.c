#include <stdio.h>

#include "xestionErros.h"


// Función que dado un código de erro imprime por pantalla unha mensaxe concreta
void lanzarErro(int linea, int codigo) {
    switch (codigo) {
        case FICHEIRO_NON_ATOPADO:
            printf("\nErro: non se atopou o ficheiro de código fonte.\n\n");
            break;
        case LEXEMA_TAM_EXCEDIDO:
            printf("\nErro na liña %d: tamaño máximo de lexema excedido.\n\n", linea);
            break;
        case COMENTARIO_MULTILINEA_NON_PECHADO:
            printf("\nErro na liña %d: comentario multiliña non pechado.\n\tDetalles: esperábanse os caracteres de peche '*\\'\n\n", linea);
            break;
        case STRING_NON_PECHADO:
            printf("\nErro na liña %d: string non pechado.\n\tDetalles: esperábase o caracter de peche \" ou `\n\n", linea);
            break;
        case BYTE_OCT_POUCOS_DIXITOS:
            printf("\nErro na liña %d: byte octal con poucos díxitos.\n\tDetalles: esperábanse 3 díxitos octais\n\n", linea);
            break;
        case BYTE_HEX_POUCOS_DIXITOS:
            printf("\nErro na liña %d: byte hexadecimal con poucos díxitos.\n\tDetalles: esperábanse 2 díxitos hexadecimais\n\n", linea);
            break;
        case UNICODE_PEQUENO_INVALIDO:
            printf("\nErro na liña %d: unicode pequeno inválido.\n\tDetalles: esperábanse 4 díxitos hexadecimais\n\n", linea);
            break;
        case UNICODE_GRANDE_INVALIDO:
            printf("\nErro na liña %d: unicode grande inválido.\n\tDetalles: esperábanse 8 díxitos hexadecimais\n\n", linea);
            break;
        case CARACTER_ESCAPADO_SECUENCIA_DESCONOCIDA:
            printf("\nErro na liña %d: secuencia de caracter escapado descoñecida.\n\n", linea);
            break;
        case FLOAT_EXPO_MAL_FORMADO:
            printf("\nErro na liña %d: número en punto flotante mal formado.\n\tDetalles: expoñente mal formado\n\n", linea);
            break;
        case FLOAT_HEX_EXPO_MAL_FORMADO:
            printf("\nErro na liña %d: hexadecimal en punto flotante mal formado.\n\tDetalles: esperábase como mínimo un número no expoñente\n\n", linea);
            break;
        case IMAGINARY_MAL_FORMADO:
            printf("\nErro na liña %d: número imaxinario mal formado.\n\tDetalles: esperábase a letra final 'i'\n\n", linea);
            break;
        case RUNA_NON_PECHADA:
            printf("\nErro na liña %d: runa non pechada.\n\tDetalles: esperábase o caracter de peche '\n\n", linea);
            break;
        case RUNA_MOITOS_CARACTERES:
            printf("\nErro na liña %d: runa con moitos caracteres.\n\tDetalles: as runas só poden conter un caracter\n\n", linea);
            break;
    }
}