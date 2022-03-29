#include <stdio.h>
#include <string.h>

#include "taboaSimbolos.h"
#include "avl.h"


avl TS;


// Función auxiliar que realiza un recorrido inorde da árbore
void _auxImprimir(avl A) {
    tipoelem E;
    if (!vacia(A)) {
        _auxImprimir(izq(A));
        ler(A, &E);
        printf("\t< %d, %s >\n", E.comp_lexico, E.lexema);
        _auxImprimir(der(A));
    }
}

// Función que inicia a táboa de símbolos
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

// Función que busca un compoñente léxico concreto na táboa de símbolos
//  Se este compoñente léxico non está na táboa, insértao
void buscar_insertar(CompLexico *comp) {
    tipoelem comp_busqueda = {0, NULL};

    // Busca na TS un lexema concreto devolvéndoo en comp_busqueda
    buscar_nodo(TS, comp->lexema, &comp_busqueda);
    if (comp_busqueda.lexema == NULL) { // Se non está na TS, insértase e devólvese
        comp->comp_lexico = ID;
        insertar(&TS, *comp);
    } else {                            // Se está na TS, devólvese o atopado
        comp->comp_lexico = comp_busqueda.comp_lexico;
    }
}

// Función que finaliza a táboa de símbolos
void finalizarTS() {
    destruir(&TS);
}

// Función que imprime a táboa de símbolos por orde alfabético dos lexemas
void imprimirTS() {
    printf("\n\n----- TÁBOA DE SÍMBOLOS -----\n");
    _auxImprimir(TS);
    printf("-----------------------------\n\n\n");
}