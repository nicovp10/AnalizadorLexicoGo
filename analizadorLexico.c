#include <stdio.h>
#include <stdlib.h>

#include "analizadorLexico.h"
#include "definicions.h"
#include "sistemaEntrada.h"



//devolve a estrutura
CompLexico segCompLexico() {
    CompLexico compLex = {0, NULL};
    int erro = 0;
    char c;

    // para os diferentes autómatas facer diferentes funcións. p.e.: cadenas alfanuméricas
    while (((c = segCaracter()) != EOF) && (erro == 0)) {

    }


    return compLex;
}