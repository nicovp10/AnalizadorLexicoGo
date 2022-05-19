# Analizador léxico del lenguaje Go

## Introducción
Este analizador léxico se ha realizado para aprender de forma didáctica como desarrollar desde cero un analizador léxico teniendo en cuenta todos los factores, como por ejemplo el sistema de entrada, la tabla de símbolos o la gestión de la memoria. Para la realización del mismo se ha consultado la [especificación del lenguaje](https://go.dev/ref/spec), más en concreto el [listado de componentes léxicos](https://go.dev/ref/spec#Lexical_elements).

## Compilación y ejecución
Para la compilación del analizador léxico se clonará o descargará este repositorio colocando todos los archivos en un mismo directorio. Una vez colocados, se abrirá una terminal de Linux, se accederá al directorio con los archivos y se ejecutará el comando `make` para compilarlo.

Una vez compilado, el analizador léxico se ejecutará por terminal con el siguiente comando: `./analizadorLexicoGo {FICHERO_CODIGO_FUENTE_GO}`. Por ejemplo: `./analizadorLexicoGo concurrentSum.go`.

Como resultado de la ejecución se imprimirá por consola primeramente la tabla de símbolos inicial (contendrá solamente las palabras clave del lenguaje), posteriormente imprimirá todos los componentes léxicos detectados en el código fuente con el formato `< COMP_LEXICO, LEXEMA >`, y finalmente imprimirá la tabla de símbolos resultante tras la ejecución, que contendrá, a mayores de las palabras reservadas, todos los identificadores detectados.
