#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "analizadorLexico.h"
#include "taboaSimbolos.h"
#include "sistemaEntrada.h"
#include "xestionErros.h"


CompLexico comp = {0, NULL};
int aceptado, erro;
char c = ' ';


/* Función auxiliar que le un número concreto de díxitos hexadecimais
 *      return 0: éxito
 *      return 1: erro
 */
int _lerDixitosHex(int n) {
    for (int i = 0; i < n; i++) {
        c = segCaracter();
        if (!isxdigit(c)) { // Se non é un díxito hexadecimal:
            return 1;
        }
    }
    return 0;
}

/* Función auxiliar que comproba se un caracter concreto é un díxito octal
 *      return 0: éxito
 *      return 1: erro
 */
int _serOctal(char caracter) {
    if ((caracter >= 48) && (caracter <= 55)) {
        return 1;
    }
    return 0;
}

// Función que acepta un lexema
void _aceptarLexema(int codigo_comp, int chars_a_devolver) {
    for (int i = 0; i < chars_a_devolver; i++) {
        devolverCaracter();
    }
    aceptarLexema(&comp);
    comp.comp_lexico = codigo_comp;
    aceptado = 1;
}

// Función que lanza un erro
void _lanzarErro(int codigo, int chars_a_devolver) {
    for (int i = 0; i < chars_a_devolver; i++) {
        devolverCaracter();
    }
    lanzarErro(codigo);
    erro = 1;
}


void _alfanumerico() {
    do {
        c = segCaracter();
    } while (isalpha(c) || isdigit(c) || c == '_');

    devolverCaracter();
    aceptarLexema(&comp);
    buscar_insertar(&comp);
    aceptado = 1;
}

