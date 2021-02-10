#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

typedef struct coordenada {
	int fil;
	int col;
} coordenada_t;

typedef struct nivel {
	coordenada_t entrada;
	coordenada_t salida;

	coordenada_t paredes[250];
	int tope_paredes;

    /*
	elemento_t obstaculos[MAX_OBSTACULOS];
	int tope_obstaculos;

	elemento_t herramientas[MAX_HERRAMIENTAS];
	int tope_herramientas;
    */

} nivel_t;


/* Verifica si la coordenada es una baldosa libre */
bool baldosa_libre(int dimension,char mapa[][12],int fila, int col){ 
    return mapa[fila][col] == '.';
}

/* Obtiene una posicion aleatoria y verifica que la misma sea una baldosa libre*/
void obtener_posicion_aleatoria(coordenada_t *posicion, int dimension, char mapa[][12]){
    
    int fil;
    int col;

    srand ((unsigned)time(NULL));
    fil = rand()%dimension;
    col = rand()%dimension;
    while(!baldosa_libre(dimension,mapa,fil,col)){
        fil = rand()%dimension;
        col = rand()%dimension;
    }
   
    posicion->col = col;
    posicion->fil = fil;
}


void inicializar_nivel(nivel_t* nivel, int numero_nivel, int cantidad_baldosas_pinches, int cantidad_guardia, bool hay_bomba){

    char mapa[12][12];

    /*cargo default todo null*/
    for(int i=0; i<12; i++){
        for(int j=0; j<12;j++){
            mapa[i][j] = '.';
        }
    }

    /*cargo paredes */
    for(int i=0; i<12; i++){
        mapa[i][0] = '#';
        mapa[0][i] = '#';
        mapa[11][i] = '#';
        mapa[i][11] = '#';
    }

    obtener_posicion_aleatoria( &nivel->entrada, 12, mapa);
    mapa[nivel->entrada.fil][nivel->entrada.col]= 'E';

    obtener_posicion_aleatoria( &nivel->salida, 12, mapa);
    mapa[nivel->salida.fil][nivel->salida.col]= 'S';

    /*mostrar*/
    for(int i=0; i<12; i++){
        for(int j=0; j<12;j++){
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
}

void test(char mapa[][12]){

    nivel_t *nivel = malloc(sizeof(nivel_t));

    inicializar_nivel(nivel, 1, 1,1 ,true);


    coordenada_t *pos = malloc(sizeof(coordenada_t));

    for(int i=0; i<12; i++){
        for(int j=0; j<12;j++){
            mapa[i][j] = '.';
        }
    }

    for(int i=0; i<12; i++){
        mapa[i][0] = '#';
        mapa[0][i] = '#';
        mapa[11][i] = '#';
        mapa[i][11] = '#';
    }
    

    mapa[pos->fil][pos->col] = '#';

    for(int i=0; i<12; i++){
        for(int j=0; j<12;j++){
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }


}


int main(){

    char m[12][12];

    test(m);

    return 0;
}
