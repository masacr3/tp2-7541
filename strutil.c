#include "strutil.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void strvlen ( char** s, size_t* len, size_t* buffer){
	*len = 0;
	*buffer = 0;
	
	for(int i=0; s[i]; i++){
		*len += 1;
		*buffer += strlen(s[i]);
	}
}

/* devuelve la cantidad de bytes hasta el separador*/
size_t buscar(const char* s, char sep){
	size_t cont = 0;
	
	for(int i=0; s[i] && s[i] != sep; i++) cont++;
	
	return cont;
}

/* devuelve la cantidad de aparaciones del separador */
size_t contar_sep(const char* s, char sep){
	size_t cont = 0;
	
	for(int i=0; s[i]; i++) if( s[i] == sep ) cont++;
	
	return cont;
}


char **split(const char* str, char sep){
	if (!str) return NULL;
	
	size_t cant_sep = contar_sep(str,sep);
	
	char** _split = (char**)malloc(sizeof(char*) * (cant_sep + 2) );
	
	if (!_split) return NULL;
	
	_split[ cant_sep + 1 ] = NULL;
	
	size_t inicio = 0;
	
	for (int i=0; i <= cant_sep; i++){
		//calculo cuantos bytes voy a copiar
		size_t bytes = buscar(&str[inicio], sep);
		
		//creo la cadena + 1 (para el caracter '\0'
		char* s = malloc( sizeof(char) * (bytes + 1));
	
		if( !s ){
			free_strv(_split);
			return NULL;
		}
		
		//copio los caracteres a s
		memcpy(s, &str[inicio], bytes);
		
		//inserto el byte '\0'
		s[bytes] = '\0';
		
		//vinculo la cadena al split
		_split[i] = s;
		
		//salto el separador
		inicio += bytes + 1;
		
	}
	return _split;
}

char* join (char** strv, char sep){
	
	if (sep == '\0' || !strv) return NULL;
	
	size_t len;
	size_t buffer;
	
	//obtengo len y la cantidad de caracteres
	strvlen(strv, &len, &buffer);
	
	size_t bytes = len + buffer;
	
	char* _join = malloc( sizeof(char) * (bytes + 1) );
	
	if( !_join) return NULL;
	
	_join[bytes] = '\0';
	
	if (!len) return _join;
	
	size_t largo = 0;
	
	for(int i=0; i<len-1; i++){
		//copio la cadena al join
		strcpy(&_join[largo],strv[i]);

		//calculo la posicion del separador
		largo += strlen(strv[i]);
		
		//agrego el separador
		_join[largo] = sep;
		
		//avanzo una posicion
		largo++;
	}

	strcpy(&_join[largo], strv[len-1]);
	
	return _join;
}

void free_strv(char *strv[]){
    for (int i = 0; strv[i]; i++) {
        free(strv[i]);
    }
	free(strv);
}
