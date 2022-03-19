#include <stdio.h>

#include "analizadorSintactico.h"
#include "analizadorLexico.h"


void analizadorSintactico(char *nomeFicheiro) {
    CompLexico comp;

    iniciarAnalizadorLexico(nomeFicheiro);

    while ((comp = segCompLexico()).comp_lexico != EOF) {
        if (comp.lexema != NULL) {
            printf("< %d, %s >\n", comp.comp_lexico, comp.lexema);
        }
    }

    finalizarAnalizadorLexico();
}