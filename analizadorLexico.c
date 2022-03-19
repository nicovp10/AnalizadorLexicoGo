#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "analizadorLexico.h"
#include "taboaSimbolos.h"
#include "sistemaEntrada.h"
#include "xestionErros.h"


CompLexico comp = {0, NULL};
int aceptado, erro;


void _alfanumerico(char char_previo) {
    int cont_chars = 0;
    char c;

    do {
        c = segCaracter();
        cont_chars++;
    } while (isalpha(c) || isdigit(c) || c == '_');

    devolverCaracter();
    aceptarLexema(&comp);

    // Se se leu só un caracter, compróbase se é "_" para indicar que é un BLANK_ID
    if (cont_chars == 1 && char_previo == '_') {
        comp.comp_lexico = BLANK_ID;
    } else if (comp.lexema != NULL) {   // Se non se excedeu o tamaño:
        buscar_insertar(&comp);
    }

    aceptado = 1;
}

void _numerico() {

}

void _strings(char char_previo) {
    if (char_previo == '"') {

    } else {

    }
}

void _comentarios() {
    int estado;
    char c;

    c = segCaracter();
    if (c == '/') {
        estado = 0;
    } else if (c == '*') {
        estado = 1;
    } else {
        lanzarErro(LEXEMA_MAL_FORMADO);
        erro = 1;
    }

    while (!aceptado && !erro) {
        switch (estado) {
            case 0: // Se é un comentario dunha liña
                do {
                    c = segCaracter();
                } while (c != '\n' && c != EOF);
                aceptado = 1;
                break;
            case 1: // Se é un comentario múltiples
                do {
                    c = segCaracter();
                } while (c != '*');
                estado = 2;
                break;
            case 2: // Se é un comentario múltiple e se leu un asterisco
                c = segCaracter();
                if (c == '/') {
                    aceptado = 1;
                } else {
                    estado = 1;
                }
                break;
        }
    }

    saltarLexema();
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
    char c = ' ';
    int estado = 0;
    _limparComp();

    aceptado = 0;
    erro = 0;

    // TODO dá fallo ás veces cando se excede o tamaño dun lexema
    while (c != EOF && !aceptado && !erro)
        switch (estado) {
            case 0:
                // Estado inicial do analizador léxico
                c = segCaracter();
                if (isalpha(c) || c == '_') {
                    _alfanumerico(c);   // Se comeza por unha letra ou _, AF de cadeas alfanuméricas
                } else if (c == '"' || c == '`') {
                    _strings(c);        // Se comeza por " ou `, AF de strings
                } else if (c == '/') {
                    _comentarios();                 // Se comeza por /, AF de comentarios
                } else {
                    ignorarCaracter();
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

    return comp;
}

// Finalización do analizador léxico
void finalizarAnalizadorLexico() {
    _limparComp();
    finalizarSistemaEntrada();
}