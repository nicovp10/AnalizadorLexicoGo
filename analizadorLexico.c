#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "analizadorLexico.h"
#include "taboaSimbolos.h"
#include "sistemaEntrada.h"
#include "xestionErros.h"


CompLexico comp = {0, NULL};

// Estas tres variables serven para controlar o caracter actual, a liña do código actual e
//  se un lexema foi aceptado ou hai algún erro
char c_actual = ' ';
int linea_actual = 1;
int aceptado, erro;


/* Función auxiliar que le un número concreto de díxitos hexadecimais
 *      return 0: éxito
 *      return 1: erro
 */
int _lerDixitosHex(int n) {
    for (int i = 0; i < n; i++) {
        c_actual = segCaracter();
        if (!isxdigit(c_actual)) { // Se non é un díxito hexadecimal, devolve erro
            return 1;
        }
    }
    return 0;
}

/* Función auxiliar que comproba se un caracter concreto é un díxito octal
 *      return 0: éxito
 *      return 1: erro
 */
int _serOctal(int caracter) {
    // Compárase o caracter cos valores ASCII correspondentes a 0 (48) e 7 (55)
    if ((caracter >= 48) && (caracter <= 55)) {
        return 1;
    }
    return 0;
}

// Función auxiliar que acepta un lexema
void _aceptarLexema(int codigo_comp, int chars_a_devolver) {
    for (int i = 0; i < chars_a_devolver; i++) {
        devolverCaracter();
    }
    aceptarLexema(&comp);
    comp.comp_lexico = codigo_comp;
    aceptado = 1;
}

// Función auxiliar que lanza un erro
void _lanzarErro(int codigo_erro, int chars_a_devolver) {
    for (int i = 0; i < chars_a_devolver; i++) {
        devolverCaracter();
    }
    lanzarErro(linea_actual, codigo_erro);
    erro = 1;
}


// Función auxiliar correspondente ao AF de cadeas alfanuméricas
void _alfanumerico() {
    do {
        c_actual = segCaracter();
    } while (isalpha(c_actual) || isdigit(c_actual) || c_actual == '_');

    devolverCaracter();
    aceptarLexema(&comp);

    // Comprobación necesaria xa que en caso de exceder o tamaño, devolverá NULL
    if (comp.lexema != NULL) {
        buscar_insertar(&comp);
    }
    aceptado = 1;
}

