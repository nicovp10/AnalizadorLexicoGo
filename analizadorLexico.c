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


/* Funcion auxiliar que le un numero concreto de dixitos hexadecimais
 *      return 0: exito
 *      return 1: erro
 */
int _lerDixitosHex(int n) {
    for (int i = 0; i < n; i++) {
        c = segCaracter();

        if (!isdigit(c) &&
            ((c < 65) || (c > 70)) &&
            ((c < 97) || (c > 102))) {
            // Se non é un díxito nin A-F nin a-f:
            return 1;
        }
    }

    return 0;
}


void _alfanumerico(char char_inicial) {
    int cont_chars = 0;

    do {
        c = segCaracter();
        cont_chars++;
    } while (isalpha(c) || isdigit(c) || c == '_');

    devolverCaracter();
    aceptarLexema(&comp);

    // Se se leu só un caracter, compróbase se é "_" para indicar que é un BLANK_ID
    if (cont_chars == 1 && char_inicial == '_') {
        comp.comp_lexico = BLANK_ID;
    } else if (comp.lexema != NULL) {   // Se non se excedeu o tamaño:
        buscar_insertar(&comp);
    }

    aceptado = 1;
}

void _numerico() {
    int estado = 0;

    if (isdigit(c) && c != '0') {
        estado = 0;
    } else if (c == '0') {
        estado = 1;
    } else if (c == '.') {
        estado = 2;
    }

    do {
        c = segCaracter();
        switch (estado) {
            case 0: // decimal_lit ou decimal_float_lit
                comp.comp_lexico = INT;
                switch (c) {
                    case '_':   // Se se le unha _, tense que ler un número posteriormente
                        estado = 3;
                        break;
                    case '.':   // Se se le un ., tense que ler un número posteriormente
                        estado = 2; // Non obstante pode ser PUNTOTRIPLE ou PUNTO
                        break;
                    case 'e':
                    case 'E':
                        estado = 5;
                        break;
                }
                break;
            case 1:
                if (isdigit(c)) {
                    estado = 0;
                } else {
                    switch (c) {
                        case 'b':
                        case 'B':   // binary_lit
                            comp.comp_lexico = INT;
                            break;
                        case 'o':   // octal_lit
                        case 'O':
                            comp.comp_lexico = INT;
                            break;
                        case 'x':   // hex_lit ou hex_float_lit
                        case 'X':
                            break;
                    }
                }
                break;
            case 2: // Último caracter lido: .
                comp.comp_lexico = FLOAT;
                if (isdigit(c)) {
                    estado = 1;
                } else if (c == '.') {
                    estado = 4;
                } else if (c == 'e' || c == 'E') {
                    estado = 5;
                } else {
                    devolverCaracter();
                    aceptarLexema(&comp);
                    comp.comp_lexico = PUNTO;
                    aceptado = 1;
                }
                break;
            case 3: // Último caracter lido: _
                if (isdigit(c)) {
                    estado = 0;
                } else {
                    lanzarErro(INT_MAL_FORMADO);
                    erro = 1;
                }
                break;
            case 4: // Últimos caracteres lidos: ..
                if (c == '.') {
                    aceptarLexema(&comp);
                    comp.comp_lexico = PUNTOTRIPLE;
                    aceptado = 1;
                } else {
                    lanzarErro(LEXEMA_DESCONOCIDO);
                    saltarLexema();
                    erro = 1;
                }
                break;
            case 5: // Último caracter lido: e | E
                break;
        }
    } while (c != EOF && !aceptado && !erro);
}

