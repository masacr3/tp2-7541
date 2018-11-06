#define _XOPEN_SOURCE
#include <time.h>
#include "algueiza.h"

/*Estructura vuelo que almacena todos los datos de un vuelo dado*/
struct vuelo{
	char* FLIGHT_NUMBER;
	char* AIRLINE;
	char* ORIGIN_AIRPORT;
	char* DESTINATION_AIRPOT;
	char* TAIL_NUMBER;
	char* PRIORITY;
	char* DATE;
	char* DEPARTURE_DELAY;
	char* AIR_TIME;
	char* CANCELLED;
};


enum dato{
	flight_number,
	airline,
	origin_airport,
	destination_airport,
	tail_number,
	priority,
	date,
	departure_delay,
	air_time,
	cancelled,
};

vuelo_t* vuelo_crear(char *linea){
	if ( !linea ) return NULL;

	vuelo_t *nuevo_vuelo = malloc(sizeof(vuelo_t));

	if ( !nuevo_vuelo ) return NULL;

	char **datos = split(linea,',');

	if ( !datos ) return NULL;

	nuevo_vuelo->FLIGHT_NUMBER = datos[flight_number];
	nuevo_vuelo->AIRLINE = datos[airline];
	nuevo_vuelo->ORIGIN_AIRPORT = datos[origin_airport];
	nuevo_vuelo->TAIL_NUMBER = datos[tail_number];
	nuevo_vuelo->PRIORITY = datos[priority];
	nuevo_vuelo->DATE = datos[date];
	nuevo_vuelo->DEPARTURE_DELAY = datos[departure_delay];
	nuevo_vuelo->AIR_TIME = datos[air_time];
	nuevo_vuelo->CANCELLED = datos[cancelled];

	free(datos[priority]);
	free(datos[date]);
	free(datos);

	return nuevo_vuelo;

}

void vuelo_destruir(vuelo_t *vuelo){
	free(vuelo->FLIGHT_NUMBER);
	free(vuelo->AIRLINE);
	free(vuelo->ORIGIN_AIRPORT);
	free(vuelo->TAIL_NUMBER);
	free(vuelo->DEPARTURE_DELAY);
	free(vuelo->AIR_TIME);
	free(vuelo->CANCELLED);
	free(vuelo);
}
