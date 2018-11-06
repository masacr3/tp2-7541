#include "info_vuelos_.h"

bool info_vuelos(hash_t *hash, char *codigo_vuelo){
  if ( !hash || !hash_pertenece(hash, codigo_vuelo)) return false;

  vuelo_t *vuelo = hash_obtener(codigo_vuelo);
  mostar_vuelo(vuelo);

  return true;
}

void mostar_vuelo(vuelo_t *vuelo){
  fprintf(stdout,"%s ",vuelo->FLIGHT_NUMBER);
	fprintf(stdout,"%s ",vuelo->AIRLINE);
	fprintf(stdout,"%s ",vuelo->ORIGIN_AIRPORT);
	fprintf(stdout,"%s ",vuelo->TAIL_NUMBER);
	fprintf(stdout,"%s ",vuelo->DEPARTURE_DELAY);
	fprintf(stdout,"%s ",vuelo->AIR_TIME);
	fprintf(stdout,"%s\n",vuelo->CANCELLED);
}
