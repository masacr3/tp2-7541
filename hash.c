/* *****************************************************************
 *    PADRÓN: 101741 NOMBRE: GABRIELA ELIZABETH MÉNDEZ GUTIÉRREZ   *
 *	  PADRÓN: 98900  NOMBRE: LUIS IGNACIO VARADY ROCA			   *
 *    CORRECTOR: EZEQUIEL 				   				  		   *
 * *****************************************************************/
#include "hash.h"
#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define TAM_INI 50
#define FACTOR_CAP 5
#define FACTOR_REDIM 2
#define FACTOR_MIN_CAP 4

/* *****************************************************************
 *                		ESTRUCTURA  HASH 						   *
 * *****************************************************************/

//ESTRUCTURA HASH_CAMPO
typedef struct hash_campo{
	char* clave;
	void* dato;
}hash_campo_t;

//Crea un campo
//Devuelve un campo con el clave y dato asignados.
hash_campo_t* hash_campo_crear(const char* clave, void* dato){
	hash_campo_t* campo = malloc(sizeof(hash_campo_t));
	if(campo==NULL){
		return NULL;
	}
	campo->dato = dato;
	char* str= malloc(sizeof(char)*strlen(clave)+1);
	if(str==NULL){
		free(campo);
		return NULL;
	}
	memcpy(str, clave,strlen(clave)+1);
	campo->clave = str;
	return campo;
}

void hash_campo_destruir(hash_campo_t* campo,void dest_dato(void*)){
	if(dest_dato!=NULL){
		dest_dato(campo->dato);
	}
	free(campo->clave);
	free(campo);
}
//ESTRUCTURA HASH
struct hash{
	lista_t** tabla;
	hash_destruir_dato_t destruir_dato;
	size_t cant;
	size_t cap;
	size_t tam;
};

//FUNCIÓN DE HASHING
size_t f_hashing(size_t tam,const char* clave){
	unsigned char* str_aux = (unsigned char*)clave;
	unsigned long hash = 5381;
	unsigned int aux;
	while((aux=*str_aux++)!=0)hash=((hash<<5)+hash)+aux;
	return hash%tam;
}

/* *****************************************************************
 *                    PRIMITIVAS DEL HASH 			   *
 * *****************************************************************/

/* Crea el hash*/
hash_t* hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if(hash==NULL){
		return NULL;
	}
	hash->cant = 0;
	hash->cap = TAM_INI*FACTOR_CAP;
	hash->tam = TAM_INI;
	hash->tabla = malloc(TAM_INI*sizeof(lista_t*));
	if(hash->tabla == NULL){
		return NULL;
	}
	for(size_t i = 0;i<TAM_INI;i++){
		hash->tabla[i]=lista_crear();
		if (hash->tabla[i] == NULL) hash_destruir(hash);
	}
	hash->destruir_dato = destruir_dato;
	return hash;
}

bool hash_redimensionar(hash_t* hash, size_t nuevo_tam){
	lista_t** tabla_ant = hash->tabla;
	size_t ant_tam = hash->tam;
	lista_t** tabla_nueva = malloc(nuevo_tam*sizeof(lista_t*));
	if (tabla_nueva == NULL) return false;
	for(size_t i = 0;i<nuevo_tam;i++){
		tabla_nueva[i]=lista_crear();
		if (tabla_nueva[i] == NULL)
		{
			for (size_t j = 0; j < i; j++) lista_destruir(tabla_nueva[j], NULL);
			free(tabla_nueva);
			return false;
		}
	}	
	hash->tabla = tabla_nueva;
	hash->cant = 0;
	hash->cap = nuevo_tam*FACTOR_CAP;
	hash->tam = nuevo_tam;
	if(tabla_nueva == NULL){
		return false;
	}
	for(size_t i = 0; i<ant_tam;i++){
		if(tabla_ant[i]!=NULL){
			while(!lista_esta_vacia(tabla_ant[i])){
				hash_campo_t* campo = lista_borrar_primero(tabla_ant[i]);
				hash_guardar(hash,campo->clave,campo->dato);
				hash_campo_destruir(campo,NULL);
			}
			lista_destruir(tabla_ant[i],NULL);
		}		
	}
	free(tabla_ant);
	return true;
}

/* Devuelve el campo con la clave pasada por parametro. 
 * Devuelve NULL si no se encuentra en la lista o si la lista está vacía.
 * Pre: La lista fue creada.
 */
lista_iter_t* lista_buscar_clave(const hash_t *hash, const char *clave){
	size_t pos = f_hashing(hash->tam,clave);
	if(lista_esta_vacia(hash->tabla[pos])){
		return false;
	}
	lista_iter_t* iterador = lista_iter_crear(hash->tabla[pos]);
	while(!lista_iter_al_final(iterador)){
		hash_campo_t* campo = lista_iter_ver_actual(iterador);
		if(strcmp(campo->clave,clave)==0){
			return iterador;
		}
		lista_iter_avanzar(iterador);
	}
	lista_iter_destruir(iterador);
	return NULL;
}

/* Determina si clave pertenece o no al hash.
 * Pre: La estructura hash fue inicializada
 */
bool hash_pertenece(const hash_t *hash, const char *clave){
	lista_iter_t* iter = lista_buscar_clave(hash,clave);
	if(iter == NULL)
	{
		lista_iter_destruir(iter);
		return false;
	}
	lista_iter_destruir(iter);
	return true;
}


