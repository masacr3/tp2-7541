#include "funciones.h"

//Lee el archivo .csv indicado en la línea de comandos y almacena los datos en el sistema.
bool agregar_archivo(char **comandos, hash_t* hash,abb_t* abb){
	if( lenstrv(comandos) != 2 ) return false; //chequeo cantidad de parametros.

	FILE* archivo = fopen(comandos[1],"r");
	if( archivo == NULL ) return false; //chequeo archivo válido.

	char* linea = NULL;
	size_t cant = 0;
	ssize_t leidos;

	while((leidos=getline(&linea,&cant,archivo))>0){
		linea[leidos-1]='\0';
		char** vuelo = split(linea,',');
		char* clave = crear_clave_abb(vuelo);
		bool nuevo_nodo = true; //variable de control p/actualización de datos de vuelo en abb.
		char** datos_anteriores = hash_obtener(hash,vuelo[FLIGHT_NUMBER]);
		if( datos_anteriores ){
			char* clave_anterior = crear_clave_abb(datos_anteriores);
			if (strcmp(clave_anterior,clave)==0) nuevo_nodo = false;
			else abb_borrar(abb,clave_anterior);
			free(clave_anterior);
		}
		if( nuevo_nodo ){
			if( !abb_guardar(abb,clave,NULL) ){
				free(clave);
				return false;
			}
		}
		if( !hash_guardar(hash,vuelo[FLIGHT_NUMBER],(void*)vuelo) ){
			abb_borrar(abb,clave);
			free(clave);
			return false;
		}
		free(clave);
	}
	free(linea);
	fclose(archivo);
	return true;
}

//Muestra toda la informacion del vuelo cuyo código de vuelo coincida con el que fue pasado por parametro
bool info_vuelo(char** comandos, hash_t* hash){
	if( lenstrv(comandos) != 2 ) return false; //chequeo cantidad de parametros.
	if( hash_cantidad(hash) == 0 ) return false;
	int flight_number = 1;
	char** vuelo = hash_obtener(hash, comandos[flight_number]);
	if( !vuelo ) return false;
	char* linea = join(vuelo,' ');
	if( !linea ) return false;
	fprintf(stdout,"%s\n",linea);
	free(linea);
	return true;
}

/*Función auxiliar de prioridad_vuelos. Recibe un heap de mínimos.
 *Imprime en forma descendente todos los datos del heap. El heap quedó vacío*/
void imprimir_prioridad_vuelos(heap_t* heap){
	if(!heap) return;
	if(heap_esta_vacio(heap)) return;
	
	char** vuelo = heap_desencolar(heap);
	imprimir_prioridad_vuelos(heap);
	fprintf(stdout,"%s - %s\n",vuelo[PRIORITY],vuelo[FLIGHT_NUMBER]);
}

