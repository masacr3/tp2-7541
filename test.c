#include "test.h"

enum datos{
	flight_number,
	airline,
	origin_airport,
	destination_airport,
	tail_number,
	priority,
	date,
	departure_delay,
	air_time,
	cancelled,
};

int lenstrv(char **strv){
	if (!strv) return -1;
	int len = 0;
	for(int i = 0; strv[i]; i++) len++;
	return len;
}

void destruir_dato_wrapper(void* dato){
	free_strv((char**)dato);
}

//procesa de forma completa un archivo de .csv que contiene datos de vuelos.
bool agregar_archivo(char **comandos, hash_t* hash){
	if(lenstrv(comandos)!=2){
		return false;
	}
	FILE* archivo = fopen(comandos[1],"r");
	if(archivo==NULL){
		return false;
	}
	char* linea = NULL;
	size_t cant = 0;
	while(getline(&linea,&cant,archivo)>0){
		char** vuelo = split(linea,',');
		if(!hash_guardar(hash,vuelo[flight_number],vuelo))return false;
	}
	free(linea);
	fclose(archivo);
	return true;
}


//test agregar archivo
bool prueba1(){
  hash_t *h = hash_crear(destruir_dato_wrapper);

  char **comando = split("agregar_archivo vuelos-algueiza-parte-01.csv",' ');
  fprintf(stdout, "\n%s\n","~~Pruebas Agregar archivo~~~" );
  fprintf(stdout, "%s\n", agregar_archivo(comando,h) ? "carga los datos al hash.. OK" : "carga los datos al hash.. ERROR" );

  hash_destruir(h);

  free_strv(comando);

  return true;
}
