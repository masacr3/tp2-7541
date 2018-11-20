#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#include "abb.h"
#include "pila.h"

#define RAMA_DERECHA 1
#define RAMA_IZQUIERDA -1

typedef struct nodo abb_nodo_t;

struct nodo {
    char* clave;
    void* dato;
    abb_nodo_t* izquierda;
    abb_nodo_t* derecha;
};

abb_nodo_t* nodo_abb_crear(const char* clave, void* dato ){
  abb_nodo_t* nodo = malloc( sizeof(abb_nodo_t) );

  if (!nodo) return NULL;

  char* clave_copia = malloc(sizeof(char) * (strlen(clave) + 1) );

  if (!clave_copia){
    free(nodo);
    return NULL;
  }

  strcpy(clave_copia,clave);

  nodo->dato = dato;
  nodo->clave = clave_copia;
  nodo->izquierda = NULL;
  nodo->derecha = NULL;

  return nodo;
}

void nodo_abb_destruir(abb_nodo_t* nodo,abb_destruir_dato_t destruir_dato){
  if (destruir_dato) destruir_dato(nodo->dato);
  free(nodo->clave);
  free(nodo);
}

struct abb {
    abb_comparar_clave_t comparar_clave;
    abb_destruir_dato_t destruir_dato;

    abb_nodo_t* raiz;
    size_t cantidad;
};

struct abb_iter{
  pila_t* pila;
  const char* inicio; //add
  const char* fin; //add
  int modo;
};

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
  abb_t* abb = malloc (sizeof(abb_t));

  if (!abb) return NULL;

  abb->comparar_clave = cmp;
  abb->destruir_dato = destruir_dato;

  abb->cantidad = 0;
  abb->raiz = NULL;

  return abb;
}

abb_nodo_t* abb_nodo_buscar(const abb_t *arbol, abb_nodo_t* raiz ,const char *clave){

  if (!raiz) return false;

  int rama = arbol->comparar_clave(clave, raiz->clave);

  if(rama == 0) return raiz;

  if(rama > 0) return abb_nodo_buscar(arbol, raiz->derecha,clave);

  return abb_nodo_buscar(arbol, raiz->izquierda,clave);
}

abb_nodo_t* abb_padre_buscar(const abb_t* arbol,abb_nodo_t* padre,const char* clave){
  if (!padre) return NULL;

  int rama = arbol->comparar_clave(clave,padre->clave);

  if( rama > 0 && padre->derecha){
      int es_el_padre = arbol->comparar_clave(clave, padre->derecha->clave) == 0;

      return es_el_padre ? padre : abb_padre_buscar(arbol, padre->derecha, clave);
  }

  if( rama < 0 && padre->izquierda){
      int es_el_padre = arbol->comparar_clave(clave, padre->izquierda->clave) == 0;

      return es_el_padre ? padre : abb_padre_buscar(arbol, padre->izquierda,clave);
  }

  return NULL;

}

void* buscar_padre(abb_t* arbol, const char* clave){
  abb_nodo_t* nodo = abb_padre_buscar(arbol, arbol->raiz, clave);

  return !nodo ? NULL : nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
  abb_nodo_t* nodo = abb_nodo_buscar(arbol, arbol->raiz,clave);

  return !nodo ? false : true;
}

void* abb_obtener(const abb_t* arbol,const char* clave){
  abb_nodo_t* nodo = abb_nodo_buscar(arbol, arbol->raiz,clave);

  return !nodo ? NULL : nodo->dato;
}

size_t abb_cantidad(abb_t* arbol){
  return arbol->cantidad;
}


