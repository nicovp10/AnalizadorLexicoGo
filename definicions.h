#ifndef P1_DEFINICIONS_H
#define P1_DEFINICIONS_H


typedef struct {
    int comp_lexico;
    char *lexema;
} CompLexico;


#define ID 300

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

#define ANGULARESESQ 326        // <<
#define ANGULARESDER 327        // >>
#define AMPEREXP 328            // &^
#define SUMAIGUAL 329           // +=
#define RESTAIGUAL 330          // -=
#define MULTIGUAL 331           // *=
#define DIVIGUAL 332            // /=
#define MODIGUAL 333            // %=
#define AMPERIGUAL 334          // &=
#define VERTIGUAL 335           // |=
#define EXPIGUAL 336            // ^=
#define ANGULARESESQIGUAL 337   // <<=
#define ANGULARESDERIGUAL 338   // >>=
#define AMPEREXPIGUAL 339       // &^=
#define AMPERDOBLE 340          // &&
#define VERTDOBLE 341           // ||
#define FRECHAESQ 342           // <-
#define SUMADOBLE 343           // ++
#define RESTADOBLE 344          // --
#define IGUALDOBLE 345          // ==
#define EXCLAMACIONIGUAL 346    // !=
#define MENORIGUAL 347          // <=
#define MAIORIGUAL 348          // >=
#define DOUSPUNTOSIGUAL 349     // :=
#define PUNTOTRIPLE 350         // ...

#define INT 351
#define FLOAT 352
#define IMAGINARY 353

#define RUNE 354
#define STRING 355


#endif