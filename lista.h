/* ******************************************************************
 * LISTA - lista.h			                                        *
 * AYUDANTE: MILENA MARCHESE										*
 * PADRÓN: 101741 													*
 * NOMBRE: GABRIELA ELIZABETH MÉNDEZ GUTIÉRREZ						*
 * ******************************************************************/
#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* TDA LISTA 
Implementada como una lista de punteros genéricos */

typedef struct lista lista_t;

lista_t* lista_crear(void);
/* Crea una lista.
   Post: devuelve una nueva lista vacía o NULL si falló la operación*/

bool lista_esta_vacia(const lista_t *lista);
/*Pre: la lista fue creada.
  Post: Devuelve verdadero o falso, según si la lista tiene o no elementos*/

bool lista_insertar_primero(lista_t *lista, void *dato);
/*Agrega un nuevo elemento a la lista.
  Pre: La lista fue creada.
  Post: Se agrego un nuevo elemento a la lista, valor se encuentra al principio de la lista.
  Devuelve falso en caso de error, verdadero si es exitoso.
*/ 

bool lista_insertar_ultimo(lista_t *lista, void *dato);
/*Agrega un nuevo elemento a la lista.
  Pre: La lista fue creada.
  Post: Se agrego un nuevo elemento a la lista, valor se encuentra al final de la lista.
  Devuelve falso en caso de error, verdadero si es exitoso.
*/ 

void *lista_borrar_primero(lista_t *lista);
/*Elimina un elemento de la lista. 
  Pre: La lista fue creada. 
  Post: El primer elemento de la lista fue removido. Se devuelve el valor de este.
  NULL si la lista estaba vacía.
*/

void *lista_ver_primero(const lista_t *lista);
/*Obtiene el valor del primer elemento de la lista.
  Pre: La lista fue creada.
  Post: Devuelve el valor del primer elemento de la lista, NULL si está vacia.
*/

void *lista_ver_ultimo(const lista_t* lista);
/*Obtiene el valor del último elemento de la lista.
  Pre: La lista fue creada.
  Post: Devuelve el valor del último elemento de la lista, NULL si está vacia.
*/

size_t lista_largo(const lista_t *lista);
/*Obtiene el largo de la lista
  Pre: La lista fue creada.
  Post: Devuelve el número de elementos en la lista
*/

void lista_destruir(lista_t *lista, void destruir_dato(void *));
/*Destruye la lista. Si se recibe la función destruir_dato por parámetro,
  para cada uno de los elementos de la lista llama a destruir_dato.
  Pre: La lista fue creada. Destruir_dato es una función capaz de destruir
  los datos de la lista, o NULL en caso de que no se la utilice.
  Post: se eliminaron todos los elementos de la lista.
*/

/* ITERADOR INTERNO */

/*
Procesa los elementos de la lista. La función visitar devuelve un valor booleano
y determina que se hará con cada dato, si en algún momento la función visitar
devuelve false, se interrumpe la iteración. El valor extra, puede ser usado para
mantener una memoria entre las distintas llamadas de visitar.
Pre: La lista fue creada
Post: Se recorre la lista de acuerdo a lo especificado en la función visitar.
*/
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ITERADOR EXTERNO */

typedef struct lista_iter lista_iter_t;

lista_iter_t *lista_iter_crear(lista_t *lista);
/*Permite recorrer los elementos almacenados en una lista.
  Pre: La lista fue creada.
  Post: Devuelve un iterador para la lista ubicado en el primer elem de esta.
*/

bool lista_iter_avanzar(lista_iter_t *iter);
/*Itera al siguiente elemento de la lista
  Pre: el iterador fue creado.
  Post: El iterador se ubica en el siguiente elemento d la lista.
  Devuelve false, s se ha llegado al final de la lista y no puede avanzar.
*/

void *lista_iter_ver_actual(const lista_iter_t *iter);
/* Permite ver el elemento en el que se encuentra el iterador.
   Pre: El iterador fue creado.
   Post: Devuelve el elemento actual del iterador.
*/

bool lista_iter_al_final(const lista_iter_t *iter);
/*Permite saber si el iterador ha llegado al final de la lista.
  Pre: el iterador fue creado.
  Post: devuelve true si el iterador está al final de la lista,
  false en caso contrario.
*/

void lista_iter_destruir(lista_iter_t *iter);
/*Destruye el iterador.
  Pre: el iterador fue creado.
  Post: se ha eliminado el iterador.*/
  
bool lista_iter_insertar(lista_iter_t *iter, void *dato);
/*Inserta un elemento en la posición actual del iterador.
  Pre: El iterador fue creado.
  Post: Se ha insertado un elemento a la lista en la posición actual.
*/

void *lista_iter_borrar(lista_iter_t *iter);
/*Borra el elemento en la posición actual del iterador.
  Pre: el iterador fue creado.
  Post: Se ha eliminado el elemento de la lista en la posición actual.
*/

/* PRUEBAS ALUMNO*/

void pruebas_lista_alumno(void);

#endif
