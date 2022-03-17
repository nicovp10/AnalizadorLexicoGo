#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "analizadorLexico.h"
#include "sistemaEntrada.h"


void _alfanumerico() {


}

void _numerico() {

}


CompLexico segCompLexico() {
    CompLexico comp = {0, NULL};
    int erro = 0;
    char c;

    while (((c = segCaracter()) != EOF) && (erro == 0)) {
        printf("%c", c);
        if (isalpha(c) || c == '_') {
            _alfanumerico();
        } else if (isdigit(c)) {
            _numerico();
        } else {

        }
    }


    return comp;
}