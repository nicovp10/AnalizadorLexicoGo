#include <stdio.h>

#include "analizadorSintactico.h"
#include "analizadorLexico.h"


void analizadorSintactico() {
    CompLexico comp;
    while ((comp = segCompLexico()).lexema != NULL) {
        printf("Comp. léxico: %d\tLexema: %s", comp.comp_lexico, comp.lexema);
    }
}