#include <stdio.h>
#include <stdlib.h>

/*Estructura vuelo que almacena todos los datos de un vuelo dado*/
typedef struct vuelo{
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
} vuelo_t;

typedef struct tablero{
	char* FLIGHT_NUMBER;
	char* DATE;
} tablero_t;


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

tablero_t* tablero_crear(char* flight_number, char* date){
	tablero_t *tablero = malloc(sizeof(tablero_t));

	if ( !tablero) return NULL;

	tablero->FLIGHT_NUMBER = malloc(sizeof(char) * (strlen(flight_number) + 1));
	tablero->DATE = malloc(sizeof(char) * (strlen(date) + 1) );

	strcpy(tablero->FLIGHT_NUMBER, flight_number);
	strcpy(tablero->DATE,date);

	return tablero;
}

void tablero_destruir(tablero_t *tablero){
	free(tablero->FLIGHT_NUMBER);
	free(tablero->DATE);
	free(tablero);
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
