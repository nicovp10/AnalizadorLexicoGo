# Analizador léxico del lenguaje Go

Implementación de un analizador léxico que **reconoce todos los componentes léxicos** del lenguaje de programación Go. Para la realización del mismo se ha consultado la [especificación del lenguaje](https://go.dev/ref/spec), más en concreto el [listado de componentes léxicos](https://go.dev/ref/spec#Lexical_elements).

#### Instrucciones de ejecución
Para la ejecución del analizador léxico se clonará o descargará este repositorio colocando todos los archivos en un mismo directorio. Una vez colocados, se abrirá una terminal de Linux, se accederá al directorio con los archivos y se ejecutará el comando `make` para compilarlo. Una vez compilado, el analizador léxico se ejecutará por terminal con el siguiente comando: `./analizadorLexicoGo {FICHERO_CODIGO_FUENTE_GO}`. Por ejemplo: `./analizadorLexicoGo concurrentSum.go`