void _rune() {  // Moi similar a un string interpretado, pero dun só caracter
    int estado = 0, escapado, cont_chars = -1;
    // Inicialízase cont_chars a -1 para que non teña en conta a ' de peche

    do {
        escapado = 0;
        c = segCaracter();
        switch (estado) {
            case 0: // Se é un caracter unicode:
                if (c == '\\') {
                    estado = 1;
                }
                cont_chars++; // Indica o número de caracteres lidos (menos 1)
                break;
            case 1: // Se se detectou una \:
                switch (c) {
                    case 'x':   // Se é un byte hexadecimal:
                        // Lense os díxitos hexadecimais correspondentes
                        if (_lerDixitosHex(2)) {
                            lanzarErro(BYTE_HEX_POUCOS_DIXITOS);
                            erro = 1;
                        }
                        break;
                    case 'u':   // Se é un unicode pequeno:
                        if (_lerDixitosHex(4)) {
                            lanzarErro(UNICODE_PEQUENO_INVALIDO);
                            erro = 1;
                        }
                        break;
                    case 'U':   // Se é un unicode grande:
                        if (_lerDixitosHex(8)) {
                            lanzarErro(UNICODE_GRANDE_INVALIDO);
                            erro = 1;
                        }
                        break;
                    default:    // Se é un byte octal ou caracter escapado:
                        if ((c >= 48) && (c <= 55)) {   // Se é un valor octal
                            for (int i = 0; i < 2; i++) {   // Lense os dous valores octais que faltan
                                c = segCaracter();
                                if ((c < 48) || (c > 55)) {   // Se non son valores octais:
                                    lanzarErro(BYTE_OCT_POUCOS_DIXITOS);
                                    erro = 1;
                                    break;
                                }
                            }
                        } else if (c != 'a' && c != 'b' && c != 'f' && c != 'n' &&
                                   c != 'r' && c != 't' && c != 'v' && c != '\\' &&
                                   c != '\'' && c != '"') {
                            // Compróbase se non é un caracter escapado
                            lanzarErro(CARACTER_ESCAPADO_SECUENCIA_DESCONOCIDA);
                            erro = 1;
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
        lanzarErro(RUNA_NON_PECHADA);
        erro = 1;
    } else if (cont_chars > 1 && !erro) {
        lanzarErro(RUNA_MOITOS_CARACTERES);
        erro = 1;
    }

    if (erro) {
        saltarLexema();
    } else {
        aceptarLexema(&comp);
        comp.comp_lexico = RUNE;
        aceptado = 1;
    }
}

void _strings() {
    int estado = 0, escapado;

    if (c == '"') {   // Se é un string interpretado:
        do {
            escapado = 0;
            c = segCaracter();
            switch (estado) {
                case 0: // Se é un caracter unicode:
                    if (c == '\\') {
                        estado = 1;
                    }
                    break;
                case 1: // Se se detectou una \:
                    switch (c) {
                        case 'x':   // Se é un byte hexadecimal:
                            // Lense os díxitos hexadecimais correspondentes
                            if (_lerDixitosHex(2)) {
                                lanzarErro(BYTE_HEX_POUCOS_DIXITOS);
                                erro = 1;
                            }
                            break;
                        case 'u':   // Se é un unicode pequeno:
                            if (_lerDixitosHex(4)) {
                                lanzarErro(UNICODE_PEQUENO_INVALIDO);
                                erro = 1;
                            }
                            break;
                        case 'U':   // Se é un unicode grande:
                            if (_lerDixitosHex(8)) {
                                lanzarErro(UNICODE_GRANDE_INVALIDO);
                                erro = 1;
                            }
                            break;
                        default:    // Se é un byte octal ou caracter escapado:
                            if ((c >= 48) && (c <= 55)) {   // Se é un valor octal
                                for (int i = 0; i < 2; i++) {   // Lense os dous valores octais que faltan
                                    c = segCaracter();
                                    if (c < 48 || (c > 55)) {   // Se non son valores octais:
                                        lanzarErro(BYTE_OCT_POUCOS_DIXITOS);
                                        erro = 1;
                                        break;
                                    }
                                }
                            } else if (c != 'a' && c != 'b' && c != 'f' && c != 'n' &&
                                       c != 'r' && c != 't' && c != 'v' && c != '\\' &&
                                       c != '\'' && c != '"') {
                                // Compróbase se non é un caracter escapado
                                lanzarErro(CARACTER_ESCAPADO_SECUENCIA_DESCONOCIDA);
                                erro = 1;
                            } else {
                                escapado = 1;
                            }
                            break;
                    }
                    estado = 0;
                    break;
            }
        } while ((c != '"' || escapado) && c != EOF);

        if (c == EOF) {
            lanzarErro(STRING_INTERPRETADO_NON_PECHADO);
            erro = 1;
        }
    } else {                    // Se é un string crudo:
        do {
            c = segCaracter();
        } while (c != '`' && c != EOF);

        if (c == EOF) {
            lanzarErro(STRING_CRUDO_NON_PECHADO);
            erro = 1;
        }
    }

    if (erro) {
        saltarLexema();
    } else {
        aceptarLexema(&comp);
        comp.comp_lexico = STRING;
        aceptado = 1;
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
            case 0: // Se é un comentario dunha liña:
                do {
                    c = segCaracter();
                } while (c != '\n' && c != EOF);
                saltarLexema();
                aceptado = 1;
                break;
            case 1: // Se é un comentario múltiples:
                do {
                    c = segCaracter();
                } while (c != '*' && c != EOF);
                if (c == EOF) {
                    lanzarErro(COMENTARIO_MULTILINEA_NON_PECHADO);
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
            case 3: // Se despois da primeira / vén algo distinto de / ou *:
                devolverCaracter();
                aceptarLexema(&comp);
                comp.comp_lexico = DIV;
                aceptado = 1;
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
                    _alfanumerico(c);     //      Acepta ID, BLANK_ID e keywords
                } else if (isdigit(c) || c == '.') {// Se comeza por un número ou ., AF de cadeas numéricas
                    _numerico();                    //      Acepta INT, FLOAT, IMAGINARY, PUNTOTRIPLE e PUNTO
                } else if (c == '\'') {             // Se comeza por ', AF de runas
                    _rune();                        //      Acepta RUNE
                } else if (c == '"' || c == '`') {  // Se comeza por " ou `, AF de strings
                    _strings();                     //      Acepta STRING
                } else if (c == '/') {              // Se comeza por /, AF de comentarios
                    _comentarios();                 //      Acepta DIV e sáltase os comentarios
                } else if (c == EOF) {              // Se é EOF, indícase no compoñente léxico
                    comp.lexema = NULL;
                    comp.comp_lexico = EOF;
                } else {                            // Se non comeza por ningún dos anteriores, ignórase o caracter
                    ignorarCaracter();              //      Está pensado para os \t, \n e espacios do código
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