//Muestra en orden descendente los K vuelos con mayor prioridad. 
bool prioridad_vuelos(char **comandos,hash_t* hash){
	if ( lenstrv(comandos) != 2 )return false; //chequeo cantidad de parametros.

	int K = atoi(comandos[1]);
	if ( K < 1) return false; //chequeo K válido.

	heap_t* heap = heap_crear(heap_min_cmp);
	if(heap==NULL) return false;

	hash_iter_t* iter = hash_iter_crear(hash);
	if(iter==NULL){
		heap_destruir(heap,NULL);
		return false;
	}

	int contador = 0;

	//Recorre todos los elementos para obtener los k elementos con mayor prioridad.
	while(!hash_iter_al_final(iter)){
		char** vuelo = hash_obtener(hash,hash_iter_ver_actual(iter));
		if(contador<K){
			heap_encolar(heap,vuelo);
			contador++;
		}
		else if(heap_min_cmp(vuelo,heap_ver_max(heap))<0){
			heap_desencolar(heap);
			heap_encolar(heap,hash_obtener(hash,hash_iter_ver_actual(iter)));
		}
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	imprimir_prioridad_vuelos(heap);
	heap_destruir(heap,NULL);
	return true;
}

//Muestra los K vuelos cuya fecha de despegue sea igual o mayor a < desde > e igual o menor a < hasta > 
bool ver_tablero(char** comandos,abb_t* abb){
	if ( lenstrv(comandos) != 5 )return false; //chequeo cantidad de parametros.
	
	int K = atoi(comandos[1]);
	if ( K < 1) return false; //chequeo K válido.

	char* modo = comandos[2];
	bool ok = strcmp(modo,"asc") == 0 || strcmp(modo,"desc") == 0;
	if (!ok ) return false; //chequeo modo válido.

	char* desde = comandos[3];
	char* hasta = comandos[4];
	time_t t1 = iso8601_to_time(desde);
	time_t t2 = iso8601_to_time(hasta); 
	if ( difftime(t2,t1) < 0) return false; //chequeo rango válido de tiempo.

	int date = 0;
	int flight_number = 1;
	int i = 0;

	abb_iter_t* iter = abb_iter_in_crear(abb,abb_it_cmp,modo,desde,hasta);
	if(!iter) return false;

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

//Muestra y elimina todos los vuelos cuya fecha de despegue sea igual o mayor a < desde > e igual o menor a < hasta >
bool borrar(char **comandos,abb_t* abb,hash_t* hash){
	if ( lenstrv(comandos) != 3 )return false; //chequeo cantidad de parametros

	char* desde = comandos[1];
	char* hasta = comandos[2];
	time_t t1 = iso8601_to_time(desde);
	time_t t2 = iso8601_to_time(hasta); 
	if ( difftime(t2,t1) < 0) return false; //chequeo rango válido de tiempo.

	char* modo = "asc";
	abb_iter_t* iter = abb_iter_in_crear(abb,abb_it_cmp,modo,desde,hasta);
	if(!iter) return false;

	lista_t* basurero = lista_crear();
	if(!iter||!basurero) return false;

	while(!abb_iter_in_al_final(iter)){
		lista_insertar_ultimo(basurero,(void*)abb_iter_in_ver_actual(iter));
		abb_iter_in_avanzar(iter);
	}
	abb_iter_in_destruir(iter);

	while(!lista_esta_vacia(basurero)){
		char* clave_abb = lista_borrar_primero(basurero);
		char** clave_hash = split(clave_abb,',');
		abb_borrar(abb,clave_abb);
		char** vuelo = hash_borrar(hash,clave_hash[1]);
		char* linea = join(vuelo,' ');
		if(linea){
			fprintf(stdout,"%s\n",linea);
			free(linea);
		}
		free(clave_abb);
		free_strv(vuelo);
    		free(clave_hash);
	}
	lista_destruir(basurero,NULL);
	return true;
}

//Función auxiliar del sistema utilizado por main, ejecuta el comando correspondiente.
bool ejecutar_operacion(char **comandos,hash_t* hash,abb_t* abb){
	if ( strcmp ( comandos[0],"agregar_archivo") == 0 ) return agregar_archivo(comandos,hash,abb);

	else if ( strcmp ( comandos[0],"info_vuelo") == 0 ) return info_vuelo(comandos,hash);

	else if ( strcmp ( comandos[0],"prioridad_vuelos") == 0 ) return prioridad_vuelos(comandos,hash);

	else if ( strcmp ( comandos[0],"ver_tablero") == 0 )return ver_tablero(comandos,abb);

	else if ( strcmp ( comandos[0],"borrar") == 0 ) return borrar(comandos,abb,hash);

	else return false;

}

//Función auxiliar, conversión de cadena a time_t
time_t iso8601_to_time(const char* iso8601)
{
    struct tm bktime = { 0 };
    strptime(iso8601, TIME_FORMAT, &bktime);
    return mktime(&bktime);
}

//Función de comparación para crear un heap de mínimos.
int heap_min_cmp(const void* a,const void* b){
	char** vuelo1 = (char**)a;
	char** vuelo2 = (char**)b;
	int prioridad1 = atoi(vuelo1[PRIORITY]);
	int prioridad2 = atoi(vuelo2[PRIORITY]);
	if(prioridad1>prioridad2) return -1;
	else if(prioridad1<prioridad2) return 1;
	else{
		int codigo1 = atoi(vuelo1[FLIGHT_NUMBER]);
		int codigo2 = atoi(vuelo2[FLIGHT_NUMBER]);
		if(codigo1 > codigo2) return 1;
		return -1;
	}
}

//Función auxiliar para crear clave de datos en formato 'YYYY-MM-DDTHH:MM:SS,<Código de vuelo>'
char* crear_clave_abb(char** datos){
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

//Función de comparación para crear un ABB.
int abb_cmp(const char *a, const char *b){
	char **clave1 = split(a,',');
	char **clave2 = split(b,',');
	
	int date = 0;
	time_t t1 = iso8601_to_time(clave1[date]); //tiempo de a.
	time_t t2 = iso8601_to_time(clave2[date]); //tiempo de b.
	
	int i=0;
	int flight_number = 1;
	if (difftime(t1,t2) > 0)i=1;
	else if (difftime(t1,t2) < 0)i=-1;
	else i = strcmp(clave1[flight_number],clave2[flight_number]);

	free_strv(clave1);
	free_strv(clave2);
	return i;
}

//Función auxiliar de comparación para funcionamiento de iterador en un rango de tiempo.
int abb_it_cmp(const char *a, const char *b){
	char **clave1 = split(a,',');
	char **clave2 = split(b,',');
	int date = 0;
	time_t t1 = iso8601_to_time(clave1[date]); //tiempo de a
	time_t t2 = iso8601_to_time(clave2[date]); //tiempo de b
	
	int i=0;
	if (difftime(t1,t2) > 0)i=1;
	else if (difftime(t1,t2) < 0)i=-1;
	else i = 0;

	free_strv(clave1);
	free_strv(clave2);
	return i;
}

//Función de destrucción de dato para tdas abb y hash.
void destruir_dato_wrapper(void* dato){
	free_strv((char**)dato);
}
