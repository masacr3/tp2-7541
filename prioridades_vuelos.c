#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "strutil.h"
#include "hash.h"
#include "heap.h"
#include "funciones_auxiliares.h"
#define FLIGHT_NUMBER 0
#define DATE 6
#define PRIORITY 5

//muestra los códigos de los K vuelos que tienen mayor prioridad.
bool prioridad_vuelos(char **comandos,hash_t* hash){
	if ( lenstrv(comandos) != 2 )return false;
	int K = atoi(comandos[1]);
	heap_t* heap = heap_crear(heap_min_cmp);
	if(heap==NULL) return false;
	hash_iter_t* iter = hash_iter_crear(hash);
	if(iter==NULL){
		heap_destruir(heap,NULL);
		return false;
	}
	//Crea un heap de mínimos de tamaño k con los primeros k elementos.
	for(int i = 0;i<K;i++){
		if(!hash_iter_al_final(iter)){
			char** vuelo = hash_obtener(hash,hash_iter_ver_actual(iter));
			heap_encolar(heap,vuelo);
			hash_iter_avanzar(iter);
		}
	}
	//Recorre todos los elementos para obtener los k elementos con mayor prioridad.
	while(!hash_iter_al_final(iter)){
		char** vuelo = hash_obtener(hash,hash_iter_ver_actual(iter));
		if(heap_max_cmp(vuelo,heap_ver_max(heap))>0){
			heap_desencolar(heap);
			heap_encolar(heap,hash_obtener(hash,hash_iter_ver_actual(iter)));
		}
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	heap_t* heap_max = heap_crear(heap_max_cmp);
	if(heap_max==NULL){
		heap_destruir(heap,NULL);
		return false;
	}
	//Vacía los k elementos con mayor prioridad en un heap de máximos
	//obteniendo el orden mayor a menor.
	while(!heap_esta_vacio(heap)){
		heap_encolar(heap_max,heap_desencolar(heap));
	}
	heap_destruir(heap,NULL);
	//Muestra los k elementos obtenidos por salida estandar de mayor a menor.
	while(!heap_esta_vacio(heap_max)){
		char** vuelo = heap_desencolar(heap_max);
		fprintf(stdout,"%s - %s\n",vuelo[PRIORITY],vuelo[FLIGHT_NUMBER]);
	}
	heap_destruir(heap_max,NULL);
	return true;
}
