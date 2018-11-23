#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "tiempo.h"
#include "strutil.h"
#include "hash.h"
#include "heap.h"
#include "funciones_auxiliares.h"
#include "borrar.h"
#include "ver_tablero.h"
#include "agregar_archivo_.h"
#include "info_vuelos_.h"
#include "prioridades_vuelos.h"
#include "ver_arbol.h"
#define FLIGHT_NUMBER 0
#define DATE 6
#define PRIORITY 5

bool ejecutar_operacion(char **comandos,hash_t* hash,abb_t* abb){
	if ( strcmp ( comandos[0],"agregar_archivo") == 0 ) return agregar_archivo(comandos,hash,abb); 
	
	else if ( strcmp ( comandos[0],"info_vuelo") == 0 ) return info_vuelo(comandos,hash);
	
	else if ( strcmp ( comandos[0],"prioridad_vuelos") == 0 ) return prioridad_vuelos(comandos,hash);
	
	else if ( strcmp ( comandos[0],"ver_tablero") == 0 )return ver_tablero(comandos,abb);
	
	else if ( strcmp ( comandos[0],"borrar") == 0 ) return borrar(comandos,abb,hash);
	
	else if(strcmp(comandos[0],"ver_arbol")==0) return ver_arbol(abb);
	
	else return false;
	
}


int heap_max_cmp(const void* a,const void* b){
	char** vuelo1 = (char**)a;
	char** vuelo2 = (char**)b;
	int prioridad1 = atoi(vuelo1[PRIORITY]);
	int prioridad2 = atoi(vuelo2[PRIORITY]);
	if(prioridad1>prioridad2) return 1;
	else if(prioridad1<prioridad2) return -1;
	else{
		int codigo1 = atoi(vuelo1[FLIGHT_NUMBER]);
		int codigo2 = atoi(vuelo2[FLIGHT_NUMBER]);
		if(codigo1 > codigo2) return -1;
		return 1;
	}
}

int heap_min_cmp(const void* a,const void* b){
	if(heap_max_cmp(a,b)>0) return -1;
	return 1;
}

char* empaquetar(char** datos){
	if(!datos) return NULL;
	
	char **paquete = malloc ( sizeof(char*) * 3);

	if ( !paquete) return NULL;

	paquete[0] = malloc(sizeof(char)*strlen(datos[6])); //date
	strcpy(paquete[0],datos[6]);
	paquete[1] = malloc(sizeof(char)*strlen(datos[0])); //flight_number
	strcpy(paquete[1],datos[0]);
	paquete[2] = NULL;
	
	char* res = join(paquete,',');
	free_strv(paquete);
	return res;
}

//suponemos que a , b tienen datos
int abb_cmp(const char *a, const char *b){
	char **clave1 = split(a,',');
	char **clave2 = split(b,',');

	time_t t1 = iso8601_to_time(clave1[0]); //tiempos
	time_t t2 = iso8601_to_time(clave2[0]); //tiempos
	int i=0;
	if (difftime(t1,t2) > 0)i=1;
	else if (difftime(t1,t2) < 0)i=-1;
	else{
		int flight1 = atoi(clave1[1]);
		int flight2 = atoi(clave2[1]);
		if (flight1<flight2)i=-1;
		else if (flight1>flight2)i=1;
	}
	free_strv(clave1);
	free_strv(clave2);
	return i;
}

//Devuelve el largo de un vector de cadenas.
int lenstrv(char **strv){
	if (!strv) return -1;
	int len = 0;
	for(int i = 0; strv[i]; i++) len++;
	return len;
}

//Función de destrucción de dato para tdas abb y hash.
void destruir_dato_wrapper(void* dato){
	free_strv((char**)dato);
}
