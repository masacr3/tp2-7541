#define _POSIX_C_SOURCE 200809L
//para tiempo
#define _GNU_SOURCE
#define _XOPEN_SOURCE
#define TIME_FORMAT "%FT%T%z"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "strutil.h"
#include "hash.h"
#include "lista.h"
#include "abb.h"
#include "pila.h"
#include "heap.h"

#define FLIGHT_NUMBER 0
#define DATE 6
#define PRIORITY 5

int lenstrv(char **);

/*Función auxiliar del sistema utilizado por main, ejecuta el comando correspondiente.
 *Recibe una cadena con el comando y sus parametros, una referencia al HASH y al ABB del sistema
 *Pre: El abb y el hash han sido inicializados.
 *Post: Devuelve true si el comando ha sido ejecutado con éxito, false en caso contrario.*/
bool ejecutar_operacion(char **comandos,hash_t* hash,abb_t* abb);

/*Función de comparación para crear un heap de mínimos. a y b inicializados.
 *Recibe dos vectores dinámicos de cadenas correspondientes a los datos de un vuelo.
 *Devuelve -1 si a>b,
 *	    1 si a<b,
 *          0 si a=b*/
int heap_min_cmp(const void* a,const void* b);

/*Función auxiliar para crear clave de datos en ABB del sistema.
 *Recibe un vector dinámicos de cadenas con todos los datos de un vuelo.
 *Devuelve una cadena en formato 'YYYY-MM-DDTHH:MM:SS,<Código de vuelo>'*/
char * crear_clave_abb(char **datos);

/*Función de comparación para crear un ABB. a y b inicializados.
 *Recibe dos cadenas a comparar en formato 'YYYY-MM-DDTHH:MM:SS,<Código de vuelo>'.
 *Devuelve -1 si a<b,
 *	    1 si a>b,
 *	    0 si a=b
 *Si los tiempos son iguales, compara por código de vuelo tomado como cadena.*/
int abb_cmp(const char *a, const char *b);

/*Función auxiliar de comparación para funcionamiento de iterador en un rango de tiempo.
 *Suponemos a y b inicializados.
 *Recibe dos cadenas a comparar, donde el primer "elemento" está en formato 'YYYY-MM-DDTHH:MM:SS'
 *Compara este dato y devuele -1 si a<b,
 *			       1 si a>b,
 *			       0 si a=b*/
int abb_it_cmp(const char *a, const char *b);

/*Función auxiliar, conversión de función free_strv a hash_destruir_dato_t*/
void destruir_dato_wrapper(void* dato);

/*Función auxiliar, conversión de cadena a time_t
 *Recibe una cadena en formato 'YYYY-MM-DDTHH:MM:SS', devuelve el dato de time_t*/
time_t iso8601_to_time(const char* iso8601);


/*FUNCIONES DE COMANDOS PRINCIPALES*/

/*Lee el archivo .csv indicado en la línea de comandos y almacena los datos en el sistema.
 *Recibe una cadena con la línea de comando con el nombre del archivo como parametro,
 *y las estructuras auxiliares del sistema (HASH y ABB).
 *Devuelve true si la operación se ejecutó con éxito, false en caso de error.
 *Los datos en el archivo han sido cargados/actualizados en el sistema.
*/
bool agregar_archivo(char **comandos, hash_t* hash,abb_t* abb);

/*Muestra toda la informacion del vuelo cuyo código de vuelo coincida con el que fue
 *pasado por parametro en la linea de comando.
 *Recibe una cadena con la línea de comando y la estructura auxiliar HASH del sistema.
 *Devuelve true si la operación se ejecutó con éxito, false en caso de error.
 *Imprime por salida estandar los datos del vuelo.
*/
bool info_vuelo(char** comandos, hash_t* hash);

/*Muestra en orden descendente los K vuelos con mayor prioridad que hayan sido cargados en el sistema.
 *Recibe una cadena con la línea de comando con K como parametro y la estructura auxiliar HASH del sistema.
 *Devuelve true si la operación se ejecutó con éxito, false en caso de error.
 *Imprime por salida estandar los K vuelos correspondientes en formato: <prioridad> - <código de vuelo>.
*/
bool prioridad_vuelos(char **comandos,hash_t* hash);

/*Muestra los K vuelos cuya fecha de despegue sea igual o mayor a < desde > e igual o menor a < hasta >
 *Recibe la estructura auxiliar ABB del sistema y una cadena con la línea de comando y los parametros:
 *	K: cantidad vuelos a mostrar.
 *	modo: asc/desc. Indica el orden en el que se mostraran los datos (ascendente o descendente).
 *	desde: YYYY-MM-DDTHH:MM:SS que indica el limite inferior del rango.
 *	hasta: YYYY-MM-DDTHH:MM:SS que indica el limite superior del rango.
 *Devuelve true si la operación se ejecutó con éxito, false en caso de error. Imprime por salida estandar los K vuelos correspondientes
 *en formato: YYYY-MM-DDTHH:MM:SS - <código de vuelo>.
*/
bool ver_tablero(char** comandos,abb_t* abb);

/*Muestra y elimina todos los vuelos cuya fecha de despegue sea igual o mayor a < desde > e igual o menor a < hasta >
 *Recibe ambas estructuras auxiliares del sistema (ABB y HASH) y una cadena con la línea de comandos y los parametros:
 *	desde: YYYY-MM-DDTHH:MM:SS que indica el limite inferior del rango.
 *	hasta: YYYY-MM-DDTHH:MM:SS que indica el limite superior del rango.
 *Devuelve true si la operación se ejecutó con éxito, false en caso de error.
 *Imprime por salida estandar la información de los vuelos eliminados.
*/
bool borrar(char **comandos,abb_t* abb,hash_t* hash);
