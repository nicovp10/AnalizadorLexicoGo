#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sistemaEntrada.h"
#include "xestionErros.h"

#define TAM 64


typedef struct {
    char A[TAM + 1];
    char B[TAM + 1];
    int inicio;     // Valores de 0 a 2*sizeof(A)-1, para recorrer ambos bloques
    int dianteiro;  // Valores de 0 a 2*sizeof(A)-1, para recorrer ambos bloques
    int activo; // Indica que bloque está activo: A = 0 | B = 1
    int cargar; // Non se precisa cargar = 0 | Precísase cargar = 1
    /*
     * O indicador "cargar" empregarase no caso no que se devolvan caracteres e se retroceda
     * de bloque na devolución, para que non se sobreescriban así os caracteres pendentes de
     * lectura ao volver ao bloque no cal se estaba antes da devolución.
     */
} Buffer;


Buffer buf;
FILE *f_codigo_fonte;


// Inicialización do buffer cos valores necesarios
void _iniciarBuffer() {
    buf.inicio = 0;
    buf.dianteiro = 0;
    buf.activo = 1;
    buf.cargar = 1;
}

// Cambio de bloque do buffer
void _cambiarBloqueActivo() {
    if (buf.activo == 0) {
        buf.activo = 1;
    } else {
        buf.activo = 0;
        // dianteiro vólvese a 0 porque se non habería overflow, sería algo similar a facer o módulo
        buf.dianteiro = 0;
    }
}

// Carga do bloque non activo
void _cargarSegBloque() {
    /*
     * A variable "bytes_lidos" serve para controlar o fin de ficheiro, xa que este pode estar no medio
     * dun bloque. Durante a maior parte da análise sempre valerá o mesmo que a constante TAM, pero no
     * último bloque a cargar do código valerá os bytes que falten por ler, poñendo o EOF inmediatamente
     * despois, sen deixar así espazos en blanco entre o fin dos bytes lidos e o final do bloque.
     */
    unsigned long bytes_lidos;

    // Compróbase que bloque está activo no momento da solictude de carga
    if (buf.activo == 0) { // Se é o bloque A:
        bytes_lidos = fread(buf.B, 1, sizeof(buf.B) - sizeof(char), f_codigo_fonte);
        buf.B[bytes_lidos] = EOF;
    } else { // Se é o bloque B:
        bytes_lidos = fread(buf.A, 1, sizeof(buf.A) - sizeof(char), f_codigo_fonte);
        buf.A[bytes_lidos] = EOF;
    }
}


// Inicio do sistema de entrada
void iniciarSistemaEntrada(char *nomeFicheiro) {
    f_codigo_fonte = fopen(nomeFicheiro, "r");

    if (f_codigo_fonte == NULL) {
        lanzarErro(FICHEIRO_NON_ATOPADO);
        exit(EXIT_FAILURE);
    }

    _iniciarBuffer();
    _cargarSegBloque();
    _cambiarBloqueActivo();
}

// Obtención do seguinte caracter da memoria intermedia
char segCaracter() {
    char c;
    // Compróbase que bloque está activo no momento da lectura
    if (buf.activo == 0) { // Se é o bloque A:
        // Compróbase se o caracter que se le é EOF
        if ((c = buf.A[buf.dianteiro]) == EOF) {    // Se é EOF hai dúas opcións: fin de ficheiro ou fin de bloque
            // Compróbase se se chegou ao fin do ficheiro de código fonte
            if (!feof(f_codigo_fonte)) {    // Se non se chegou ao fin de ficheiro, chegouse ao fin de bloque
                if (buf.cargar)      // Se se precisa cargar, cárgase o seguinte bloque
                    _cargarSegBloque();
                else
                    buf.cargar = 1;  // Se non se precisaba, indícase que se precisará a continuación
                _cambiarBloqueActivo(); // Cámbiase de bloque activo
                c = segCaracter();      // Chamada recursiva á función para ler un caracter xa que nesta chamada leuse o EOF de final de bloque
            }
                // Se se chegou ao fin de ficheiro, devólvese EOF
            else {
                buf.dianteiro++; // Móvese de posición o punteiro
            }
        }
            // Se o caracter lido non é EOF, soamente se move de posición o punteiro
        else {
            buf.dianteiro++;
        }
    } else { // Se é o bloque B:
        // O proceso é análogo ao do bloque A
        if ((c = buf.B[buf.dianteiro - TAM]) == EOF) {  // Réstase TAM para adaptar o valor ao bloque B
            if (!feof(f_codigo_fonte)) {
                if (buf.cargar) {
                    _cargarSegBloque();
                } else {
                    buf.cargar = 1;
                }
                _cambiarBloqueActivo();
                c = segCaracter();
            } else {
                buf.dianteiro++;
            }
        } else {
            buf.dianteiro++;
        }
    }

    return c;
}

