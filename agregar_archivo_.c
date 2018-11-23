#define _POSIX_C_SOURCE 200809L
#include "agregar_archivo_.h"
#include "funciones_auxiliares.h"
#include "strutil.h"
#include "hash.h"
#define flight_number 0

//procesa de forma completa un archivo de .csv que contiene datos de vuelos.
bool agregar_archivo(char **comandos, hash_t* hash,abb_t* abb){
	if(lenstrv(comandos)!=2){
		return false;
	}
	FILE* archivo = fopen(comandos[1],"r");
	if(archivo==NULL){
		return false;
	}
	char* linea = NULL;
	size_t cant = 0;
	ssize_t leidos;
	while((leidos=getline(&linea,&cant,archivo))>0){
		linea[leidos-1]='\0';
		char** vuelo = split(linea,',');
		if(!hash_guardar(hash,vuelo[flight_number],(void*)vuelo))return false;
		char* clave = empaquetar(vuelo);
		if(!abb_guardar(abb,clave,vuelo[flight_number])){
			free(hash_borrar(hash,vuelo[flight_number]));
			free(clave);
			return false;
		}
		free(clave);
	}
	free(linea);
	fclose(archivo);
	return true;
}
