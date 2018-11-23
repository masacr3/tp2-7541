#include "funciones.h"

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
		if(!hash_guardar(hash,vuelo[FLIGHT_NUMBER],(void*)vuelo))return false;
		char* clave = empaquetar(vuelo);
		if(!abb_guardar(abb,clave,vuelo[FLIGHT_NUMBER])){
			free(hash_borrar(hash,vuelo[FLIGHT_NUMBER]));
			free(clave);
			return false;
		}
		free(clave);
	}
	free(linea);
	fclose(archivo);
	return true;
}

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

bool ver_tablero(char** comandos,abb_t* abb){
	if ( lenstrv(comandos) != 5 )return false;
	int K = atoi(comandos[1]);
	char* modo = comandos[2];
	char* desde = comandos[3];
	char* hasta = comandos[4];
  int date = 0; //para que no genere colisiones con los define
  int flight_number = 1; //para que no genere colisiones con los define
	abb_iter_t* iter = abb_iter_in_crear(abb,modo,desde,hasta);
	if(!iter) return false;
	int i = 0;
	while(!abb_iter_in_al_final(iter)&&i<K){
		char** vuelo = split(abb_iter_in_ver_actual(iter),',');
		fprintf(stdout,"%s - %s\n",vuelo[date],vuelo[flight_number]);
		free_strv(vuelo);
		abb_iter_in_avanzar(iter);
		i++;
	}
	abb_iter_in_destruir(iter);
	return true;
}

bool ejecutar_operacion(char **comandos,hash_t* hash,abb_t* abb){
	if ( strcmp ( comandos[0],"agregar_archivo") == 0 ) return agregar_archivo(comandos,hash,abb);

	else if ( strcmp ( comandos[0],"info_vuelo") == 0 ) return info_vuelo(comandos,hash);

	else if ( strcmp ( comandos[0],"prioridad_vuelos") == 0 ) return prioridad_vuelos(comandos,hash);

	else if ( strcmp ( comandos[0],"ver_tablero") == 0 )return ver_tablero(comandos,abb);

	//else if ( strcmp ( comandos[0],"borrar") == 0 ) return borrar(comandos,abb,hash);

	else return false;

}


time_t iso8601_to_time(const char* iso8601)
{
    struct tm bktime = { 0 };
    strptime(iso8601, TIME_FORMAT, &bktime);
    return mktime(&bktime);
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

	paquete[0] = malloc(sizeof(char)*strlen(datos[DATE])); //date
	strcpy(paquete[0],datos[DATE]);
	paquete[1] = malloc(sizeof(char)*strlen(datos[FLIGHT_NUMBER])); //flight_number
	strcpy(paquete[1],datos[FLIGHT_NUMBER]);
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
    if ( strcmp (clave1[1], clave2[1]) == 0) i =0;
    else if ( strcmp (clave1[1], clave2[1]) < 0) i = -1;
		else i=1;
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