void _numerico() {
    int i, estado, empezarPunto = 0, contaPuntos = 0;

    switch (c) {
        case '0': // Pode ser diversos tipos de número: decimal (0), binario, octal, hexadecimal ou imaxinario
            estado = 1;
            break;
        case '.': // decimal_float_lit, '.' ou PUNTOTRIPLE
            estado = 2;
            empezarPunto = 1;
            contaPuntos++;
            break;
        default: // decimal_lit ou decimal_float_lit
            estado = 0;
            break;
    }

    do {
        c = segCaracter();
        switch (estado) {
            case 0: // decimal_lit ou decimal_float_lit
                switch (c) {
                    case '_': // Se se le '_', tense que ler un número posteriormente
                        estado = 3;
                        break;
                    case '.': // Se se le '.', tense que ler a parte decimal dun decimal_float_lit
                        estado = 2;
                        break;
                    case 'e': // Se se le 'e' ou 'E', tense que ler o expoñente dun decimal_float_lit
                    case 'E':
                        estado = 6;
                        break;
                    case 'i': // Se se le 'i', é IMAGINARY e acéptase
                        _aceptarLexema(IMAGINARY, 0);
                        break;
                    default:    // Se non se le ningunha das letras anteriores nin un número, é INT e acéptase
                        if (!isdigit(c)) {
                            // Devólvese o caracter que activou a condición de aceptación
                            _aceptarLexema(INT, 1);
                        }
                        break;
                }
                break;
            case 1: // Pode ser diversos tipos de número: decimal (0), binario, octal, hexadecimal ou imaxinario
                switch (c) {
                    case 'b':
                    case 'B': // binary_lit
                        estado = 10;
                        break;
                    case 'o':
                    case 'O': // octal_lit
                        estado = 14;
                        break;
                    case '_': // octal_lit ou IMAGINARY
                        estado = 15;
                        break;
                    case 'x':
                    case 'X': // hex_lit ou hex_float_lit
                        estado = 21;
                        break;
                    default:
                        if (_serOctal(c)) { // octal_lit que comeza por 0, sen letra
                            estado = 17;
                        } else if (c == '8' || c == '9') {  // Inicio dun posible IMAGINARY
                            estado = 19;
                        } else { // Se non se le nada do indicado anteriormente, é INT e acéptase
                            // Devólvese o caracter que activou a condición de aceptación
                            _aceptarLexema(INT, 1);
                        }
                        break;
                }
                break;
            case 2: // decimal_lit e leuse '.'
                switch (c) {
                    case 'e':
                    case 'E':               // Se se le 'e' ou 'E', tense que ler o expoñente dun decimal_float_lit
                        estado = 6;
                        break;
                    default:
                        if (isdigit(c)) {   // Se se le un número, tense que ler a parte decimal dun decimal_float_lit
                            estado = 4;
                        } else {            // Se non se lé nin 'e' ou 'E' nin un número, acéptase
                            // Devólvese o caracter que activou a condición de aceptación
                            devolverCaracter();
                            if (empezarPunto) {     // Se se empezou recoñecendo un punto, ao non ter parte enteira non é un número, se non un punto.
                                //  Hai que comprobar se poden ser tres puntos
                                do {
                                    c = segCaracter();
                                    if (c == '.') {
                                        contaPuntos++;
                                    }
                                } while (contaPuntos < 3 && c == '.');

                                // Se o while rematou porque leu un caracter que non era un punto, este devólvese
                                if (c != '.') {
                                    devolverCaracter();
                                }

                                if (contaPuntos == 3) {
                                    _aceptarLexema(PUNTOTRIPLE, 0);
                                } else {
                                    _aceptarLexema('.', 0);
                                }
                            } else {                // Se non se empezou recoñecendo un punto, é FLOAT
                                _aceptarLexema(FLOAT, 0);
                            }
                        }
                        break;
                }
                break;
            case 3: // decimal_lit e leuse '_' entre números
                if (isdigit(c)) {   // Se se le un número, vólvese ao estado anterior
                    estado = 0;
                } else {            // Se non se le un número, é INT e acéptase
                    // Devólvense dous caracteres: (...)_{c}
                    _aceptarLexema(INT, 2);
                    break;
                }
                break;
            case 4: // Parte decimal dun decimal_float_lit
                switch (c) {
                    case '_': // Se se le '_', tense que ler un número posteriormente
                        estado = 5;
                        break;
                    case 'e': // Se se le 'e' ou 'E', tense que ler o expoñente
                    case 'E':
                        estado = 6;
                        break;
                    case 'i': // Se se le 'i', é IMAGINARY e acéptase
                        _aceptarLexema(IMAGINARY, 0);
                        break;
                    default: // Se non se le ningunha das letras anteriores nin un número, é FLOAT e acéptase
                        // Devólvese o caracter que activou a condición de aceptación
                        if (!isdigit(c)) {
                            _aceptarLexema(FLOAT, 1);
                        }
                        break;
                }
                break;
            case 5: // Parte decimal dun decimal_float_lit e leuse '_' entre números
                if (isdigit(c)) {   // Se se le un número, vólvese ao estado anterior
                    estado = 4;
                } else {            // Se non se le un número, é FLOAT e acéptase
                    // Devólvense dous caracteres: (...)_{c}
                    _aceptarLexema(FLOAT, 2);
                }
                break;
            case 6: // Inicio do expoñente dun decimal_float_lit
                switch (c) {
                    case '+':
                    case '-': // Se se le un signo, é un expoñente de decimal_float_lit con signo
                        estado = 7;
                        break;
                    default:
                        if (isdigit(c)) { // Se se le un número, tense que ler a parte numérica do expoñente
                            estado = 8;
                        } else {          // Se non se le un número, lánzase un erro porque o expoñente non está ben formado
                            // Devólvense dous caracteres: (...)e{c}
                            _lanzarErro(FLOAT_EXPO_MAL_FORMADO, 2);
                        }
                        break;
                }
            case 7: // Expoñente do decimal_float_lit con signo
                if (isdigit(c)) { // Se se le un número, tense que ler a parte numérica do expoñente
                    estado = 8;
                } else {          // Se non se le un número, lánzase un erro porque o expoñente non está ben formado
                    // Devólvense dous caracteres: (...){signo}{c}
                    _lanzarErro(FLOAT_EXPO_MAL_FORMADO, 2);
                }
                break;
            case 8: // Díxitos do expoñente do decimal_float_lit
                switch (c) {
                    case '_': // Se se le '_', tense que ler un número posteriormente
                        estado = 9;
                        break;
                    case 'i': // Se se le 'i', é IMAGINARY e acéptase
                        _aceptarLexema(IMAGINARY, 0);
                        break;
                    default: // Acéptase cando o que continúa non é un número
                        if (!isdigit(c)) {
                            // Devólvese o caracter que activou a condición de aceptación
                            _aceptarLexema(FLOAT, 1);
                        }
                        break;
                }
                break;
            case 9: // Expoñente dun FLOAT e leuse '_' entre números
                if (isdigit(c)) {   // Se se le un número, vólvese ao estado anterior
                    estado = 8;
                } else {            // Se non se le un número, é FLOAT e acéptase:
                    // Devólvense dous caracteres: (...)_{c}
                    _aceptarLexema(FLOAT, 2);
                }
                break;
            case 10: // binary_lit: estado inicial
                switch (c) {
                    case '_': // Se se le '_' inicial, tense que comprobar que posteriormente veña un díxito binario
                        estado = 11;
                        break;
                    case '0':
                    case '1': // Se se le un díxito binario, tense que ler a parte numérica do binary_lit
                        estado = 12;
                        break;
                    default: // Se non se le nin '_' nin un díxito binario, devólvese o número '0'
                        // Devólvense dous caracteres: (...)b{c}
                        _aceptarLexema(INT, 2);
                        break;
                }
                break;
            case 11: // binary_lit e leuse '_' inicial
                switch (c) {
                    case '0':
                    case '1': // Se se le un díxito binario, tense que ler a parte numérica do binary_lit
                        estado = 12;
                        break;
                    default: // Se despois de '_' non vén un díxito binario, devólvese o número '0'
                        // Devólvense tres caracteres: (...)b_{c}
                        _aceptarLexema(INT, 3);
                        break;
                }
                break;
            case 12: // binary_lit e leuse un díxito binario
                switch (c) {
                    case '0':
                    case '1':
                        break;
                    case 'i': // Se se le 'i', é IMAGINARY e acéptase
                        _aceptarLexema(IMAGINARY, 0);
                        break;
                    case '_': // Se se le '_', tense que ler un número posteriormente
                        estado = 13;
                        break;
                    default: // Se non se le nin '_' nin 'i' nin un díxito binario, é INT e acéptase
                        // Devólvese o caracter que activou a condición de aceptación
                        _aceptarLexema(INT, 1);
                        break;
                }
                break;
            case 13: // binary_lit e lese '_' entre díxitos binarios
                switch (c) {
                    case '0':
                    case '1': // Se se le un díxito binario, vólvese ao estado anterior
                        estado = 12;
                        break;
                    default: // Se non se le un díxito binario, é INT e acéptase
                        // Devólvense dous caracteres: (...)_{c}
                        _aceptarLexema(INT, 2);
                        break;
                }
                break;
            case 14: // octal_lit con letra: estado inicial
                if (c == '_') {                     // Se se le '_' inicial, tense que comprobar que posteriormente veña un díxito octal
                    estado = 16;
                } else if (_serOctal(c)) {  // Se se le un díxito octal, tense que ler a parte numérica do octal_lit
                    estado = 17;
                } else {                            // Se non se le nin '_' nin un díxito octal, devólvese o número '0'
                    // Devólvense dous caracteres: (...)o{c}
                    _aceptarLexema(INT, 2);
                }
                break;
            case 15: // octal_lit sen letra: estado inicial
                if (_serOctal(c)) { // Se se le un díxito octal, tense que ler a parte numérica do octal_lit
                    estado = 17;
                } else if (isdigit(c)) {    // Se non se le un díxito octal pero si un número, posible IMAGINARY
                    estado = 21;
                } else {                    // Se non se le ningún número, devólvese o número '0'
                    // Devólvense dous caracteres: (...)_{c}
                    _aceptarLexema(INT, 2);
                }
                break;
            case 16: // octal_lit con letra e leuse '_' inicial
                if (_serOctal(c)) { // Se se le un díxito octal, tense que ler a parte numérica do octal_lit
                    estado = 17;
                } else {                    // Se despois de '_' non vén un díxito octal, devólvese o número '0'
                    // Devólvense tres caracteres: (...)o_{c}
                    _aceptarLexema(INT, 3);
                }
                break;
            case 17: // octal_lit con ou sen letra e leuse un díxito octal
                switch (c) {
                    case '_': // Se se le '_', tense que ler un número posteriormente
                        estado = 18;
                        break;
                    case 'i': // Se se le 'i', é IMAGINARY e acéptase
                        _aceptarLexema(IMAGINARY, 0);
                        break;
                    default: // Se non se le nin '_' nin 'i' nin un díxito octal, é INT e acéptase
                        if (!_serOctal(c)) {
                            // Devólvese o caracter que activou a condición de aceptación
                            _aceptarLexema(INT, 1);
                        }
                        break;
                }
                break;
            case 18: // octal_lit e lese '_' entre díxitos octais
                if (_serOctal(c)) { // Se se le un díxito octal, vólvese ao estado anterior
                    estado = 17;
                } else {                    // Se non se le un díxito octal, é INT e acéptase
                    // Devólvense dous caracteres: (...)_{c}
                    _aceptarLexema(INT, 2);
                }
                break;
            case 19: // Número comezado por '0' e que non lle segue un díxito octal: posible IMAGINARY
                switch (c) {
                    case 'i':   // Se se le 'i', é IMAGINARY e acéptase
                        _aceptarLexema(IMAGINARY, 0);
                        break;
                    case '_':   // Se se le '_', tense que ler un número posteriormente
                        estado = 20;
                        break;
                    default:
                        if (!isdigit(c)) {  // Se non se le ningún dos caracteres anteriores nin un número,
                                            //  lánzase erro xa que non corresponde a ningún tipo de número
                            // Devólvese o caracter que activou a condición de aceptación
                            _lanzarErro(IMAGINARY_MAL_FORMADO, 1);
                        }
                        break;
                }
                break;
            case 20: // Posible IMAGINARY e leuse '_'
                if (isdigit(c)) {   // Se se le un número, vólvese ao estado anterior
                    estado = 19;
                } else {            // Se non se le un número, lánzase erro xa que non corresponde a ningún tipo de número
                    // Devólvese o caracter que activou a condición de erro
                    _lanzarErro(IMAGINARY_MAL_FORMADO, 1);
                }
                break;
            case 21: // hex_lit ou hex_float_lit: estado inicial
                switch (c) {
                    case '_': // Se se le '_' inicial, tense que comprobar que posteriormente veña un díxito hexadecimal
                        estado = 22;
                        break;
                    case '.': // Se se le '.', tense que ler a parte decimal dun hex_float_lit
                        estado = 25;
                        break;
                    default:
                        if (isxdigit(c)) { // Se se le un díxito hexadecimal, tense que ler a parte enteira dun hex_lit ou hex_float_lit
                            estado = 23;
                        } else {           // Se non se le un díxito hexadecimal, devólvese o número '0'
                            // Devólvense dous caracteres: (...)x{c}
                            _aceptarLexema(INT, 2);
                        }
                        break;
                }
                break;
            case 22: // hex_lit e leuse '_' inicial
                if (isxdigit(c)) { // Se se le un díxito hexadecimal, tense que ler a parte enteira dun hex_lit ou hex_float_lit
                    estado = 23;
                } else {           // Se non se le un díxito hexadecimal, devólvese o número '0'
                    // Devólvense tres caracteres: (...)x_{c}
                    _aceptarLexema(INT, 3);
                }
                break;
            case 23: // hex_lit e leuse un díxito hexadecimal
                switch (c) {
                    case 'i': // Se se le 'i', é IMAGINARY e acéptase
                        _aceptarLexema(IMAGINARY, 0);
                        break;
                    case '_': // Se se le '_', tense que ler un número posteriormente
                        estado = 24;
                        break;
                    case '.': // Se se le '.', tense que ler a parte decimal dun hex_float_lit
                        estado = 25;
                        break;
                    case 'p':
                    case 'P': // Se se le 'p' ou 'P', tense que ler o expoñente dun hex_float_lit
                        estado = 28;
                        break;
                    default: // Se non se le ningunha das letras anteriores nin un díxito hexadecimal, é INT e acéptase
                        if (!isxdigit(c)) {
                            // Devólvese o caracter que activou a condición de aceptación
                            _aceptarLexema(INT, 1);
                        }
                        break;
                }
                break;
            case 24: // hex_lit e lese '_' entre díxitos
                if (isxdigit(c)) { // Se se le un díxito hexadecimal, vólvese ao estado anterior
                    estado = 23;
                } else {           // Se non se le un díxito hexadecimal, é INT e acéptase
                    // Devólvense dous caracteres: (...)_{c}
                    _aceptarLexema(INT, 2);
                }
                break;
            case 25: // hex_lit e leuse '.'
                switch (c) {
                    case 'p':
                    case 'P': // Se se le 'p' ou 'P', tense que ler o expoñente dun hex_float_lit
                        estado = 28;
                        break;
                    default:
                        if (isxdigit(c)) { // Se se le un díxito hexadecimal, tense que ler a parte decimal dun hex_float_lit
                            estado = 26;
                        } else {           // Se non se le un díxito hexadecimal, é INT e acéptase
                            // Devólvense dous caracteres: (...).{c}
                            _aceptarLexema(INT, 2);
                        }
                }
                break;
            case 26: // Parte decimal dun hex_float_lit
                switch (c) {
                    case 'p':
                    case 'P': // Se se le 'p' ou 'P', tense que ler o expoñente
                        estado = 28;
                        break;
                    case '_': // Se se le '_', tense que ler un número posteriormente
                        estado = 27;
                        break;
                    default:
                        if (!isxdigit(c)) { // Se non se le un díxito hexadecimal,
                                            //  lánzase erro xa que se esperaba o expoñente
                            // Devólvese o caracter que activou a condición de erro
                            _lanzarErro(FLOAT_HEX_MAL_FORMADO, 1);
                        }
                }
                break;
            case 27: // Parte decimal dun hex_float_lit e leuse '_' entre díxitos
                if (isxdigit(c)) { // Se se le un díxito hexadecimal, vólvese ao estado anterior
                    estado = 26;
                } else {           // Se non se le un díxito hexadecimal,
                                   //  lánzase erro xa que se esperaba o expoñente
                    // Devólvense dous caracteres: (...)_{c}
                    _lanzarErro(FLOAT_HEX_MAL_FORMADO, 2);
                }
                break;
            case 28: // Inicio do expoñente dun hex_float_lit
                switch (c) {
                    case '+':
                    case '-': // Se se le un signo, é un expoñente de hex_float_lit con signo
                        estado = 29;
                        break;
                    default:
                        if (isdigit(c)) { // Se se le un número, tense que ler a parte numérica do expoñente
                            estado = 30;
                        } else {          // Se non se le un número, lánzase un erro por expoñente mal formado
                            // Devólvense dous caracteres: (...)p{c}
                            _lanzarErro(FLOAT_EXPO_MAL_FORMADO, 2);
                        }
                }
                break;
            case 29: // Expoñente dun hex_float_lit con signo
                if (isdigit(c)) { // Se se le un número, tense que ler a parte numérica do expoñente
                    estado = 30;
                } else {          // Se non se le un número, lánzase un erro por expoñente mal formado
                    // Devólvense dous caracteres: (...){signo}{c}
                    _lanzarErro(FLOAT_EXPO_MAL_FORMADO, 2);
                }
                break;
            case 30: // Expoñente dun hex_float_lit e leuse un número
                switch (c) {
                    case 'i': // Se se le 'i', é IMAGINARY e acéptase
                        _aceptarLexema(IMAGINARY, 0);
                        break;
                    case '_': // Se se le '_', tense que ler un número posteriormente
                        estado = 31;
                        break;
                    default: // Se non se le nin 'i' nin '_' nin un díxito hexadecimal, é FLOAT e acéptase
                        if (!isdigit(c)) {
                            // Devólvese o caracter que activou a condición de aceptación
                            _aceptarLexema(FLOAT, 1);
                        }
                }
                break;
            case 31: // Expoñente dun hex_float_lit e leuse '_' entre díxitos
                if (isdigit(c)) { // Se se le un díxito hexadecimal, vólvese ao estado anterior
                    estado = 30;
                } else {          // Se non se le un díxito hexadecimal, é FLOAT e acéptase
                    // Devólvense dous caracteres: (...)_{c}
                    _aceptarLexema(FLOAT, 2);
                }
                break;
        }
    } while (c != EOF && !aceptado && !erro);

    if (erro) {
        saltarLexema();
    }
}

