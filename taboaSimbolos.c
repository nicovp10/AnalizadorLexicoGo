#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "taboaSimbolos.h"
#include "abb.h"


abb TS;


// Recorrido inorde da árbore
void _auxImprimir(abb A) {
    tipoelem E;
    if (!vacia(A)) {
        _auxImprimir(izq(A));
        ler(A, &E);
        printf("\t< %d, %s >\n", E.comp_lexico, E.lexema);
        _auxImprimir(der(A));
    }
}


void iniciarTS() {
    tipoelem keywords[] = {
            {BREAK,       "break"},
            {CASE,        "case"},
            {CHAN,        "chan"},
            {CONST,       "const"},
            {CONTINUE,    "continue"},
            {DEFAULT,     "default"},
            {DEFER,       "defer"},
            {ELSE,        "else"},
            {FALLTHROUGH, "fallthrough"},
            {FOR,         "for"},
            {FUNC,        "func"},
            {GO,          "go"},
            {GOTO,        "goto"},
            {IF,          "if"},
            {IMPORT,      "import"},
            {INTERFACE,   "interface"},
            {MAP,         "map"},
            {PACKAGE,     "package"},
            {RANGE,       "range"},
            {RETURN,      "return"},
            {SELECT,      "select"},
            {STRUCT,      "struct"},
            {SWITCH,      "switch"},
            {TYPE,        "type"},
            {VAR,         "var"}
    };

    crear(&TS);
    for (int i = 0; i < (sizeof(keywords) / sizeof(tipoelem)); i++) {
        insertar(&TS, keywords[i]);
    }
}

void buscar_insertar(CompLexico *comp) {
    tipoelem comp_busqueda = {0, NULL};

    // Busca na TS un lexema concreto devolvéndoo en comp_busqueda
    buscar_nodo(TS, comp->lexema, &comp_busqueda);
    if (comp_busqueda.lexema == NULL) { // Se non está na TS, insértase e devólvese
        comp->comp_lexico = ID;
        insertar(&TS, *comp);
    } else {
        // Se está na TS, devólvese o atopado
        comp->comp_lexico = comp_busqueda.comp_lexico;
    }
}

void finalizarTS() {
    destruir(&TS);
}

void imprimirTS() {
    printf("\n\n----- TÁBOA DE SÍMBOLOS -----\n");
    _auxImprimir(TS);
    printf("-----------------------------\n\n\n");
}