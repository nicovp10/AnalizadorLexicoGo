#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "analizadorLexico.h"
#include "sistemaEntrada.h"


CompLexico comp = {0, NULL};


void _alfanumerico() {


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

CompLexico segCompLexico() {
    char c;

    _limparComp();

    while ((c = segCaracter()) != EOF) {
            if (isalpha(c) || c == '_') {
                _alfanumerico();
            } else if (isdigit(c)) {
                _numerico();
            } else {

            }
    }


    return comp;
}