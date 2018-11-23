#define _POSIX_C_SOURCE 200809L
//para tiempo
#define _GNU_SOURCE
#define _XOPEN_SOURCE
#define TIME_FORMAT "%FT%T%z"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "strutil.h"
#include "hash.h"
#include "lista.h"
#include "abb.h"
#include "pila.h"
#include "heap.h"

#define FLIGHT_NUMBER 0
#define DATE 6
#define PRIORITY 5

//para el main
bool ejecutar_operacion(char **comandos,hash_t* hash,abb_t* abb);

//para el heap
int heap_min_cmp(const void* a,const void* b);
int heap_max_cmp(const void* a,const void* b);

//para agregar_archivo
char * empaquetar(char **datos);

//para el abb
int abb_cmp(const char *a, const char *b);

//nose ahre
int lenstrv(char **strv);
void destruir_dato_wrapper(void* dato);

//funcion para el tiempo o
time_t iso8601_to_time(const char* iso8601);


//funciones principales
bool agregar_archivo(char **comandos, hash_t* hash,abb_t* abb);

bool info_vuelo(char** comandos, hash_t* hash);

bool prioridad_vuelos(char **comandos,hash_t* hash);
