#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "datos.c"
enum dato{
	flight_number;
	airline;
	origin_airport;
	destination_airport;
	tail_number;
	priority;
	date;
	departure_delay;
	air_time;
	cancelled;
}
vuelo_t* vuelo_crear(char **linea){
	if ( !linea ) return NULL;

	vuelo_t *nuevo_vuelo = malloc(sizeof(vuelo_t));

	if ( !nuevo_vuelo ) return NULL;

	char **datos = split(linea,",");

	if ( !datos ) return NULL;

	nuevo_vuelo->FLIGHT_NUMBER = datos[fligh_number];
	nuevo_vuelo->AIRLINE = datos[airline];
	nuevo_vuelo->ORIGIN_AIRPORT = datos[origin_airport];
	nuevo_vuelo->TAIL_NUMBER = datos[tail_number];
	nuevo_vuelo->PRIORITY = atoi(datos[priority]);
	nuevo_vuelo->DATE = iso8601_to_time(datos[date]);
	nuevo_vuelo->DEPARTURE_DELAY = datos[departure_delay];
	nuevo_vuelo->AIR_TIME = datos[air_delay];
	nuevo_vuelo->CANCELLED = datos[cancelled];
	
	free(datos[priority]);
	free(datos[date]);
	free(datos);

	return nuevo_vuelo;
				
}

void vuelo_destruir(vuelo_t *vuelo){
	free(nuevo_vuelo->FLIGHT_NUMBER = datos[fligh_number]);
	free(nuevo_vuelo->AIRLINE = datos[airline]);
	free(nuevo_vuelo->ORIGIN_AIRPORT = datos[origin_airport]);
	free(nuevo_vuelo->TAIL_NUMBER = datos[tail_number]);
	free(nuevo_vuelo->PRIORITY = atoi(datos[priority]));
	free(nuevo_vuelo->DATE = iso8601_to_time(datos[date]));
	free(nuevo_vuelo->DEPARTURE_DELAY = datos[departure_delay]);
	free(nuevo_vuelo->AIR_TIME = datos[air_delay]);
	free(nuevo_vuelo->CANCELLED = datos[cancelled]);
	free(vuelo);	
}
