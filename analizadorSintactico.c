#include <stdio.h>

#include "analizadorSintactico.h"
#include "analizadorLexico.h"


// Función que invoca ao analizador sintáctico
void analizadorSintactico(char *nomeFicheiro) {
    CompLexico comp;

    iniciarAnalizadorLexico(nomeFicheiro);

    printf("----- INICIO DA ANÁLISE -----\n");
    while ((comp = segCompLexico()).comp_lexico != EOF) {
        if (comp.lexema != NULL) { // Realízase esta comprobación xa que en caso de erro o lexema será NULL
            printf("< %d, %s >\n", comp.comp_lexico, comp.lexema);
        }
    }
    printf("-----  FIN DA ANÁLISE   -----\n");

    finalizarAnalizadorLexico();
}