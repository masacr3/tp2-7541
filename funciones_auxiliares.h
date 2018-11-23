#ifndef FUNCIONES_H
#define FUNCIONES_H
#include <string.h>
#include "strutil.h"
#include "abb.h"
#include "hash.h"
#include "heap.h"
#include "funciones_auxiliares.h"
#include "borrar.h"
#include "ver_tablero.h"
#include "agregar_archivo_.h"
#include "info_vuelos_.h"
#include "prioridades_vuelos.h"
#include <time.h>

bool ejecutar_operacion(char **comandos,hash_t* hash,abb_t* abb);
int heap_min_cmp(const void* a,const void* b);
int heap_max_cmp(const void* a,const void* b);
char * empaquetar(char **datos);
int abb_cmp(const char *a, const char *b);
int lenstrv(char **strv);
void destruir_dato_wrapper(void* dato);

#endif
