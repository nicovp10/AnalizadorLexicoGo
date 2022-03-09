#ifndef P1_DEFINICIONS_H
#define P1_DEFINICIONS_H


typedef struct compLexico {
    int comp_lexico;
    char *lexema;
} CompLexico;


#define IDENTIFICADOR 300

#define BREAK 301
#define CASE 302
#define CHAN 303
#define CONST 304
#define CONTINUE 305
#define DEFAULT 306
#define DEFER 307
#define ELSE 308
#define FALLTHROUGH 309
#define FOR 310
#define FUNC 311
#define GO 312
#define GOTO 313
#define IF 314
#define IMPORT 315
#define INTERFACE 316
#define MAP 317
#define PACKAGE 318
#define RANGE 319
#define RETURN 320
#define SELECT 321
#define STRUCT 322
#define SWITCH 323
#define TYPE 324
#define VAR 325

#define SUMA 326                // +
#define RESTA 327               // -
#define MULT 328                // *
#define DIV 329                 // /
#define MOD 330                 // %
#define AMPER 331               // &
#define VERT 332                // |
#define EXP 333                 // ^
#define ANGULARESIZQ 334        // <<
#define ANGULARESDER 335        // >>
#define AMPEREXP 336            // &^
#define SUMAIGUAL 337           // +=
#define RESTAIGUAL 338          // -=
#define MULTIGUAL 339           // *=
#define DIVIGUAL 340            // /=
#define MODIGUAL 341            // %=
#define AMPERIGUAL 342          // &=
#define VERTIGUAL 343           // |=
#define EXPIGUAL 344            // ^=
#define ANGULARESIZQIGUAL 345   // <<=
#define ANGULARESDERIGUAL 346   // >>=
#define AMPEREXPIGUAL 347       // &^=
#define AMPERDOBLE 348          // &&
#define VERTDOBLE 349           // ||
#define FLECHAIZQ 350           // <-
#define SUMADOBLE 351           // ++
#define RESTADOBLE 352          // --
#define IGUALDOBLE 353          // ==
#define MENORQUE 354            // <
#define MAYORQUE 355            // >
#define IGUAL 356               // =
#define EXCLAMACION 357         // !
#define EXCLAMACIONIGUAL 358    // !=
#define MENORIGUAL 359          // <=
#define MAYORIGUAL 360          // >=
#define DOSPUNTOSIGUAL 361      // :=
#define PUNTOTRIPLE 362         // ...
#define PARENTESISIZQ 363       // (
#define CORCHETEIZQ 364         // [
#define CHAVEIQZ 365            // {
#define COMA 366                // ,
#define PUNTO 367               // .
#define PARENTESISDER 368       // )
#define CORCHETEDER 369         // ]
#define CHAVEDER 370            // }
#define PUNTOECOMA 371          // ;
#define DOUSPUNTOS 372          // :

#define INT 373
#define FLOAT 374
#define IMAGINARY 375
#define RUNE 376
#define STRING 377


#endif