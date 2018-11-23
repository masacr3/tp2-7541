EXEC = pruebas
CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes
CFILES = main.c strutil.c hash.c lista.c heap.c abb.c pila.c funciones.c
all:
	 $(CC) $(CFLAGS) $(CFILES) -o $(EXEC)
run: all
	./$(EXEC) < test.txt
val: all
	valgrind $(VFLAGS) ./$(EXEC) < test.txt
