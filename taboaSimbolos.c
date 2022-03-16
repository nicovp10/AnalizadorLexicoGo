#include <stdlib.h>

#include "taboaSimbolos.h"
#include "abb.h"


abb TS;


void iniciarTS() {
    CompLexico keywords[] = {
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
    for (int i = 0; i < (sizeof(keywords) / sizeof(CompLexico)); i++) {
        insertar(&TS, keywords[i]);
    }
}

CompLexico buscar_insertar(CompLexico comp_input) {
    CompLexico comp_busqueda = {0, NULL};

    // Busca na TS un lexema concreto devolvéndoo en comp_busqueda
    buscar_nodo(TS, comp_input.lexema, &comp_busqueda);
    if (comp_busqueda.lexema == NULL) { // Se non está na TS, insértase e devólvese
        comp_input.comp_lexico = ID;
        insertar(&TS, comp_input);
        return comp_input;
    }

    // Se está na TS, devólvese o atopado
    return comp_busqueda;
}

void finalizarTS() {
    destruir(&TS);
}