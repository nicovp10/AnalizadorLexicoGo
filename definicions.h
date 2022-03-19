#ifndef P1_DEFINICIONS_H
#define P1_DEFINICIONS_H


typedef struct {
    int comp_lexico;
    char *lexema;
} CompLexico;


#define ID 300
#define BLANK_ID 301

#define BREAK 302
#define CASE 303
#define CHAN 304
#define CONST 305
#define CONTINUE 306
#define DEFAULT 307
#define DEFER 308
#define ELSE 309
#define FALLTHROUGH 310
#define FOR 311
#define FUNC 312
#define GO 313
#define GOTO 314
#define IF 315
#define IMPORT 316
#define INTERFACE 317
#define MAP 318
#define PACKAGE 319
#define RANGE 320
#define RETURN 321
#define SELECT 322
#define STRUCT 323
#define SWITCH 324
#define TYPE 325
#define VAR 326

#define SUMA 327                // +
#define RESTA 328               // -
#define MULT 329                // *
#define DIV 330                 // /
#define MOD 331                 // %
#define AMPER 332               // &
#define VERT 333                // |
#define EXP 334                 // ^
#define ANGULARESIZQ 335        // <<
#define ANGULARESDER 336        // >>
#define AMPEREXP 337            // &^
#define SUMAIGUAL 338           // +=
#define RESTAIGUAL 339          // -=
#define MULTIGUAL 340           // *=
#define DIVIGUAL 341            // /=
#define MODIGUAL 342            // %=
#define AMPERIGUAL 343          // &=
#define VERTIGUAL 344           // |=
#define EXPIGUAL 345            // ^=
#define ANGULARESIZQIGUAL 346   // <<=
#define ANGULARESDERIGUAL 347   // >>=
#define AMPEREXPIGUAL 348       // &^=
#define AMPERDOBLE 349          // &&
#define VERTDOBLE 350           // ||
#define FLECHAIZQ 351           // <-
#define SUMADOBLE 352           // ++
#define RESTADOBLE 353          // --
#define IGUALDOBLE 354          // ==
#define MENORQUE 355            // <
#define MAYORQUE 356            // >
#define IGUAL 357               // =
#define EXCLAMACION 358         // !
#define EXCLAMACIONIGUAL 359    // !=
#define MENORIGUAL 360          // <=
#define MAYORIGUAL 361          // >=
#define DOSPUNTOSIGUAL 362      // :=
#define PUNTOTRIPLE 363         // ...
#define PARENTESISIZQ 364       // (
#define CORCHETEIZQ 365         // [
#define CHAVEIQZ 366            // {
#define COMA 367                // ,
#define PUNTO 368               // .
#define PARENTESISDER 369       // )
#define CORCHETEDER 370         // ]
#define CHAVEDER 371            // }
#define PUNTOECOMA 372          // ;
#define DOUSPUNTOS 373          // :

#define INT 374
#define FLOAT 375
#define IMAGINARY 376
#define RUNE 377
#define STRING 378


#endif