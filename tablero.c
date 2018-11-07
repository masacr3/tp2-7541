#include "tablero.h"

bool ver_tablero(abb_t *abb,const char *modo, char *desde, const char *hasta){
  abb_iter_t *iter = abb_iter_in_crear(abb,modo,desde,hasta);

  if (!iter) return false;

  while ( !abb_iter_in_al_final(iter)){
    tablero_t * tablero = abb_iter_in_ver_actual(iter);
    fprintf(stdout, "%s - %s\n", tablero->fecha, tablero->codigo_vuelo );
    abb_iter_in_avanzar(abb,iter);
  }
  return true;
}
