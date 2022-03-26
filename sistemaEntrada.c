#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sistemaEntrada.h"
#include "xestionErros.h"

#define TAM 64


typedef struct {
    char A[TAM + 1];
    char B[TAM + 1];
    int inicio;     // Toma valores de 0 a 2*sizeof(A)-1, para recorrer ambos bloques
    int dianteiro;  // Toma valores de 0 a 2*sizeof(A)-1, para recorrer ambos bloques
    int activo; // Indica que bloque está activo: A = 0 | B = 1
    int cargar; // Indica se se precisa cargar o seguinte bloque:
    //      Non se precisa cargar = 0 | Precísase cargar = 1
    /*
     * O indicador "cargar" empregarase no caso no que se devolvan caracteres e se retroceda
     * de bloque na devolución, para que non se sobreescriban así os caracteres pendentes de
     * lectura ao volver ao bloque no cal se estaba antes da devolución.
     */
} Buffer;


Buffer buf;
FILE *f_codigo_fonte;


// Función auxiliar que inicia o buffer cos valores necesarios
void _iniciarBuffer() {
    buf.inicio = 0;
    buf.dianteiro = 0;
    buf.activo = 1;
    buf.cargar = 1;
}

// Función auxiliar que cambia o bloque activo do buffer
void _cambiarBloqueActivo() {
    if (buf.activo == 0) {
        buf.activo = 1;
    } else {
        buf.activo = 0;
        // dianteiro vólvese a 0 porque se non accedería a posicións maiores das que ten o bloque
        //      Sería algo similar a facer o módulo
        buf.dianteiro = 0;
    }
}

// Función auxiliar que carga o bloque non activo do buffer
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
    } else {               // Se é o bloque B:
        bytes_lidos = fread(buf.A, 1, sizeof(buf.A) - sizeof(char), f_codigo_fonte);
        buf.A[bytes_lidos] = EOF;
    }
}


// Función que inicia o sistema de entrada
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

