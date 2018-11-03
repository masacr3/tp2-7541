#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "strutil.h"

bool ejecutar_operacion(char **linea );

bool agregar_archivo(char **comandos);
bool ver_tablero(char **comandos);
bool info_vuelo(char **comandos);
bool prioridad_vuelos(char **comandos);
bool borrar(char **comandos);


//version para strv
int strvlen(char **strv);

//verifica
bool verifica_timeT(char *inicio, char *fin);

int main(){
	size_t cap = 0;
	char *linea = NULL;
	
	while( getline ( &linea, &cap, stdin ) > 0 ){
		char **comandos = split(linea,' ');
		bool ok = ejecutar_operacion(comandos);
		if (!ok) fprintf(stderr,"Error en el comando %s\n",comandos[0]);
		free_strv(comandos);
	} 	

	free(linea);
	return 0;
}

bool ejecutar_operacion(char **datos){

	if ( strcmp ( datos[0],"agregar_archivo") == 0 ) return agregar_archivo(datos); 
	
	else if ( strcmp ( datos[0],"ver_tablero") == 0 ) return ver_tablero(datos);
	
	else if ( strcmp ( datos[0],"info_vuelo") == 0 ) return info_vuelo(datos);
	
	else if ( strcmp ( datos[0],"prioridad_vuelos") == 0 ) return prioridad_vuelos(datos);
	
	else if ( strcmp ( datos[0],"borrar") == 0 ) return borrar(datos);
	
	else return false;
	
}

bool agregar_archivo(char **comandos){
	fprintf(stdout,"%s\n","ok");	
	return true;
}

bool ver_tablero(char **comandos){
	if( strvlen(comandos) != 5 ) return false;
	
	int k = atoi(comandos[1]);
	
	if (k < 1) return false;
	
	if ( strcmp(comandos[2],"asc") != 0) || strcmp(comandos[2],"desc") != 0 ) return false;
	
	if ( verifica_timeT(comandos[3],comandos[4]) );
	fprintf(stdout,"%s\n","ok");	
	return true;
}

bool info_vuelo(char **comandos){
	if ( strvlen(comandos) != 2 ) return false;
	fprintf(stdout,"%s\n","ok");	
	return true;
}

bool prioridad_vuelos(char **comandos){
	if ( strvlen(comandos) != 2 ) return false;
	fprintf(stdout,"%s\n","ok");	
	return true;
}

bool borrar(char **comandos){
	if ( strvlen(comandos) != 3 && verificar_timeT(comando[2],comando[3] ) return false;
	fprintf(stdout,"%s\n","ok");	
	return true;
}

int strvlen(char **strv){
	if (!strv) return -1;
	int len = 0;
	for(int i = 0, strv[i]; i++) len++;
	return len;
}

bool verifica_timeT(char *inicio, char *fin){
	//hay q preguntar esta funcion
	return true;
}