// Función auxiliar correspondente ao AF de cadeas numéricas
void _numerico() {
    int estado, empezarPunto = 0, contaPuntos = 0;

    switch (c_actual) {
        case '0': // Poden ser diversos tipos de número: decimal_lit ('0'), bin_lit, octal_lit,
            //                                        hex_lit, hex_float_lit ou IMAGINARY
            estado = 1;
            break;
        case '.': // decimal_float_lit, '.' ou PUNTOTRIPLE
            estado = 2;
            empezarPunto = 1; // Esta variable serve para aceptar '.' e PUNTOTRIPLE
            contaPuntos++;    // Esta variable serve para levar a conta dos puntos lidos
            //    Ver estado 2 -> caso default do switch
            break;
        default: // decimal_lit ou decimal_float_lit
            estado = 0;
            break;
    }

    do {
        c_actual = segCaracter();
        switch (estado) {
            case 0: // decimal_lit ou decimal_float_lit
                switch (c_actual) {
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
                    default:  // Se non se le ningún dos caracteres anteriores nin un número, é INT e acéptase
                        if (!isdigit(c_actual)) {
                            // Devólvese o caracter que activou a condición de aceptación
                            _aceptarLexema(INT, 1);
                        }
                        break;
                }
                break;
            case 1: // Poden ser diversos tipos de número: decimal_lit ('0'), bin_lit, octal_lit,
                //                                      hex_lit, hex_float_lit ou IMAGINARY
                switch (c_actual) {
                    case 'i': // Se se le 'i', é IMAGINARY e acéptase
                        _aceptarLexema(IMAGINARY, 0);
                        break;
                    case 'b':
                    case 'B': // Se se le 'b' ou 'B', é binary_lit
                        estado = 10;
                        break;
                    case 'o':
                    case 'O': // Se se le 'o' ou 'O', é octal_lit
                        estado = 14;
                        break;
                    case '_': // Se se le '_', pode ser octal_lit ou IMAGINARY
                        estado = 15;
                        break;
                    case 'x':
                    case 'X': // Se se le 'x' ou 'X', pode ser hex_lit ou hex_float_lit
                        estado = 21;
                        break;
                    default:
                        if (_serOctal(c_actual)) { // Se é un díxito octal, é octal_lit que comeza por 0, sen letra o ou O
                            estado = 17;
                        } else if (c_actual == '8' || c_actual == '9') {  // Se non é díxito octal, é o inicio dun IMAGINARY
                            estado = 19;
                        } else { // Se non se le nada do indicado anteriormente, é INT e acéptase
                            // Devólvese o caracter que activou a condición de aceptación
                            _aceptarLexema(INT, 1);
                        }
                        break;
                }
                break;
            case 2: // decimal_lit e leuse '.'
                switch (c_actual) {
                    case 'e':
                    case 'E':               // Se se le 'e' ou 'E', tense que ler o expoñente dun decimal_float_lit
                        estado = 6;
                        break;
                    default:
                        if (isdigit(c_actual)) {   // Se se le un número, tense que ler a parte decimal dun decimal_float_lit
                            estado = 4;
                        } else {            // Se non se lé nin 'e' ou 'E' nin un número, realízanse unha serie de
                            //  comprobacións para comprobar de que compoñente léxico se trata
                            // Devólvese o caracter que activou a condición de aceptación
                            devolverCaracter();
                            if (empezarPunto) {     // Se se empezou recoñecendo un punto, ao non ter parte enteira non é un número, se non un punto.
                                // Compróbase se pode ser PUNTOTRIPLE
                                do {
                                    c_actual = segCaracter();
                                    if (c_actual == '.') {
                                        contaPuntos++;
                                    }
                                } while (contaPuntos < 3 && c_actual == '.');


                                if (contaPuntos == 3) { // Se se contaron 3 puntos, é PUNTORIPLE e acéptase
                                    _aceptarLexema(PUNTOTRIPLE, 0);
                                } else {                // Se non se contaron 3 puntos, é '.' e acéptase
                                    // Se o while rematou recoñecendo dous puntos,
                                    //  devólvese un caracter extra para solucionar o desfase do segundo punto
                                    if (contaPuntos == 2) {
                                        devolverCaracter();
                                    }

                                    // Devólvese o caracter que provocou a parada do while
                                    _aceptarLexema('.', 1);
                                }
                            } else {                // Se non se empezou recoñecendo un punto, é FLOAT e acéptase
                                _aceptarLexema(FLOAT, 0);
                            }
                        }
                        break;
                }
                break;
            case 3: // decimal_lit e leuse '_' entre números
                if (isdigit(c_actual)) {   // Se se le un número, vólvese ao estado anterior
                    estado = 0;
                } else {            // Se non se le un número, é INT e acéptase
                    // Devólvense dous caracteres: (...)_{c_actual}
                    _aceptarLexema(INT, 2);
                    break;
                }
                break;
            case 4: // Parte decimal dun decimal_float_lit
                switch (c_actual) {
                    case '_': // Se se le '_', tense que ler un número posteriormente
                        estado = 5;
                        break;
                    case 'e':
                    case 'E': // Se se le 'e' ou 'E', tense que ler o expoñente
                        estado = 6;
                        break;
                    case 'i': // Se se le 'i', é IMAGINARY e acéptase
                        _aceptarLexema(IMAGINARY, 0);
                        break;
                    default: // Se non se le ningún dos caracteres anteriores nin un número, é FLOAT e acéptase
                        // Devólvese o caracter que activou a condición de aceptación
                        if (!isdigit(c_actual)) {
                            _aceptarLexema(FLOAT, 1);
                        }
                        break;
                }
                break;
            case 5: // Parte decimal dun decimal_float_lit e leuse '_' entre números
                if (isdigit(c_actual)) {   // Se se le un número, vólvese ao estado anterior
                    estado = 4;
                } else {            // Se non se le un número, é FLOAT e acéptase
                    // Devólvense dous caracteres: (...)_{c_actual}
                    _aceptarLexema(FLOAT, 2);
                }
                break;
            case 6: // Inicio do expoñente dun decimal_float_lit
                switch (c_actual) {
                    case '+':
                    case '-': // Se se le un signo, é un expoñente de decimal_float_lit con signo
                        estado = 7;
                        break;
                    default:
                        if (isdigit(c_actual)) { // Se se le un número, tense que ler a parte numérica do expoñente
                            estado = 8;
                        } else {          // Se non se le un número, lánzase un erro porque o expoñente non está ben formado
                            // Devólvense dous caracteres: (...)e{c_actual} ou (...)E{c_actual}
                            _lanzarErro(FLOAT_EXPO_MAL_FORMADO, 2);
                        }
                        break;
                }
                break;
            case 7: // Expoñente dun decimal_float_lit con signo
                if (isdigit(c_actual)) { // Se se le un número, tense que ler a parte numérica do expoñente
                    estado = 8;
                } else {          // Se non se le un número, lánzase un erro porque o expoñente non está ben formado
                    // Devólvense dous caracteres: (...)e+{c_actual} ou (...)e-{c_actual}
                    _lanzarErro(FLOAT_EXPO_MAL_FORMADO, 3);
                }
                break;
            case 8: // Díxitos do expoñente dun decimal_float_lit
                switch (c_actual) {
                    case '_': // Se se le '_', tense que ler un número posteriormente
                        estado = 9;
                        break;
                    case 'i': // Se se le 'i', é IMAGINARY e acéptase
                        _aceptarLexema(IMAGINARY, 0);
                        break;
                    default: // Se non se le nin '_' nin 'i' nin un número, é FLOAT e acéptase
                        if (!isdigit(c_actual)) {
                            // Devólvese o caracter que activou a condición de aceptación
                            _aceptarLexema(FLOAT, 1);
                        }
                        break;
                }
                break;
            case 9: // Expoñente dun decimal_float_lit e leuse '_' entre números
                if (isdigit(c_actual)) {   // Se se le un número, vólvese ao estado anterior
                    estado = 8;
                } else {            // Se non se le un número, é FLOAT e acéptase:
                    // Devólvense dous caracteres: (...)_{c_actual}
                    _aceptarLexema(FLOAT, 2);
                }
                break;
            case 10: // Inicio dun binary_lit
                switch (c_actual) {
                    case '_': // Se se le '_' inicial, tense que comprobar que posteriormente veña un díxito binario
                        estado = 11;
                        break;
                    case '0':
                    case '1': // Se se le un díxito binario, tense que ler a parte numérica do binary_lit
                        estado = 12;
                        break;
                    default: // Se non se le nin '_' nin un díxito binario, devólvese o número '0'
                        // Devólvense dous caracteres: (...)b{c_actual}
                        _aceptarLexema(INT, 2);
                        break;
                }
                break;
            case 11: // binary_lit e leuse '_' inicial
                switch (c_actual) {
                    case '0':
                    case '1': // Se se le un díxito binario, tense que ler a parte numérica do binary_lit
                        estado = 12;
                        break;
                    default: // Se non se le un díxito binario, devólvese o número '0'
                        // Devólvense tres caracteres: (...)b_{c_actual}
                        _aceptarLexema(INT, 3);
                        break;
                }
                break;
            case 12: // binary_lit e leuse un díxito binario
                switch (c_actual) {
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
            case 13: // binary_lit e leuse '_' entre díxitos binarios
                switch (c_actual) {
                    case '0':
                    case '1': // Se se le un díxito binario, vólvese ao estado anterior
                        estado = 12;
                        break;
                    default: // Se non se le un díxito binario, é INT e acéptase
                        // Devólvense dous caracteres: (...)_{c_actual}
                        _aceptarLexema(INT, 2);
                        break;
                }
                break;
            case 14: // Inicio dun octal_lit con letra o ou O
                if (c_actual == '_') {                     // Se se le '_' inicial, tense que comprobar que posteriormente veña un díxito octal
                    estado = 16;
                } else if (_serOctal(c_actual)) { // Se se le un díxito octal, tense que ler a parte numérica do octal_lit
                    estado = 17;
                } else {                           // Se non se le nin '_' nin un díxito octal, devólvese o número '0'
                    // Devólvense dous caracteres: (...)o{c_actual} ou (...)O{c_actual}
                    _aceptarLexema(INT, 2);
                }
                break;
            case 15: // Inicio dun octal_lit sen letra o ou O
                if (_serOctal(c_actual)) { // Se se le un díxito octal, tense que ler a parte numérica do octal_lit
                    estado = 17;
                } else if (isdigit(c_actual)) {    // Se non se le un díxito octal pero si un número, posible IMAGINARY
                    estado = 21;
                } else {                    // Se non se le ningún número, devólvese o número '0'
                    // Devólvense dous caracteres: (...)_{c_actual}
                    _aceptarLexema(INT, 2);
                }
                break;
            case 16: // octal_lit con letra e leuse '_' inicial
                if (_serOctal(c_actual)) { // Se se le un díxito octal, tense que ler a parte numérica do octal_lit
                    estado = 17;
                } else {                    // Se despois de '_' non vén un díxito octal, devólvese o número '0'
                    // Devólvense tres caracteres: (...)o_{c_actual}
                    _aceptarLexema(INT, 3);
                }
                break;
            case 17: // octal_lit (con ou sen letra) e leuse un díxito octal
                switch (c_actual) {
                    case '_': // Se se le '_', tense que ler un número posteriormente
                        estado = 18;
                        break;
                    case 'i': // Se se le 'i', é IMAGINARY e acéptase
                        _aceptarLexema(IMAGINARY, 0);
                        break;
                    default: // Se non se le nin '_' nin 'i' nin un díxito octal, é INT e acéptase
                        if (!_serOctal(c_actual)) {
                            // Devólvese o caracter que activou a condición de aceptación
                            _aceptarLexema(INT, 1);
                        }
                        break;
                }
                break;
            case 18: // octal_lit e lese '_' entre díxitos octais
                if (_serOctal(c_actual)) { // Se se le un díxito octal, vólvese ao estado anterior
                    estado = 17;
                } else {                    // Se non se le un díxito octal, é INT e acéptase
                    // Devólvense dous caracteres: (...)_{c_actual}
                    _aceptarLexema(INT, 2);
                }
                break;
            case 19: // Número comezado por '0' e que non lle segue un díxito octal: IMAGINARY
                switch (c_actual) {
                    case 'i':   // Se se le 'i', é IMAGINARY e acéptase
                        _aceptarLexema(IMAGINARY, 0);
                        break;
                    case '_':   // Se se le '_', tense que ler un número posteriormente
                        estado = 20;
                        break;
                    default:
                        if (!isdigit(c_actual)) {  // Se non se le ningún dos caracteres anteriores nin un número,
                            //  lánzase erro xa que non corresponde a ningún tipo de número
                            // Devólvese o caracter que activou a condición de aceptación
                            _lanzarErro(IMAGINARY_MAL_FORMADO, 1);
                        }
                        break;
                }
                break;
            case 20: // IMAGINARY e leuse '_'
                if (isdigit(c_actual)) {   // Se se le un número, vólvese ao estado anterior
                    estado = 19;
                } else {            // Se non se le un número, lánzase erro xa que non corresponde a ningún tipo de número
                    // Devólvese o caracter que activou a condición de erro
                    _lanzarErro(IMAGINARY_MAL_FORMADO, 1);
                }
                break;
            case 21: // Inicio dun hex_lit ou hex_float_lit
                switch (c_actual) {
                    case '_': // Se se le '_' inicial, tense que comprobar que posteriormente veña un díxito hexadecimal
                        estado = 22;
                        break;
                    case '.': // Se se le '.', tense que ler a parte decimal dun hex_float_lit
                        estado = 25;
                        break;
                    default:
                        if (isxdigit(
                                c_actual)) { // Se se le un díxito hexadecimal, tense que ler a parte enteira dun hex_lit ou hex_float_lit
                            estado = 23;
                        } else {           // Se non se le un díxito hexadecimal, devólvese o número '0'
                            // Devólvense dous caracteres: (...)x{c_actual} ou (...)X{c_actual}
                            _aceptarLexema(INT, 2);
                        }
                        break;
                }
                break;
            case 22: // hex_lit ou hex_float_lit e leuse '_' inicial
                if (isxdigit(
                        c_actual)) { // Se se le un díxito hexadecimal, tense que ler a parte enteira dun hex_lit ou hex_float_lit
                    estado = 23;
                } else {           // Se non se le un díxito hexadecimal, devólvese o número '0'
                    // Devólvense tres caracteres: (...)x_{c_actual} ou (...)X_{c_actual}
                    _aceptarLexema(INT, 3);
                }
                break;
            case 23: // hex_lit ou hex_float_lit e leuse un díxito hexadecimal
                switch (c_actual) {
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
                    default: // Se non se le ningún dos caracteres anteriores nin un díxito hexadecimal, é INT e acéptase
                        if (!isxdigit(c_actual)) {
                            // Devólvese o caracter que activou a condición de aceptación
                            _aceptarLexema(INT, 1);
                        }
                        break;
                }
                break;
            case 24: // Parte enteira dun hex_lit ou hex_float_lit e lese '_' entre díxitos
                if (isxdigit(c_actual)) { // Se se le un díxito hexadecimal, vólvese ao estado anterior
                    estado = 23;
                } else {           // Se non se le un díxito hexadecimal, é INT e acéptase
                    // Devólvense dous caracteres: (...)_{c_actual}
                    _aceptarLexema(INT, 2);
                }
                break;
            case 25: // hex_lit e leuse '.'
                switch (c_actual) {
                    case 'p':
                    case 'P': // Se se le 'p' ou 'P', tense que ler o expoñente dun hex_float_lit
                        estado = 28;
                        break;
                    default:
                        if (isxdigit(
                                c_actual)) { // Se se le un díxito hexadecimal, tense que ler a parte decimal dun hex_float_lit
                            estado = 26;
                        } else {           // Se non se le nin 'p' ou 'P' nin un díxito hexadecimal, é INT e acéptase
                            // Devólvense dous caracteres: (...).{c_actual}
                            _aceptarLexema(INT, 2);
                        }
                }
                break;
            case 26: // Parte decimal dun hex_float_lit
                switch (c_actual) {
                    case 'p':
                    case 'P': // Se se le 'p' ou 'P', tense que ler o expoñente
                        estado = 28;
                        break;
                    case '_': // Se se le '_', tense que ler un número posteriormente
                        estado = 27;
                        break;
                    default:
                        if (!isxdigit(c_actual)) { // Se non se le nin 'p' ou 'P' nin '_' nin un díxito hexadecimal,
                            //  lánzase erro xa que se esperaba o expoñente
                            // Devólvese o caracter que activou a condición de erro
                            _lanzarErro(FLOAT_HEX_EXPO_MAL_FORMADO, 1);
                        }
                }
                break;
            case 27: // Parte decimal dun hex_float_lit e leuse '_' entre díxitos
                if (isxdigit(c_actual)) { // Se se le un díxito hexadecimal, vólvese ao estado anterior
                    estado = 26;
                } else {           // Se non se le un díxito hexadecimal,
                    //  lánzase erro xa que se esperaba o expoñente
                    // Devólvense dous caracteres: (...)_{c_actual}
                    _lanzarErro(FLOAT_HEX_EXPO_MAL_FORMADO, 2);
                }
                break;
            case 28: // Inicio do expoñente dun hex_float_lit
                switch (c_actual) {
                    case '+':
                    case '-': // Se se le un signo, é un expoñente de hex_float_lit con signo
                        estado = 29;
                        break;
                    default:
                        if (isdigit(c_actual)) { // Se se le un número, tense que ler a parte numérica do expoñente
                            estado = 30;
                        } else {          // Se non se le un número, lánzase un erro por expoñente mal formado
                            // Devólvense dous caracteres: (...)p{c_actual}
                            _lanzarErro(FLOAT_EXPO_MAL_FORMADO, 2);
                        }
                }
                break;
            case 29: // Expoñente dun hex_float_lit con signo
                if (isdigit(c_actual)) { // Se se le un número, tense que ler a parte numérica do expoñente
                    estado = 30;
                } else {          // Se non se le un número, lánzase un erro por expoñente mal formado
                    // Devólvense dous caracteres: (...){signo}{c_actual}
                    _lanzarErro(FLOAT_EXPO_MAL_FORMADO, 2);
                }
                break;
            case 30: // Expoñente dun hex_float_lit e leuse un número
                switch (c_actual) {
                    case 'i': // Se se le 'i', é IMAGINARY e acéptase
                        _aceptarLexema(IMAGINARY, 0);
                        break;
                    case '_': // Se se le '_', tense que ler un número posteriormente
                        estado = 31;
                        break;
                    default: // Se non se le nin 'i' nin '_' nin un díxito hexadecimal, é FLOAT e acéptase
                        if (!isdigit(c_actual)) {
                            // Devólvese o caracter que activou a condición de aceptación
                            _aceptarLexema(FLOAT, 1);
                        }
                }
                break;
            case 31: // Expoñente dun hex_float_lit e leuse '_' entre díxitos
                if (isdigit(c_actual)) { // Se se le un díxito hexadecimal, vólvese ao estado anterior
                    estado = 30;
                } else {          // Se non se le un díxito hexadecimal, é FLOAT e acéptase
                    // Devólvense dous caracteres: (...)_{c_actual}
                    _aceptarLexema(FLOAT, 2);
                }
                break;
        }
    } while (c_actual != EOF && !aceptado && !erro);

    if (erro) {
        saltarLexema();
    }
}

// Función auxiliar correspondente ao AF de runas
void _rune() {  // Moi similar a un string interpretado, pero dun só caracter
    int estado = 0, escapado, cont_chars = -1;
    // Inicialízase cont_chars a -1 para que non teña en conta a ' de peche

    do {
        escapado = 0;
        c_actual = segCaracter();
        switch (estado) {
            case 0:
                if (c_actual == '\\') { // Se se le '\', tense que ler un unicode_char
                    estado = 1;
                }
                cont_chars++; // Indica o número de caracteres lidos (menos 1)
                break;
            case 1: // Inicio dun unicode_char
                switch (c_actual) {
                    case 'x':   // Se é un byte hexadecimal:
                        // Lense dous díxitos hexadecimais
                        if (_lerDixitosHex(2)) {
                            // Devólvese o caracter que activou a condición de erro
                            _lanzarErro(BYTE_HEX_POUCOS_DIXITOS, 1);
                        }
                        break;
                    case 'u':   // Se é un unicode pequeno:
                        // Lense catro díxitos hexadecimais
                        if (_lerDixitosHex(4)) {
                            _lanzarErro(UNICODE_PEQUENO_INVALIDO, 1);
                        }
                        break;
                    case 'U':   // Se é un unicode grande:
                        // Lense oito díxitos hexadecimais
                        if (_lerDixitosHex(8)) {
                            _lanzarErro(UNICODE_GRANDE_INVALIDO, 1);
                        }
                        break;
                    default:    // Se é un byte octal ou caracter escapado:
                        if (_serOctal(c_actual)) {
                            for (int i = 0; i < 2; i++) {    // Lense os dous valores octais que faltan
                                c_actual = segCaracter();
                                if (!_serOctal(c_actual)) { // Se non son valores octais,
                                    //     lánzase un erro por ser poucos díxitos octais
                                    _lanzarErro(BYTE_OCT_POUCOS_DIXITOS, 1);
                                    break;
                                }
                            }
                        } else if (c_actual != 'a' && c_actual != 'b' && c_actual != 'f' && c_actual != 'n' &&
                                   c_actual != 'r' && c_actual != 't' && c_actual != 'v' && c_actual != '\\' &&
                                   c_actual != '\'' && c_actual != '"') {
                            // Se non é un caracter escapado, lánzase un erro por secuencia descoñecida
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
    } while ((c_actual != '\'' || escapado) && c_actual != EOF);

    if (c_actual == EOF) { // Se se chegou ao fin de ficheiro e non se pechou, lánzase un erro por runa sen pechar
        _lanzarErro(RUNA_NON_PECHADA, 0);
    } else if (cont_chars > 1 && !erro) { // Se hai máis dun caracter na runa, lánzase un erro por moitos caracteres
        _lanzarErro(RUNA_MOITOS_CARACTERES, 0);
    }

    if (erro) {
        saltarLexema();
    } else {
        _aceptarLexema(RUNE, 0);
    }
}

// Función auxiliar correspondente ao AF de strings
void _strings() {
    int estado = 0, escapado;

    if (c_actual == '"') {   // Se é un string interpretado:
        do {
            escapado = 0;
            c_actual = segCaracter();
            switch (estado) {
                case 0:
                    if (c_actual == '\\') { // Se se le '\', tense que ler un unicode_char
                        estado = 1;
                    } else if (c_actual == '\n') {
                        linea_actual++;
                    }
                    break;
                case 1: // Inicio dun unicode_char
                    switch (c_actual) {
                        case 'x':   // Se é un byte hexadecimal:
                            // Lense dous díxitos hexadecimais correspondentes
                            if (_lerDixitosHex(2)) {
                                _lanzarErro(BYTE_HEX_POUCOS_DIXITOS, 1);
                            }
                            break;
                        case 'u':   // Se é un unicode pequeno:
                            // Lense catro díxitos hexadecimais correspondentes
                            if (_lerDixitosHex(4)) {
                                _lanzarErro(UNICODE_PEQUENO_INVALIDO, 1);
                            }
                            break;
                        case 'U':   // Se é un unicode grande:
                            // Lense oito díxitos hexadecimais correspondentes
                            if (_lerDixitosHex(8)) {
                                _lanzarErro(UNICODE_GRANDE_INVALIDO, 1);
                            }
                            break;
                        default:    // Se é un byte octal ou caracter escapado:
                            if (_serOctal(c_actual)) {   // Se é un valor octal
                                for (int i = 0; i < 2; i++) {   // Lense os dous valores octais que faltan
                                    c_actual = segCaracter();
                                    if (!_serOctal(c_actual)) { // Se non son valores octais,
                                        //     lánzase un erro por ser poucos díxitos octais
                                        _lanzarErro(BYTE_OCT_POUCOS_DIXITOS, 1);
                                        break;
                                    }
                                }
                            } else if (c_actual != 'a' && c_actual != 'b' && c_actual != 'f' && c_actual != 'n' &&
                                       c_actual != 'r' && c_actual != 't' && c_actual != 'v' && c_actual != '\\' &&
                                       c_actual != '\'' && c_actual != '"') {
                                // Se non é un caracter escapado, lánzase un erro por secuencia descoñecida
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
        } while ((c_actual != '"' || escapado) && c_actual != EOF);
    } else {                    // Se é un string crudo:
        do {
            c_actual = segCaracter();
            if (c_actual == '\n') {
                linea_actual++;
            }
        } while (c_actual != '`' && c_actual != EOF);
    }

    if (c_actual == EOF) { // Se se chegou ao fin de ficheiro e non se pechou, lánzase un erro por string sen pechar
        _lanzarErro(STRING_NON_PECHADO, 0);
    }

    if (erro) {
        saltarLexema();
    } else {
        _aceptarLexema(STRING, 0);
    }
}

// Función auxiliar correspondente ao AF de comentarios
void _comentarios() {
    int estado, saltado = 0;

    c_actual = segCaracter();
    if (c_actual == '/') {
        estado = 0;
    } else if (c_actual == '*') {
        estado = 1;
    } else {
        estado = 3;
    }

    while (!saltado && !aceptado && !erro) {
        switch (estado) {
            case 0: // Comentario dunha liña
                do {
                    c_actual = segCaracter();
                } while (c_actual != '\n' && c_actual != EOF);
                // Lense caracteres mentres non haxa un salto de liña ou fin de ficheiro

                if (c_actual == '\n') {
                    linea_actual++;
                }

                saltarLexema();
                saltado = 1;
                break;
            case 1: // Comentario multiliña
                do {
                    c_actual = segCaracter();
                    if (c_actual == '\n') {
                        linea_actual++;
                    }
                } while (c_actual != '*' && c_actual != EOF);

                // Se se chegou ao fin de ficheiro e non se pechou, lánzase un erro por comentario multiliña sen pechar
                if (c_actual == EOF) {
                    _lanzarErro(COMENTARIO_MULTILINEA_NON_PECHADO, 0);
                    saltarLexema();
                } else {
                    estado = 2;
                }
                break;
            case 2: // Comentario multiliña e leuse '*'
                c_actual = segCaracter();
                if (c_actual == '/') { // Se se le '/', péchase o comentario e sáltase
                    saltarLexema();
                    saltado = 1;
                } else {        // Se non se le unha '/', vólvese ao estado anterior
                    if (c_actual == '\n') {
                        linea_actual++;
                    }
                    estado = 1;
                }
                break;
            case 3: // Despois da primeira '/' leuse algo distinto de '/' ou '*', pode ser '/' ou DIVIGUAL
                if (c_actual == '=') {
                    _aceptarLexema(DIVIGUAL, 0);
                } else {
                    // Devólvese o caracter que non é '='
                    _aceptarLexema('/', 1);
                }
                break;
        }
    }
}


// Función auxiliar que libera a memoria asociada a un lexema lido anteriormente e restablece os valores inicias
void _limparComp() {
    if (comp.lexema != NULL) {
        free(comp.lexema);
        comp.comp_lexico = 0;
        comp.lexema = NULL;
    }
}


// Función que devolve a línea actual de análise
int obterLineaActual() {
    return linea_actual;
}

// Función que inicia o analizador léxico
void iniciarAnalizadorLexico(char *nomeFicheiro) {
    iniciarSistemaEntrada(nomeFicheiro);
}

// Función que le o seguinte compoñente léxico do código fonte
CompLexico segCompLexico() {
    int estado = 0;
    _limparComp();

    aceptado = 0;
    erro = 0;

    do {
        c_actual = segCaracter();
        switch (estado) {
            case 0: // Estado inicial do analizador léxico
                if (isalpha(c_actual) || c_actual == '_') {       // Se comeza por unha letra ou _, AF de cadeas alfanuméricas
                    _alfanumerico();                //      Acepta ID e keywords
                } else if (isdigit(c_actual) || c_actual == '.') {// Se comeza por un número ou ., AF de cadeas numéricas
                    _numerico();                    //      Acepta INT, FLOAT, IMAGINARY, . e PUNTOTRIPLE
                } else {
                    switch (c_actual) {
                        case '\'':                  // Se comeza por ', AF de runas
                            _rune();                //      Acepta RUNE
                            break;
                        case '"':
                        case '`':                   // Se comeza por " ou `, AF de strings
                            _strings();             //      Acepta STRING
                            break;
                        case '/':                   // Se comeza por /, AF de comentarios
                            _comentarios();         //      Acepta /, DIVIGUAL e sáltase os comentarios
                            break;
                        case '+':
                            estado = 1;
                            break;
                        case '-':
                            estado = 2;
                            break;
                        case '*':
                            estado = 3;
                            break;
                        case '%':
                            estado = 4;
                            break;
                        case '&':
                            estado = 5;
                            break;
                        case '|':
                            estado = 7;
                            break;
                        case '^':
                            estado = 8;
                            break;
                        case '<':
                            estado = 9;
                            break;
                        case '>':
                            estado = 11;
                            break;
                        case '=':
                            estado = 13;
                            break;
                        case '!':
                            estado = 14;
                            break;
                        case '~':
                            _aceptarLexema('~', 0);
                            break;
                        case ':':
                            estado = 15;
                            break;
                        case '(':
                            _aceptarLexema('(', 0);
                            break;
                        case '[':
                            _aceptarLexema('[', 0);
                            break;
                        case '{':
                            _aceptarLexema('{', 0);
                            break;
                        case ',':
                            _aceptarLexema(',', 0);
                            break;
                        case ')':
                            _aceptarLexema(')', 0);
                            break;
                        case ']':
                            _aceptarLexema(']', 0);
                            break;
                        case '}':
                            _aceptarLexema('}', 0);
                            break;
                        case ';':
                            _aceptarLexema(';', 0);
                            break;
                        case EOF:                   // Se é EOF, indícase para finalizar a análise léxica
                            comp.lexema = NULL;
                            comp.comp_lexico = EOF;
                            break;
                        default:                    // Se non comeza por ningún dos anteriores, ignórase o caracter
                            ignorarCaracter();      //      Está pensado para os \t, \n e espacios do código
                            if (c_actual == '\n') {
                                linea_actual++;
                            }
                            break;
                    }
                }
                break;
            case 1: // Leuse '+'
                switch (c_actual) {
                    case '+':
                        _aceptarLexema(SUMADOBLE, 0);
                        break;
                    case '=':
                        _aceptarLexema(SUMAIGUAL, 0);
                        break;
                    default:
                        // Devólvese o caracter que non é '+' ou '='
                        _aceptarLexema('+', 1);
                        break;
                }
                break;
            case 2: // Leuse '-'
                switch (c_actual) {
                    case '-':
                        _aceptarLexema(RESTADOBLE, 0);
                        break;
                    case '=':
                        _aceptarLexema(RESTAIGUAL, 0);
                        break;
                    default:
                        // Devólvese o caracter que non é '-' ou '='
                        _aceptarLexema('-', 1);
                        break;
                }
                break;
            case 3: // Leuse '*'
                if (c_actual == '=') {
                    _aceptarLexema(MULTIGUAL, 0);
                } else {
                    // Devólvese o caracter que non é '='
                    _aceptarLexema('*', 1);
                }
                break;
            case 4: // Leuse '%'
                if (c_actual == '=') {
                    _aceptarLexema(MODIGUAL, 0);
                } else {
                    // Devólvese o caracter que non é '='
                    _aceptarLexema('%', 1);
                }
                break;
            case 5: // Leuse '&'
                switch (c_actual) {
                    case '^':
                        estado = 6;
                        break;
                    case '=':
                        _aceptarLexema(AMPERIGUAL, 0);
                        break;
                    case '&':
                        _aceptarLexema(AMPERDOBLE, 0);
                        break;
                    default:
                        // Devólvese o caracter que non é '^', '=' ou '&'
                        _aceptarLexema('&', 1);
                        break;
                }
                break;
            case 6: // Leuse '&^'
                if (c_actual == '=') {
                    _aceptarLexema(AMPEREXPIGUAL, 0);
                } else {
                    // Devólvese o caracter que non é '='
                    _aceptarLexema(AMPEREXP, 1);
                }
                break;
            case 7: // Leuse '|'
                switch (c_actual) {
                    case '=':
                        _aceptarLexema(VERTIGUAL, 0);
                        break;
                    case '|':
                        _aceptarLexema(VERTDOBLE, 0);
                        break;
                    default:
                        // Devólvese o caracter que non é '=' ou '|'
                        _aceptarLexema('|', 1);
                        break;
                }
                break;
            case 8: // Leuse '^'
                if (c_actual == '=') {
                    _aceptarLexema(EXPIGUAL, 0);
                } else {
                    // Devólvese o caracter que non é '='
                    _aceptarLexema('=', 1);
                }
                break;
            case 9: // Leuse '<'
                switch (c_actual) {
                    case '<':
                        estado = 10;
                        break;
                    case '-':
                        _aceptarLexema(FRECHAESQ, 0);
                        break;
                    case '=':
                        _aceptarLexema(MENORIGUAL, 0);
                        break;
                    default:
                        // Devólvese o caracter que non é '<', '-' ou '='
                        _aceptarLexema('<', 1);
                        break;
                }
                break;
            case 10: // Leuse '<<'
                if (c_actual == '=') {
                    _aceptarLexema(ANGULARESESQIGUAL, 0);
                } else {
                    // Devólvese o caracter que non é '='
                    _aceptarLexema(ANGULARESESQ, 1);
                }
                break;
            case 11: // Leuse '>'
                switch (c_actual) {
                    case '>':
                        estado = 12;
                        break;
                    case '=':
                        _aceptarLexema(MAIORIGUAL, 0);
                        break;
                    default:
                        // Devólvese o caracter que non é '>' ou '='
                        _aceptarLexema('>', 1);
                        break;
                }
                break;
            case 12: // Leuse '>>'
                if (c_actual == '=') {
                    _aceptarLexema(ANGULARESDERIGUAL, 0);
                } else {
                    // Devólvese o caracter que non é '='
                    _aceptarLexema(ANGULARESDER, 1);
                }
                break;
            case 13: // Leuse '='
                if (c_actual == '=') {
                    _aceptarLexema(IGUALDOBLE, 0);
                } else {
                    // Devólvese o caracter que non é '='
                    _aceptarLexema('=', 1);
                }
                break;
            case 14: // Leuse '!'
                if (c_actual == '=') {
                    _aceptarLexema(EXCLAMACIONIGUAL, 0);
                } else {
                    // Devólvese o caracter que non é '='
                    _aceptarLexema('!', 1);
                }
                break;
            case 15: // Leuse ':'
                if (c_actual == '=') {
                    _aceptarLexema(DOUSPUNTOSIGUAL, 0);
                } else {
                    // Devólvese o caracter que non é '='
                    _aceptarLexema(':', 1);
                }
                break;
        }
    } while (c_actual != EOF && !aceptado && !erro);

    return comp;
}

// Función que finaliza o analizador léxico
void finalizarAnalizadorLexico() {
    _limparComp();
    finalizarSistemaEntrada();
}
