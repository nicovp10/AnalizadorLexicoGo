# Opcións de compilación: mostra todos os warnings (-Wall)
CC=gcc -Wall

# Carpeta das cabeceiras
HEADER_FILES_DIR = .

# Opcións de compilación: indica onde están os ficheiros .h
INCLUDES = -I $(HEADER_FILES_DIR)

# Nome do executable
OUTPUT = analizadorLexicoGo

# Ficheiros .h
LIB_HEADERS = analizadorLexico.h analizadorSintactico.h definicions.h sistemaEntrada.h taboaSimbolos.h xestionErros.h abb.h

# Ficheiros .c
SRCS = analizadorLexico.c analizadorSintactico.c main.c sistemaEntrada.c taboaSimbolos.c xestionErros.c abb.c

# Ficheros .o: todos os .o cun análogo .c en SRCS
OBJS = $(SRCS:.c=.o)


# REGRA 1: xera o executable, dependencia dos .o
$(OUTPUT): $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS)
	rm *.o

# REGRA 2: xera os .o cando é necesario, dependencia dos .c e .h
# $@ é o nome do ficheiro que se xera coa regra (.o)
# $< é o nome do primeiro prerrequisito (o arquivo .c cuxo .o se está xerando)
%.o: %.c $(LIB_HEADERS)
	$(CC) -c -o $@ $< $(INCLUDES)

# REGRA 3: borra o executable
clean:
	rm -f $(OUTPUT)
