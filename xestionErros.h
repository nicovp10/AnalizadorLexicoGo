#ifndef P1_XESTIONERROS_H
#define P1_XESTIONERROS_H


#define FICHEIRO_NON_ATOPADO 1
#define LEXEMA_DESCONOCIDO 2
#define LEXEMA_TAM_EXCEDIDO 3
#define COMENTARIO_MULTILINEA_NON_PECHADO 4
#define STRING_CRUDO_NON_PECHADO 5
#define STRING_INTERPRETADO_NON_PECHADO 6
#define BYTE_OCT_POUCOS_DIXITOS 7
#define BYTE_HEX_POUCOS_DIXITOS 8
#define UNICODE_PEQUENO_INVALIDO 9
#define UNICODE_GRANDE_INVALIDO 10
#define CARACTER_ESCAPADO_SECUENCIA_DESCONOCIDA 11
#define INT_MAL_FORMADO 12
#define RUNA_NON_PECHADA 13
#define RUNA_MOITOS_CARACTERES 14


void lanzarErro(int codigo);


#endif
