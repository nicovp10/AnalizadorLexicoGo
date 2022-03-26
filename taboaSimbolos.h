#ifndef P1_TABOASIMBOLOS_H
#define P1_TABOASIMBOLOS_H

#include "definicions.h"


// Función que inicia a táboa de símbolos
void iniciarTS();

// Función que busca un compoñente léxico concreto na táboa de símbolos
//  Se este compoñente léxico non está na táboa, insértao
void buscar_insertar(CompLexico *comp);

// Función que finaliza a táboa de símbolos
void finalizarTS();

// Función que imprime a táboa de símbolos por orde alfabético dos lexemas
void imprimirTS();


#endif