#include "ver_tablero.h"
#define FLIGHT_NUMBER 1
#define DATE 0

bool ver_tablero(char** comandos,abb_t* abb){
	if ( lenstrv(comandos) != 5 )return false;
	int K = atoi(comandos[1]);
	char* modo = comandos[2];
	char* desde = comandos[3];
	char* hasta = comandos[4];
	abb_iter_t* iter = abb_iter_in_crear(abb,modo,desde,hasta);
	if(!iter) return false;
	int i = 0;
	while(!abb_iter_in_al_final(iter)&&i<K){
		char** vuelo = split(abb_iter_in_ver_actual(iter),',');
		fprintf(stdout,"%s - %s\n",vuelo[DATE],vuelo[FLIGHT_NUMBER]);
		free_strv(vuelo);
		abb_iter_in_avanzar(iter);
		i++;
	}
	abb_iter_in_destruir(iter);
	return true;
}
