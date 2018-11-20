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

//agregar archivo
int lenstrv(char **);
void destruir_dato_wrapper(void *);
bool agregar_archivo(char **, hash_t *);

//prioridad vuelos
bool prioridad_vuelos(char **comandos,hash_t* hash);


int heap_min_cmp(const void* a,const void* b);
int heap_max_cmp(const void* a,const void* b);

//Función de comparación para estructura heap de máximos
int heap_max_cmp(const void* a,const void* b){
	char** vuelo1 = (char**)a;
	char** vuelo2 = (char**)b;
	int prioridad1 = atoi(vuelo1[priority]);
	int prioridad2 = atoi(vuelo2[priority]);
	if(prioridad1>prioridad2) return 1;
	else if(prioridad1<prioridad2) return -1;
	else{
		int codigo1 = atoi(vuelo1[flight_number]);
		int codigo2 = atoi(vuelo2[flight_number]);
		if(codigo1 > codigo2) return -1;
		return 1;
	}
}
//Función de comparación para estructura heap de mínimos
int heap_min_cmp(const void* a,const void* b){
	if(heap_max_cmp(a,b)>0) return -1;
	return 1;
}

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
		fprintf(stdout,"%s - %s\n",vuelo[priority],vuelo[flight_number]);
	}
	heap_destruir(heap_max,NULL);
	return true;
}

//procesa de forma completa un archivo de .csv que contiene datos de vuelos.
int lenstrv(char **strv){
	if (!strv) return -1;
	int len = 0;
	for(int i = 0; strv[i]; i++) len++;
	return len;
}

void destruir_dato_wrapper(void* dato){
	free_strv((char**)dato);
}

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


//info vuelo
bool info_vuelo(char** comandos, hash_t* hash){
	if ( lenstrv(comandos) != 2 ){
    fprintf(stderr,"%s\n","1");
    return false;
  }

	if(hash_cantidad(hash)==0){
    fprintf(stderr,"%s\n","1");
    return false;
  }
	char** vuelo = hash_obtener(hash, comandos[1]);
	if(vuelo==NULL){
    fprintf(stderr,"%s\n","1");
    return false;
  }
	for(int i=0;i<10;i++){
		fprintf(stdout,"%s",vuelo[i]);
		if(i<9)fprintf(stdout," ");
	}
	return true;
}



//test agregar archivo
void prueba1(){
  hash_t *h = hash_crear(destruir_dato_wrapper);

  char **comando = split("agregar_archivo vuelos-algueiza-parte-01.csv",' ');
  fprintf(stdout, "\n%s\n","~~Pruebas Agregar archivo~~~" );
  bool ok = agregar_archivo(comando,h);
  fprintf(stdout, "%s\n", ok ? "carga los datos al hash.. OK" : "carga los datos al hash.. ERROR" );

  hash_destruir(h);

  free_strv(comando);
}

void prueba2(){
  hash_t *h = hash_crear(destruir_dato_wrapper);

  char **comando = split("agregar_archivo vuelos-algueiza-parte-01.csv",' ');
  char **infocomando = split("info_vuelo 4608",' ');
  fprintf(stdout, "\n%s\n","~~Prueba info vuelo~~~" );
  bool ok = agregar_archivo(comando,h);
  fprintf(stdout, "%s\n", ok ? "carga los datos al hash.. OK" : "carga los datos al hash.. ERROR" );
  ok = info_vuelo(infocomando,h);
  fprintf(stdout, "%s\n", ok ? "" : "carga los datos al hash.. ERROR" );
  hash_destruir(h);

  free_strv(comando);
  free_strv(infocomando);
}

void prueba3(){
  hash_t *h = hash_crear(destruir_dato_wrapper);

  char **comando = split("agregar_archivo vuelos-algueiza-parte-01.csv",' ');
  char **prioridadvuelos = split("prioridad_vuelos 20", ' ');
  fprintf(stdout, "\n%s\n","~~Prioridad vuelos~~~" );
  bool ok = agregar_archivo(comando,h);
  fprintf(stdout, "%s\n", ok ? "carga los datos al hash.. OK" : "carga los datos al hash.. ERROR" );

  ok = prioridad_vuelos(prioridadvuelos,h);

  fprintf(stdout, "%s\n", ok ? "OK" : "carga los datos al hash.. ERROR" );

  hash_destruir(h);

  free_strv(comando);
  free_strv(prioridadvuelos);
}

void prueba4(){
	abb_t *arbol = abb_crear(strcmp, NULL);

	char *claves[] = {"1","3","5","2","4","6","10","8","9","17"};
	char *valores[] = {"1","2","3","4","5","6","7","8","9","10"};

	printf("%s\n", "~~~TEST ARBOL BINARIO~~~~\n");

	printf("%s\n","arbol cargado con 1,2,3,4,5,6,7,8,9,10" );
	printf("%s\n","itera 4 al 8" );

	for (int i = 0; i < 10; i++){
		abb_guardar(arbol,claves[i],valores[i]);
	}

	abb_iter_t *it = abb_iter_in_crear(arbol,"asc","4","8");

	while( !abb_iter_in_al_final(it) ){
		const char *clave = abb_iter_in_ver_actual(it);
		abb_iter_in_avanzar(arbol,it);
		printf("%s\n",clave);
	}
	printf("ok\n");
	abb_iter_in_destruir(it);
	abb_destruir(arbol);

}