bool insertar_nodo (abb_t* arbol,abb_nodo_t* raiz, const char* clave, void* dato){
  int rama = arbol->comparar_clave(clave,raiz->clave);

  if(rama == 0){

    if (arbol->destruir_dato) arbol->destruir_dato(raiz->dato);

    raiz->dato = dato;
  }

  if(rama < 0 ){

    if(raiz->izquierda) return insertar_nodo(arbol,raiz->izquierda,clave,dato);

    abb_nodo_t* nodo = nodo_abb_crear(clave,dato);

    if(!nodo) return false;

    raiz->izquierda = nodo;
    arbol->cantidad++;
  }

  if (rama > 0 ){

    if(raiz->derecha) return insertar_nodo(arbol,raiz->derecha,clave,dato);

    abb_nodo_t* nodo = nodo_abb_crear(clave,dato);

    if(!nodo) return false;

    raiz->derecha = nodo;
    arbol->cantidad++;
  }
  return true;
}

bool abb_guardar(abb_t* arbol, const char* clave, void* dato){

  if (!arbol->raiz){
    abb_nodo_t* nodo=nodo_abb_crear(clave,dato);

    if (!nodo) return false;

    arbol->cantidad++;
    arbol->raiz=nodo;
    return true;
  }

  return insertar_nodo(arbol, arbol->raiz, clave, dato);
}

void* abb_borrar_nodo_hoja(abb_t* arbol,abb_nodo_t* nodo,abb_nodo_t* padre,const char* clave){
  arbol->cantidad--;
  void* dato=nodo->dato;

  if ( padre ){
    if (padre->izquierda && arbol->comparar_clave(clave,padre->izquierda->clave)==0) padre->izquierda=NULL;

    else padre->derecha=NULL;

  }

  else arbol->raiz = NULL;

  nodo_abb_destruir(nodo,NULL);
  return dato;
}

void* abb_borrar_nodo_un_hijo(abb_t* arbol,abb_nodo_t* nodo,abb_nodo_t* padre,const char* clave){
  arbol->cantidad--;
  void* dato=nodo->dato;

  if ( padre ){

    if ( padre->derecha && padre->derecha == nodo ) padre->derecha = nodo->derecha ? nodo->derecha : nodo->izquierda;

    else padre->izquierda = nodo->izquierda ? nodo->izquierda : nodo->derecha;

  }

  else arbol->raiz = nodo->derecha ? nodo->derecha : nodo->izquierda;

  nodo_abb_destruir(nodo,NULL);
  return dato;
}

abb_nodo_t* abb_nodo_profundo_buscar(abb_nodo_t* nodo){

  if (!nodo) return NULL;

  if (!nodo->izquierda) return nodo;

  return abb_nodo_profundo_buscar(nodo->izquierda);
}

void* abb_borrar_nodo_dos_hijos(abb_t* arbol,abb_nodo_t* nodo,abb_nodo_t* padre){
  void* dato=nodo->dato;
  arbol->cantidad--;

  if (!nodo->derecha->izquierda){

    nodo->derecha->izquierda=nodo->izquierda;

    if (padre){

      if (padre->derecha && arbol->comparar_clave(nodo->clave, padre->derecha->clave)==0) padre->derecha=nodo->derecha;

      else padre->izquierda=nodo->derecha;

    }

    else arbol->raiz=nodo->derecha;

  }

  else{

    abb_nodo_t* reemplazante=abb_nodo_profundo_buscar(nodo->derecha->izquierda);
    abb_nodo_t* padre_reemplazante=abb_padre_buscar(arbol,nodo->derecha,reemplazante->clave );

    if (reemplazante->derecha) padre_reemplazante->izquierda=reemplazante->derecha;

    else padre_reemplazante->izquierda=NULL;

    reemplazante->derecha=nodo->derecha;
    reemplazante->izquierda=nodo->izquierda;

    if (padre){

      if (padre->derecha && arbol->comparar_clave(nodo->clave,padre->derecha->clave)==0) padre->derecha=reemplazante;

      else padre->izquierda=reemplazante;

    }

    else arbol->raiz=reemplazante;

  }

  nodo_abb_destruir(nodo,NULL);
  return dato;
}