void _rune() {  // Moi similar a un string interpretado, pero dun só caracter
    int estado = 0, escapado, cont_chars = -1;
    // Inicialízase cont_chars a -1 para que non teña en conta a ' de peche

    do {
        escapado = 0;
        c = segCaracter();
        switch (estado) {
            case 0:
                if (c == '\\') { // Se se le '\', tense que ler un unicode_char
                    estado = 1;
                }
                cont_chars++; // Indica o número de caracteres lidos (menos 1)
                break;
            case 1: // Inicio dun unicode_char
                switch (c) {
                    case 'x':   // Se é un byte hexadecimal:
                        // Lense os díxitos hexadecimais correspondentes
                        if (_lerDixitosHex(2)) {
                            // Devólvese o caracter que activou a condición de erro
                            _lanzarErro(BYTE_HEX_POUCOS_DIXITOS, 1);
                        }
                        break;
                    case 'u':   // Se é un unicode pequeno:
                        if (_lerDixitosHex(4)) {
                            _lanzarErro(UNICODE_PEQUENO_INVALIDO, 1);
                        }
                        break;
                    case 'U':   // Se é un unicode grande:
                        if (_lerDixitosHex(8)) {
                            _lanzarErro(UNICODE_GRANDE_INVALIDO, 1);
                        }
                        break;
                    default:    // Se é un byte octal ou caracter escapado:
                        if (_serOctal(c)) {
                            for (int i = 0; i < 2; i++) {    // Lense os dous valores octais que faltan
                                c = segCaracter();
                                if (!_serOctal(c)) { // Se non son valores octais:
                                    _lanzarErro(BYTE_OCT_POUCOS_DIXITOS, 1);
                                    break;
                                }
                            }
                        } else if (c != 'a' && c != 'b' && c != 'f' && c != 'n' &&
                                   c != 'r' && c != 't' && c != 'v' && c != '\\' &&
                                   c != '\'' && c != '"') {
                            // Compróbase se non é un caracter escapado
                            // Devólvese o caracter que activou a condición de erro
                            _lanzarErro(CARACTER_ESCAPADO_SECUENCIA_DESCONOCIDA, 1);
                        } else {
                            escapado = 1;
                        }
                        break;
                }
                estado = 0;
                break;
        }
    } while ((c != '\'' || escapado) && c != EOF);

    if (c == EOF) {
        _lanzarErro(RUNA_NON_PECHADA, 0);
    } else if (cont_chars > 1 && !erro) {
        _lanzarErro(RUNA_MOITOS_CARACTERES, 0);
    }

    if (erro) {
        saltarLexema();
    } else {
        _aceptarLexema(RUNE, 0);
    }
}

