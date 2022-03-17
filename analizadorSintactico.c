#include <stdio.h>

#include "analizadorSintactico.h"
#include "analizadorLexico.h"


void analizadorSintactico() {
    CompLexico comp;
    while ((comp = segCompLexico()).lexema != NULL) {
        printf("Comp. léxico: %d\t\tLexema: %s\n", comp.comp_lexico, comp.lexema);
    }
}