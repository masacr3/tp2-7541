EXEC = pruebas
CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes
CFILES = main.c strutil.c hash.c lista.c test.c heap.c
all:
	 $(CC) $(CFLAGS) $(CFILES) -o $(EXEC)
run: all
	./$(EXEC)
val: all
	valgrind $(VFLAGS) ./$(EXEC)
