#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "analizadorLexico.h"
#include "taboaSimbolos.h"
#include "sistemaEntrada.h"


CompLexico comp = {0, NULL};
int aceptado;

void _alfanumerico() {
    int estado = 0;
    char c;

    while (!aceptado) {
        switch (estado) {
            case 0:
                do {
                    c = segCaracter();
                } while (isalpha(c) || isdigit(c) || c == '_');
                estado = 1;
                break;
            case 1:
                devolverCaracter();
                aceptarLexema(&comp);
                buscar_insertar(&comp);
                aceptado = 1;
                break;
        }
    }
}

void _numerico() {

}


void _limparComp() {
    // Se xa se lera un lexema, libérase a memoria asociada e reestablécense os valores
    if (comp.lexema != NULL) {
        free(comp.lexema);
        comp.comp_lexico = 0;
        comp.lexema = NULL;
    }
}

// Inicio do analizador léxico
void iniciarAnalizadorLexico(char *nomeFicheiro) {
    iniciarSistemaEntrada(nomeFicheiro);
}

CompLexico segCompLexico() {
    char c;
    int estado = 0;
    _limparComp();

    aceptado = 0;
    while ((c = segCaracter()) != EOF && !aceptado) {
        switch (estado) {
            case 0: // Estado inicial do analizador léxico
                if (c == ' ' || c == '\t' || c == '\n') {
                    ignorarCaracter();
                }
                else if (isalpha(c) || c == '_') {
                    _alfanumerico();    // Se comeza por unha letra ou _, AF de cadeas alfanuméricas
                } else {

                }
                break;
        }

        /*
        if (c == ' ') {
            aceptarLexema(&comp);
            aceptado = 1;
        }
        if (isalpha(c) || c == '_') {
            _alfanumerico();
        } else if (isdigit(c)) {
            _numerico();
        } else {

        }
         */
    }

    return comp;
}

// Finalización do analizador léxico
void finalizarAnalizadorLexico() {
    _limparComp();
    finalizarSistemaEntrada();
}