void _strings() {
    int estado = 0, escapado;

    if (c == '"') {   // Se é un string interpretado:
        do {
            escapado = 0;
            c = segCaracter();
            switch (estado) {
                case 0:
                    if (c == '\\') { // Se se le '\', tense que ler un unicode_char
                        estado = 1;
                    }
                    break;
                case 1: // Inicio dun unicode_char
                    switch (c) {
                        case 'x':   // Se é un byte hexadecimal:
                            // Lense os díxitos hexadecimais correspondentes
                            if (_lerDixitosHex(2)) {
                                _lanzarErro(BYTE_HEX_POUCOS_DIXITOS, 1);
                            }
                            break;
                        case 'u':   // Se é un unicode pequeno:
                            if (_lerDixitosHex(4)) {
                                _lanzarErro(UNICODE_PEQUENO_INVALIDO, 1);
                            }
                            break;
                        case 'U':   // Se é un unicode grande:
                            if (_lerDixitosHex(8)) {
                                _lanzarErro(UNICODE_GRANDE_INVALIDO, 1);
                            }
                            break;
                        default:    // Se é un byte octal ou caracter escapado:
                            if (_serOctal(c)) {   // Se é un valor octal
                                for (int i = 0; i < 2; i++) {   // Lense os dous valores octais que faltan
                                    c = segCaracter();
                                    if (!_serOctal(c)) {   // Se non son valores octais:
                                        _lanzarErro(BYTE_OCT_POUCOS_DIXITOS, 1);
                                        break;
                                    }
                                }
                            } else if (c != 'a' && c != 'b' && c != 'f' && c != 'n' &&
                                       c != 'r' && c != 't' && c != 'v' && c != '\\' &&
                                       c != '\'' && c != '"') {
                                // Compróbase se non é un caracter escapado
                                _lanzarErro(CARACTER_ESCAPADO_SECUENCIA_DESCONOCIDA, 1);
                            } else {
                                escapado = 1;
                            }
                            break;
                    }
                    estado = 0;
                    break;
            }
        } while ((c != '"' || escapado) && c != EOF);
    } else {                    // Se é un string crudo:
        do {
            c = segCaracter();
        } while (c != '`' && c != EOF);
    }

    if (c == EOF) {
        _lanzarErro(STRING_NON_PECHADO, 0);
    } else if (erro) {
        saltarLexema();
    } else {
        _aceptarLexema(STRING, 0);
    }
}