// Función que obtén seguinte caracter da memoria intermedia
char segCaracter() {
    char c;

    // Compróbase que bloque está activo no momento da lectura
    if (buf.activo == 0) { // Se é o bloque A:
        // Compróbase se o caracter que se le é EOF
        if ((c = buf.A[buf.dianteiro]) == EOF) {    // Se é EOF hai dúas opcións: fin de ficheiro ou fin de bloque
            // Compróbase se se chegou ao fin do ficheiro de código fonte
            if (!feof(f_codigo_fonte)) {    // Se non se chegou ao fin de ficheiro, chegouse ao fin de bloque
                if (buf.cargar) {     // Se se precisa cargar, cárgase o seguinte bloque
                    _cargarSegBloque();
                } else {              // Se non se precisa cargar, indícase que se precisará a continuación
                    buf.cargar = 1;
                }
                _cambiarBloqueActivo(); // Cámbiase de bloque activo
                c = segCaracter();      // Chamada recursiva á función para ler un caracter xa que nesta chamada leuse o EOF de final de bloque
            } else {                              // Se se chegou ao fin de ficheiro, devólvese EOF
                buf.dianteiro++; // Móvese de posición o punteiro
            }
        } else {                                    // Se o caracter lido non é EOF, soamente se move de posición o punteiro
            buf.dianteiro++;
        }
    } else { // Se é o bloque B:
        // O proceso é análogo ao do bloque A
        if ((c = buf.B[buf.dianteiro - TAM]) == EOF) {  // Réstase TAM para adaptar o valor do punteiro ao bloque B
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

// Función que ignora un caracter, saltándoo na memoria intermedia
void ignorarCaracter() {
    int modificar_dianteiro = 0;

    /*
     * A variable "modificar_dianteiro" serve para ter os valores correctos nos punteiros naqueles casos
     * nos que só se lera un caracter do lexema actual. Nos casos nos que os punteiros permanecen no mesmo
     * bloque tras saltarse o caracter a ignorar non sería necesaria ningunha modificación extra, pero nos
     * casos nos que o punteiro inicio cambie de bloque para ignorar o caracter si que será necesaria unha
     * adaptación do punteiro dianteiro, xa que se indica o cambio do bloque pero o punteiro dianteiro seguirá
     * cun valor pertencente ao bloque no cal se estaba anteriormente, provocando así valores inconsistentes e
     * accesos a memoria non permitidos.
     */
    if (buf.dianteiro - buf.inicio == 1) {
        modificar_dianteiro = 1;
    }

    if (buf.activo == 0) {  // Se é o bloque A:
        // Compróbase a que elemento do bloque apunta o punteiro inicio
        if (buf.inicio == TAM - 1) {    // Se apuntaba ao último elemento, cámbiase de bloque e cárgase
            _cargarSegBloque();
            _cambiarBloqueActivo();
        }
        buf.inicio++;
    } else {                // Se é o bloque B:
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

// Función que devolve un caracter, retrocedendo na memoria intermedia
void devolverCaracter() {
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
            buf.activo = 0;
            buf.cargar = 0;
        }
        buf.dianteiro--;
    }
}

// Función que acepta o lexema que se está lendo actualmente
void aceptarLexema(CompLexico *comp) {
    int tam_lexema;
    // Compróbanse a posición relativa entre ambos punteiros do buffer
    // As dúas primeiras comprobacións son se os punteiros están en bloques distintos
    if (buf.inicio < TAM && buf.dianteiro >= TAM) {         // Se inicio está en A e dianteiro en B:
        // Resérvase a memoria restando dianteiro menos inicio para obter o número de chars a reservar
        //  Súmaselle 1 para o '\0'
        tam_lexema = buf.dianteiro - buf.inicio;
        comp->lexema = malloc((tam_lexema + 1) * sizeof(char));

        // Cópiase a parte do bloque A no campo do lexema
        strncpy(comp->lexema, buf.A + buf.inicio, (TAM - buf.inicio) * sizeof(char));

        // Engádese o '\0' ao final para evitar warnings/erros na xestión da memoria
        comp->lexema[(TAM - buf.inicio) * sizeof(char)] = '\0';

        // Concaténase a parte do bloque B no campo do lexema
        strncat(comp->lexema, buf.B, (buf.dianteiro - TAM) * sizeof(char));
    } else if (buf.inicio >= TAM && buf.dianteiro < TAM) {  // Se inicio está en B e dianteiro en A:
        // Resérvase a memoria restando 2*TAM menos inicio máis dianteiro para obter o número de chars a reservar
        // Realízase esta resta de forma diferente para adaptarse ao valor dos punteiros
        //  Súmaselle 1 para o '\0'
        tam_lexema = 2 * TAM - buf.inicio + buf.dianteiro;
        comp->lexema = malloc((tam_lexema + 1) * sizeof(char));

        strncpy(comp->lexema, buf.B + buf.inicio - TAM, (2 * TAM - buf.inicio) * sizeof(char));
        comp->lexema[(2 * TAM - buf.inicio) * sizeof(char)] = '\0';
        strncat(comp->lexema, buf.A, buf.dianteiro * sizeof(char));
    } else {                                                // Se os dous punteiros están no mesmo bloque e inicio está antes que dianteiro:
        // Resérvase a memoria restando dianteiro menos inicio para obter o número de chars a reservar
        //  Súmaselle 1 para o '\0'
        tam_lexema = buf.dianteiro - buf.inicio;
        comp->lexema = malloc((tam_lexema + 1) * sizeof(char));

        // Compróbase o bloque no que está:
        if (buf.activo == 0) {
            strncpy(comp->lexema, buf.A + buf.inicio, (buf.dianteiro - buf.inicio) * sizeof(char));
        } else {
            strncpy(comp->lexema, buf.B + buf.inicio - TAM, (buf.dianteiro - buf.inicio) * sizeof(char));
        }
    }

    // Engádese o '\0' ao final para evitar warnings / erros na xestión da memoria
    comp->lexema[tam_lexema] = '\0';

    saltarLexema();

    /*
     * Compróbase se o lexema excede o tamaño máximo.
     * Este tamaño será o tamaño do lexema nun dos peores casos: o lexema está
     * na última posición dun bloque e no bloque seguinte completo. O tamaño deste
     * lexema será o máximo permitido
     */
    if (strlen(comp->lexema) > TAM + 1) {
        free(comp->lexema);
        comp->lexema = NULL;
        lanzarErro(LEXEMA_TAM_EXCEDIDO);
    }
}

// Función que salta o lexema que se está lendo actualmente
void saltarLexema() {
    buf.inicio = buf.dianteiro;
}

// Función que finaliza o sistema de entrada
void finalizarSistemaEntrada() {
    fclose(f_codigo_fonte);
}