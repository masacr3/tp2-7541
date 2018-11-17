/* ******************************************************************
 * LISTA - lista.c			                                        *
 * AYUDANTE: MILENA MARCHESE										*
 * PADRÓN: 101741 													*
 * NOMBRE: GABRIELA ELIZABETH MÉNDEZ GUTIÉRREZ						*
 * ******************************************************************/
#include "lista.h"
#include <stdlib.h>

/* TDA NODO*/

typedef struct nodo{
	void* dato;
	struct nodo* prox;
}nodo_t;

nodo_t* nodo_crear(void* dato){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(nodo==NULL)return NULL;
	nodo->dato = dato;
	nodo->prox = NULL;
	return nodo;
}
 
/* TDA LISTA */

struct lista{
	nodo_t* prim;
	nodo_t* ult;
	size_t largo;
};

lista_t* lista_crear(void){
/*Crea una lista vacía*/
	lista_t* lista = malloc(sizeof(lista_t));
	if(lista==NULL)return NULL;
	lista->prim = NULL;
	lista->ult = NULL;
	lista->largo = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista){
/*Verifica si la lista está vacía o no*/
	return lista->largo == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
/*Agrega un nuevo elemento a la lista en la primera posición */
	nodo_t* nuevo = nodo_crear(dato);
	if(nuevo==NULL)return false;
	nuevo->prox = lista->prim;
	lista->prim = nuevo;
	if(lista->ult == NULL)lista->ult = nuevo;
	lista->largo+=1;
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
/*Agrega un nuevo elemento a la lista en la última posición*/
	nodo_t* nuevo = nodo_crear(dato);
	if(nuevo==NULL)return false;
	if(!lista_esta_vacia(lista)){
		lista->ult->prox = nuevo;
	}else{
		lista->prim = nuevo;
	}
	lista->ult = nuevo;
	lista->largo+=1;
	return true;
}
	

void *lista_borrar_primero(lista_t *lista){
/* Elimina el primer elemento de la lista */
	if(lista_esta_vacia(lista))return NULL;
	nodo_t* aux = lista->prim;
	void* dato = aux->dato;
	if(lista->prim==lista->ult){
		lista->ult = NULL;
		lista->prim = NULL;
	}else{
		lista->prim = aux->prox;
	}
	free(aux);
	lista->largo-=1;
	return dato;
}

void *lista_ver_primero(const lista_t *lista){
/* Devuelve el primer elemento de la lista */
	if(lista_esta_vacia(lista))return NULL;
	return lista->prim->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
/* Devuelve el último elemento de la lista */
	if(lista_esta_vacia(lista))return NULL;
	return lista->ult->dato;
}

size_t lista_largo(const lista_t *lista){
/* Devuelve el número de elementos en la lista */
	return lista->largo;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)){
/* Destruye la lista y los elementos dentro de ella con la función destruir_dato */
	while(!lista_esta_vacia(lista)){
		void* dato = lista_borrar_primero(lista);
		if(destruir_dato!=NULL){
			destruir_dato(dato);
		}
	}
	free(lista);
}

/* ITERADOR INTERNO */

/*
Procesa los elementos de la lista. con una función visitar y un dato extra para mantener memoria
* entre llamadas.
*/
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	nodo_t* actual = lista->prim;
	while(actual!=NULL){
		void* dato = actual->dato;
		if(!visitar(dato,extra)){
			break;
		}
		actual = actual->prox;
	}
}

/* ITERADOR EXTERNO */

struct lista_iter {
	nodo_t* anterior;
	nodo_t* actual;
	lista_t* lista;
};

lista_iter_t* lista_iter_crear(lista_t *lista){
/* Crea un nuevo iterador úbicado al inicio de una lista dada */
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if(iter==NULL)return NULL;
	iter->anterior = NULL;
	iter->actual = lista->prim;
	iter->lista = lista;
	return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){
/*Itera al siguiente elemento de la lista*/
	if(lista_iter_al_final(iter))return false;
	iter->anterior=iter->actual;
	iter->actual=iter->actual->prox;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
/* Permite ver el elemento en el que se encuentra el iterador.*/
	if(lista_iter_al_final(iter)) return NULL;
	return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
/*Permite saber si el iterador ha llegado al final de la lista.*/
	return(iter->actual==NULL);
}

void lista_iter_destruir(lista_iter_t *iter){
/*Destruye el iterador.*/
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
/*Inserta un elemento en la posición actual del iterador.*/
	nodo_t* nuevo = nodo_crear(dato);
	if(nuevo==NULL)return false;
	if(iter->anterior==NULL){
		nuevo->prox = iter->lista->prim;
		iter->lista->prim = nuevo;
		if(nuevo->prox==NULL) iter->lista->ult = nuevo;
	}else{
		iter->anterior->prox = nuevo;
		nuevo->prox = iter->actual;
		if(iter->actual == NULL) iter->lista->ult = nuevo;
	}
	iter->actual = nuevo;
	iter->lista->largo+=1;
	return true;
}
	

void *lista_iter_borrar(lista_iter_t *iter){
/*Borra el elemento en la posición actual del iterador.*/
	if(lista_esta_vacia(iter->lista)||lista_iter_al_final(iter)){
		return NULL;
	}
	void* dato = iter->actual->dato;
	nodo_t* aux = iter->actual;
	iter->actual = iter->actual->prox;
	if(iter->anterior==NULL){
		iter->lista->prim = iter->actual;
	}else{
		iter->anterior->prox = iter->actual;
	}
	if(iter->actual==NULL){
		iter->lista->ult = iter->anterior;
	}
	free(aux);
	iter->lista->largo-=1;
	return dato;
}
	
