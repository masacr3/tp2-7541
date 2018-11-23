#include <stdlib.h>
#include "info_vuelos_.h"
#include "strutil.h"
#include "funciones_auxiliares.h"

//Muestra toda la información posible en sobre el vuelo que tiene el código pasado por parámetro.
bool info_vuelo(char** comandos, hash_t* hash){
	if ( lenstrv(comandos) != 2 )return false;
	if(hash_cantidad(hash)==0)return false;
	char** vuelo = hash_obtener(hash, comandos[1]);
	if(!vuelo)return false;
	char* linea = join(vuelo,' ');
	if(!linea) return false;
	fprintf(stdout,"%s\n",linea);
	free(linea);
	return true;
}