// Ignora un caracter, saltando na memoria intermedia
void ignorarCaracter() {
    int modificar_dianteiro = 0;

    // Se só se lera un caracter do lexema, tamén se modifica dianteiro para que non se quede atrás
    if (buf.dianteiro - buf.inicio == 1) {
        modificar_dianteiro = 1;
    }

    // Realízanse diferentes comprobacións en función do bloque activo
    if (buf.activo == 0) {  // Se é o bloque A:
        // Compróbase a que elemento do bloque apunta o punteiro inicio
        if (buf.inicio == TAM - 1) {    // Se apuntaba ao último, cámbiase de bloque e cárgase
            _cargarSegBloque();
            _cambiarBloqueActivo();
        }
        buf.inicio++;
    } else {    // Se é o bloque B:
        // O proceso é análogo ao bloque A
        if (buf.inicio == 2 * TAM - 1) {
            _cargarSegBloque();
            buf.activo = 0;
            buf.inicio = 0;
        } else {
            buf.inicio++;
        }
    }

    if (modificar_dianteiro) {
        buf.dianteiro = buf.inicio;
    }
}

// Devolución de caracter, retrocedendo na memoria intermedia
void devolverCaracter() {
    // Realízanse diferentes comprobacións en función do bloque activo
    if (buf.activo == 0) {  // Se é o bloque A:
        // Compróbase a que elemento do bloque apunta o punteiro dianteiro
        if (buf.dianteiro == 0) {   // Se apunta ao primeiro elemento do bloque:
            _cambiarBloqueActivo();
            buf.cargar = 0; // Indícase que non se cargue cando se volve ao bloque do que se acaba de retroceder
            buf.dianteiro = 2 * TAM - 1;    // Modifícase o punteiro dianteiro para adaptarse ao bloque B
        } else {
            buf.dianteiro--; // Se non apunta ao primeiro elemento, retrocédese unha posición soamente
        }
    } else {    // Se é o bloque B:
        // O proceso é análogo ao bloque A
        if (buf.dianteiro == TAM) {
            _cambiarBloqueActivo();
            buf.cargar = 0;
            buf.dianteiro = TAM - 1;
        } else {
            buf.dianteiro--;
        }
    }
}

// Acepta o lexema que está léndose actualmente, reubicando os punteiros do búffer
void aceptarLexema(CompLexico *comp) {
    // Compróbanse a posición relativa entre ambos punteiros do buffer
    // As dúas primeiras comprobacións son se os punteiros están en bloques distintos
    if (buf.inicio < TAM && buf.dianteiro >= TAM) {         // Se inicio está en A e dianteiro en B:
        // Resérvase a memoria restando dianteiro menos inicio para obter o número de chars a reservar
        comp->lexema = malloc((buf.dianteiro - buf.inicio) * sizeof(char));

        // Cópiase a parte do bloque A no campo do lexema
        strncpy(comp->lexema, buf.A + buf.inicio, (TAM - buf.inicio) * sizeof(char));

        // Concaténase a parte do bloque B no campo do lexema
        strncat(comp->lexema, buf.B, (buf.dianteiro - TAM) * sizeof(char));
    } else if (buf.inicio >= TAM && buf.dianteiro < TAM) {  // Se inicio está en B e dianteiro en A:
        // Resérvase a memoria restando TAM menos inicio máis dianteiro para obter o número de chars a reservar
        // Realízase esta resta de forma diferente para adaptarse ao valor dos punteiros
        comp->lexema = malloc((2 * TAM - buf.inicio + buf.dianteiro) * sizeof(char));

        strncpy(comp->lexema, buf.B + buf.inicio - TAM, (2 * TAM - buf.inicio) * sizeof(char));
        strncat(comp->lexema, buf.A, buf.dianteiro * sizeof(char));
    } else {                                                // Se os dous punteiros están no mesmo bloque e inicio está antes que dianteiro:
        comp->lexema = malloc((buf.dianteiro - buf.inicio) * sizeof(char));

        // Compróbase o bloque no que está para a selección
        if (buf.activo == 0) {
            strncpy(comp->lexema, buf.A + buf.inicio, (buf.dianteiro - buf.inicio) * sizeof(char));
        } else {
            strncpy(comp->lexema, buf.B + buf.inicio - TAM, (buf.dianteiro - buf.inicio) * sizeof(char));
        }
    }

    saltarLexema();

    /*
     * Compróbase se o lexema exede o tamaño máximo.
     * Este tamaño será o tamaño do lexema nun dos peores casos: o lexema está
     * na última posición nun bloque e no bloque seguinte completo. O tamaño deste
     * lexema será o máximo permitido
     */
    if (strlen(comp->lexema) > TAM + 1) {
        free(comp->lexema);
        comp->lexema = NULL;
        lanzarErro(LEXEMA_TAM_EXCEDIDO);
    }
}

// Salta o lexema que está léndose actualmente, reubicando os punteiros do búffer
void saltarLexema() {
    buf.inicio = buf.dianteiro;
}

// Finalización do sistema de entrada
void finalizarSistemaEntrada() {
    free(f_codigo_fonte);
}