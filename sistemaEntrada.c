#include <stdio.h>
#include <stdlib.h>

#include "sistemaEntrada.h"
#include "xestionErros.h"

#define TAM 32


typedef struct {
    char A[TAM+1];
    char B[TAM+1];
    int inicio;     // Valores de 0 a 2*sizeof(A)-1
    int dianteiro;  // Valores de 0 a 2*sizeof(A)-1
    int activo; // A = 0 | B = 1
    int cargar; // Non se precisa cargar = 0 | Precísase cargar = 1
} Buffer;


FILE *f_codigo_fonte;
Buffer buf;


void _inicializarBuffer() {
    buf.inicio = 0;
    buf.dianteiro = 0;
    buf.activo = 1;
    buf.cargar = 1;
}

void _cambiarBloqueActivo() {
    if (buf.activo == 0) {
        buf.activo = 1;
    } else {
        buf.activo = 0;
    }
}

void _cargarBloque() {
    if (buf.activo == 0) { // Se o bloque A é o que estaba activo:
        fgets(buf.B, sizeof(buf.B) - sizeof(char), f_codigo_fonte);
        buf.B[TAM - 1] = EOF;
    } else { // Se o bloque B é o que estaba activo:
        fgets(buf.A, sizeof(buf.A) - sizeof(char), f_codigo_fonte);
        buf.A[TAM - 1] = EOF;
    }
}


void iniciarSistemaEntrada(char *nomeFicheiro) {
    f_codigo_fonte = fopen(nomeFicheiro, "r");

    if (f_codigo_fonte == NULL) {
        notificarErro(FICHEIRO_NON_ATOPADO);
        exit(EXIT_FAILURE);
    }
    _inicializarBuffer();
    _cargarBloque();
}

char segCaracter() {
    char c;

    if (buf.activo == 0) { // Se o bloque A é o que está activo:
        // Se o caracter que hai no array do bloque é EOF hai dúas opcións: fin de ficheiro ou fin de bloque
        if ((c = buf.A[buf.dianteiro]) == EOF) {
            // Se non se chegou ao fin de ficheiro significa que se chegou ao fin de bloque,
            //      polo cal hai que mirar se se carga o seguinte bloque e cambiar de bloque activo.
            if (!feof(f_codigo_fonte)) {
                if (buf.cargar) {
                    _cargarBloque();
                } else buf.cargar = 1;
                _cambiarBloqueActivo();
                c = segCaracter();
            }
                // Se se chegou ao fin de ficheiro, devólvese EOF
            else buf.dianteiro++; // Móvese de posición o punteiro
        }
            // Se o caracter lido non é EOF, soamente se move de posición o punteiro
        else buf.dianteiro++;
    } else { // Se o bloque B é o que está activo:
        // O proceso é análogo ao bloque A
        if ((c = buf.B[buf.dianteiro - TAM - 1]) == EOF) {
            if (!feof(f_codigo_fonte)) {
                if (buf.cargar) {
                    _cargarBloque();
                } else buf.cargar = 1;
                _cambiarBloqueActivo();
                c = segCaracter();
            } else buf.dianteiro++;
        } else buf.dianteiro++;
    }

    return c;
}

void devolverCaracter(char c) {

}

void finalizarSistemaEntrada() {
    free(f_codigo_fonte);
}