void* abb_borrar(abb_t* arbol, const char* clave){

  if (arbol->cantidad==0) return NULL;

  abb_nodo_t* nodo=abb_nodo_buscar(arbol,arbol->raiz,clave);

  if (!nodo) return NULL;

  abb_nodo_t* padre=abb_padre_buscar(arbol,arbol->raiz,clave);

  if (!nodo->izquierda && !nodo->derecha) return abb_borrar_nodo_hoja(arbol,nodo,padre,clave);

  if (!nodo->izquierda || !nodo->derecha) return abb_borrar_nodo_un_hijo(arbol,nodo,padre,clave);

  return abb_borrar_nodo_dos_hijos(arbol,nodo,padre);
}

void abb_destruir_nodos(abb_nodo_t* nodo,abb_destruir_dato_t destruir_dato){

  if (!nodo) return;

  abb_destruir_nodos(nodo->izquierda,destruir_dato);
  abb_destruir_nodos(nodo->derecha,destruir_dato);
  nodo_abb_destruir(nodo,destruir_dato);
}

void abb_destruir(abb_t* arbol){
  abb_destruir_nodos(arbol->raiz,arbol->destruir_dato);
  free(arbol);
}

/*******************************************************************************
 *                           ITERADOR INTERNO
 ******************************************************************************/
bool abb_iter_in_order(abb_nodo_t* nodo,bool visitar(const char* clave, void* dato, void* extra),void* extra){

  if (!nodo) return true;

  bool continuar=abb_iter_in_order(nodo->izquierda,visitar,extra);

  if (!continuar) return false;

  bool seguir=visitar(nodo->clave,nodo->dato,extra);

  if (!seguir) return false;

  abb_iter_in_order(nodo->derecha,visitar,extra);
  return true;
}

void abb_in_order(abb_t* arbol, bool visitar(const char* clave, void* dato, void* extra), void* extra){
  abb_iter_in_order(arbol->raiz,visitar,extra);
}

/*******************************************************************************
 *                         ITERADOR EXTERNO
 ******************************************************************************/
/*Funcion de apilado especial para que tenga contemplado el caso asc y desc a la vez @Leonel.R */
void apilar(const abb_t* arbol, abb_iter_t* it, pila_t* pila,abb_nodo_t* nodo){
  if (!nodo) return;

  int rango_inicio = arbol->comparar_clave(nodo->clave,it->inicio) >= 0;
  int rango_fin = arbol->comparar_clave(nodo->clave,it->fin) <= 0;

  bool esta_en_rango = rango_inicio && rango_fin;

  if (esta_en_rango){
    pila_apilar(pila,nodo);
    apilar(arbol,it,pila, it->modo ? nodo->izquierda : nodo->derecha);
    return;
  }

  if (it->modo ? rango_inicio : rango_fin){
    apilar(arbol, it, pila, it->modo ? nodo->izquierda : nodo->derecha);
    return;
  }

  else{
    apilar(arbol, it, pila, it->modo ? nodo->derecha : nodo->izquierda);
    return;
  }
}

//modify add modo ,add inicio, fin limits for iteration
abb_iter_t* abb_iter_in_crear(const abb_t* arbol, const char *modo, const char* inicio, const char* fin){
  abb_iter_t* iter=malloc(sizeof(abb_iter_t));

  if (!iter) return NULL;

  iter->pila=pila_crear();
  iter->inicio = inicio;
  iter->fin = fin;
  iter->modo = strcmp(modo,"asc") == 0 ? 1 : 0;
  apilar(arbol,iter,iter->pila,arbol->raiz);
  return iter;
}

bool abb_iter_in_avanzar(abb_t* arbol,abb_iter_t* iter){

  if (pila_esta_vacia(iter->pila)) return false;

  abb_nodo_t* nodo=pila_desapilar(iter->pila);

  apilar(arbol,iter,iter->pila,iter->modo ? nodo->derecha : nodo->izquierda);

  return true;
}

const char* abb_iter_in_ver_actual(const abb_iter_t* iter){

  if (pila_esta_vacia(iter->pila)) return NULL;

  abb_nodo_t* nodo_actual=pila_ver_tope(iter->pila);
  return nodo_actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t* iter){
  return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
  pila_destruir(iter->pila);
  free(iter);
}
