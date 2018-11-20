#include "pila.h"
#include <stdlib.h>

#define PILA_TAMANIO 30
#define PILA_VACIA 0
#define TAMANIO_REDIMENSION 2

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

pila_t* pila_crear(void){
  pila_t* pila=malloc(sizeof(pila_t));
  if(pila==NULL){
    return NULL;
  }
  pila->datos=malloc(sizeof(void*)*PILA_TAMANIO);
  if(pila->datos==NULL){
    free(pila);
    return NULL;
  }
  pila->capacidad=PILA_TAMANIO;
  pila->cantidad=PILA_VACIA;
  return pila;
}

void pila_destruir(pila_t* pila){
  free(pila->datos);
  free(pila);
}

bool pila_esta_vacia(const pila_t* pila){
  return pila->cantidad==0;
}

void* pila_ver_tope(const pila_t* pila){
  if (pila_esta_vacia(pila)){
    return NULL;
  }
  return pila->datos[pila->cantidad-1];
}

// Cambia el tamaño de la pila.
// Pre: la pila fue creada.
// Post: la pila cambió de tamaño a tamanio_nuevo y devuelve true o si la pila
// no cambió de tamaño a tamanio_nuevo devuelve false.
bool redimensionar_pila(pila_t* pila,size_t tamanio_nuevo){
  void** datos_nuevos=realloc(pila->datos,tamanio_nuevo*sizeof(void*));
  if (datos_nuevos==NULL){
    return false;
  }
  pila->datos=datos_nuevos;
  pila->capacidad=tamanio_nuevo;
  return true;
}

bool pila_apilar(pila_t* pila,void* valor){
  bool pila_llena=pila->capacidad==pila->cantidad;
  if (pila_llena){
    size_t tamanio_nuevo=pila->capacidad*TAMANIO_REDIMENSION;
    bool esta_redimensionada_pila=redimensionar_pila(pila,tamanio_nuevo);
    if (!esta_redimensionada_pila){
      return false;
    }
  }
  pila->datos[pila->cantidad]=valor;
  pila->cantidad++;
  return true;
}

void* pila_desapilar(pila_t* pila){
  if (pila_esta_vacia(pila)){
    return NULL;
  }
  void* dato_desapilado=pila->datos[pila->cantidad-1];
  pila->cantidad--;
  size_t un_cuarto_pila=pila->capacidad/4;
  size_t tamanio_nuevo=pila->capacidad/TAMANIO_REDIMENSION;
  bool hay_que_redimensionar_pila=un_cuarto_pila==pila->cantidad && tamanio_nuevo>=PILA_TAMANIO;
  if (hay_que_redimensionar_pila){
    redimensionar_pila(pila,tamanio_nuevo);
  }
  return dato_desapilado;
}
