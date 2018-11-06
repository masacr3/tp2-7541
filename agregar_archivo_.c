#include "agregar_archivo_.h"

bool agregar_archivo(hash_t *hash, abb_t* abb, char *archivo){
  FILE *f;

  if ( (f = fopen(archivo,"r")) == NULL) return false;

  char *linea = NULL;
  size_t cap = 0;

  while ( getline(&linea, &cap, f) != EOF){
    vuelo_t *vuelo = vuelo_crear(linea);
    tablero_t *tablero = tablero_crear(vuelo->FLIGHT_NUMBER, vuelo->DATE);
    hash_guardar(hash, vuelo->FLIGHT_NUMBER, vuelo);
    abb_guardar(abb,tablero->DATE, tablero);
  }
  free(linea);
  fclose(f);
  return true;
}