/* Guarda un elemento en el hash, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura hash fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	size_t pos = f_hashing(hash->tam,clave);
	bool guardado = false;
	if(hash_pertenece(hash,clave)){
		lista_iter_t* iter = lista_buscar_clave(hash, clave);
		hash_campo_t* campo = lista_iter_ver_actual(iter);
		lista_iter_destruir(iter);
		if(hash->destruir_dato!=NULL){
			hash->destruir_dato(campo->dato);
		}
		campo->dato=dato;
		guardado=true;
	}else{
		hash_campo_t* nuevo_campo = hash_campo_crear(clave,dato);
		if(nuevo_campo==NULL)return false;
		guardado = lista_insertar_primero(hash->tabla[pos],nuevo_campo);
		hash->cant+=1;
	}
	if(guardado&&hash->cant>=hash->cap){
		hash_redimensionar(hash,hash->tam*FACTOR_REDIM);
	}
	return guardado;
}

/* Borra un elemento del hash y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura hash fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void* hash_borrar(hash_t *hash, const char *clave){
	if(!hash_pertenece(hash,clave)){
		return NULL;
	}
	void* dato = NULL;
	lista_iter_t* iter = lista_buscar_clave(hash, clave);
	hash_campo_t* campo = lista_iter_borrar(iter);
	dato = campo->dato;
	hash_campo_destruir(campo,NULL);
	hash->cant--;
	lista_iter_destruir(iter);
	size_t cap_aux = hash->cap/FACTOR_MIN_CAP;
	size_t tam_aux = hash->tam/FACTOR_REDIM;
	if(hash->cant<=cap_aux&&tam_aux>=TAM_INI){
		hash_redimensionar(hash,tam_aux);
	}
	return dato;
}

/* Obtiene el valor de un elemento del hash, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura hash fue inicializada
 */
void *hash_obtener(const hash_t *hash, const char *clave){
	if(!hash_pertenece(hash,clave)){
		return NULL;
	}
	lista_iter_t* iter = lista_buscar_clave(hash,clave);
	hash_campo_t* campo = lista_iter_ver_actual(iter);
	lista_iter_destruir(iter);
	if(campo != NULL){
		return campo->dato;
	}
	return NULL;
}

/* Devuelve la cantidad de elementos del hash.
 * Pre: La estructura hash fue inicializada
 */
size_t hash_cantidad(const hash_t *hash){
	return hash->cant;
}

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida
 */
void hash_destruir(hash_t *hash){
	for(size_t i = 0; i < hash->tam; i++){
		while(!lista_esta_vacia(hash->tabla[i])){
			hash_campo_t* campo = lista_borrar_primero(hash->tabla[i]);
			hash_campo_destruir(campo,hash->destruir_dato);
		}
		lista_destruir(hash->tabla[i],NULL);
	}
	free(hash->tabla);
	free(hash);
}


/* *****************************************************************
 *                			 ITERADOR			   				   *
 * *****************************************************************/

//ESTRUCTURA ITERADOR
struct hash_iter{
	size_t indice;
	const hash_t* hash;
	lista_iter_t* l_iter;
};

// Crea iterador del hash
// Pre: el hash fue creado.
hash_iter_t* hash_iter_crear(const hash_t *hash) {
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if (iter == NULL) return NULL;
	iter->indice = 0;
	iter->hash = hash;
	while(iter->indice < iter->hash->tam){
		if(lista_esta_vacia(iter->hash->tabla[iter->indice])){
			iter->indice++;
		}else{
			break;
		}
	}
	if(iter->indice<iter->hash->tam){
		iter->l_iter=lista_iter_crear(iter->hash->tabla[iter->indice]);
	}else{
		iter->l_iter = NULL;
	}
	return iter;
}

// Avanza iterador
// Pre: el iterador ha sido creado.
// Post: el iterador avanza una posición en el hash.
// Devuelve false si el iterador está al final.
bool hash_iter_avanzar(hash_iter_t *iter) {
	if (hash_iter_al_final(iter)) return false;
	lista_iter_avanzar(iter->l_iter);
	if (lista_iter_al_final(iter->l_iter)) {
		lista_iter_destruir(iter->l_iter);
		iter->indice++;
		while(!hash_iter_al_final(iter)){
			if(lista_esta_vacia(iter->hash->tabla[iter->indice])){
				iter->indice++;
			}else{
				iter->l_iter=lista_iter_crear(iter->hash->tabla[iter->indice]);
				return true;
			}
		}
		return false;
	}
	return true;
}

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
// Pre: el iterador fue creado.
const char *hash_iter_ver_actual(const hash_iter_t *iter) {
	if(hash_iter_al_final(iter)) return NULL;
	hash_campo_t* campo = lista_iter_ver_actual(iter->l_iter);
	return campo->clave;
}

// Comprueba si terminó la iteración
// Pre: el iterador fue creado.
// Devuelve true o false, dependiendo si está o no al final del hash.
bool hash_iter_al_final(const hash_iter_t *iter) {
	if (iter->indice == (iter->hash->tam)||iter->l_iter==NULL){
		return true;
	}
	return false;
}

// Destruye iterador
// Pre: el iterador fue creado.
void hash_iter_destruir(hash_iter_t* iter) {
    free(iter);
}
