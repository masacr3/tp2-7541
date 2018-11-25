#include "funciones.h"

int main(){
	size_t cap = 0;
	char *linea = NULL;
	hash_t* hash = hash_crear(destruir_dato_wrapper);
	abb_t* abb = abb_crear(abb_cmp,NULL);
	while( getline ( &linea, &cap, stdin ) != EOF ){
		size_t len_linea = strlen(linea);
		if(linea[len_linea-1]=='\n')linea[len_linea-1]='\0';
		char **comandos = split(linea,' ');
		bool ok = ejecutar_operacion(comandos,hash,abb);
		if (!ok) fprintf(stderr,"Error en comando %s\n",comandos[0]);
		else fprintf(stdout,"OK\n");
		free_strv(comandos);
	}
	abb_destruir(abb);
	hash_destruir(hash);
	free(linea);
	return 0;
}
