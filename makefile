#Opcións de compilación, mostra todos os warnings (-Wall)
CC=gcc -Wall

#Inclúese unha librería estándar
#Todas teñen o formato de ficheiro libNOMBRE.a
#e ao incluíla no compilador ponse -lNOMBRE
#Se non hai librerías adicionais, elimínase esta liña
LIBS = 

#Carpeta das cabeceiras (SE ESTÁN NA ACTUAL, POÑEMOS .)
HEADER_FILES_DIR = .

#Opcións de compilación, indica onde están os arquivos .h
INCLUDES = -I $(HEADER_FILES_DIR)

#Nome do executable ou arquivo de saída
OUTPUT = analizadorLexico

#Ficheros .h. Se hai varios, enuméranse co seu path completo
LIB_HEADERS = analizadorLexico.h analizadorSintactico.h definicions.h sistemaEntrada.h taboaSimbolos.h

#Fontes
SRCS = analizadorLexico.c analizadorSintactico.c main.c sistemaEntrada.c taboaSimbolos.c

#Ficheros .o: todos los .o con un análogo .c en SRCS
OBJS = $(SRCS:.c=.o)


#IMPORTANTE: as regras deben ir tabuladas á dereita
#$@ é o nome do ficheiro que se xera coa regra (.o)
#$< é o nome do primeiro prerrequisito (o arquivo .c cuxo .o se está xerando)

#REGRA 1: dependencia dos .o
	#Se non hai librerías adicionais, non existe a variable $(LIBS),
	#polo que se eliminaría $(LIBS) da regra siguinte
$(OUTPUT): $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS) $(LIBS)

#REGRA 2: xera os .o cando é necesario, dependencia dos .c e .h
%.o: %.c $(LIB_HEADERS)
	$(CC) -c -o $@ $< $(INCLUDES)

#REGRA 3: borra o executable (prerrequisito: clean)
cleanall: clean
	rm -f $(OUTPUT)

#REGRA 4: borra os ficheiros .o e os de backup (terminan en ~)
clean:	
	rm -f *.o *~
