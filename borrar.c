#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "strutil.h"
#include "abb.h"
#include "hash.h"
#include "heap.h"
#include "lista.h"
#include "funciones_auxiliares.h"

bool borrar(char **comandos,abb_t* abb,hash_t* hash){
	if ( lenstrv(comandos) != 3 )return false;
	char* desde = comandos[1];
	char* hasta = comandos[2];
	char* modo = "asc";
	abb_iter_t* iter = abb_iter_in_crear(abb,modo,desde,hasta);
	lista_t* elem_rango = lista_crear();
	if(!iter||!elem_rango) return false;
	
	while(!abb_iter_in_al_final(iter)){
		lista_insertar_ultimo(elem_rango,(void*)abb_iter_in_ver_actual(iter));
		abb_iter_in_avanzar(abb,iter);
	}
	abb_iter_in_destruir(iter);
	while(!lista_esta_vacia(elem_rango)){
		char* clave = abb_borrar(abb,lista_borrar_primero(elem_rango));
		char** vuelo = hash_borrar(hash,clave);
		free_strv(vuelo);
	}
	lista_destruir(elem_rango,NULL);
	return true;
}