void _comentarios() {
    int estado;

    c = segCaracter();
    if (c == '/') {
        estado = 0;
    } else if (c == '*') {
        estado = 1;
    } else {
        estado = 3;
    }

    while (!aceptado && !erro) {
        switch (estado) {
            case 0: // Comentario dunha liña
                do {
                    c = segCaracter();
                } while (c != '\n' && c != EOF);
                saltarLexema();
                aceptado = 1;
                break;
            case 1: // Comentario de múltiples liñas
                do {
                    c = segCaracter();
                } while (c != '*' && c != EOF);
                if (c == EOF) {
                    _lanzarErro(COMENTARIO_MULTILINEA_NON_PECHADO, 0);
                    saltarLexema();
                    erro = 1;
                } else {
                    estado = 2;
                }
                break;
            case 2: // Se é un comentario múltiple e se leu un *:
                c = segCaracter();
                if (c == '/') {
                    saltarLexema();
                    aceptado = 1;
                } else {
                    estado = 1;
                }
                break;
            case 3: // Se despois da primeira / vén algo distinto de / ou *, é o operador de división
                // Devólvese o caracter que activou a condición de erro
                _aceptarLexema('/', 1);
                break;
        }
    }
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
    int estado = 0;
    _limparComp();

    aceptado = 0;
    erro = 0;

    while (c != EOF && !aceptado && !erro) {
        switch (estado) {
            case 0: // Estado inicial do analizador léxico
                c = segCaracter();
                if (isalpha(c) || c == '_') {       // Se comeza por unha letra ou _, AF de cadeas alfanuméricas
                    _alfanumerico();                //      Acepta ID e keywords
                } else if (isdigit(c) || c == '.') {// Se comeza por un número ou ., AF de cadeas numéricas
                    _numerico();                    //      Acepta INT, FLOAT, IMAGINARY, . e PUNTOTRIPLE
                } else {
                    switch (c) {
                        case '\'':                  // Se comeza por ', AF de runas
                            _rune();                //      Acepta RUNE
                            break;
                        case '"':
                        case '`':                   // Se comeza por " ou `, AF de strings
                            _strings();             //      Acepta STRING
                            break;
                        case '/':                   // Se comeza por /, AF de comentarios
                            _comentarios();         //      Acepta / e sáltase os comentarios
                            break;
                        case EOF:                   // Se é EOF, indícase para finalizar
                            comp.lexema = NULL;
                            comp.comp_lexico = EOF;
                            break;
                        default:                    // Se non comeza por ningún dos anteriores, ignórase o caracter
                            ignorarCaracter();      //      Está pensado para os \t, \n e espacios do código
                            break;
                    }
                }
                break;
        }
    }

    return comp;
}

// Finalización do analizador léxico
void finalizarAnalizadorLexico() {
    _limparComp();
    finalizarSistemaEntrada();
}
