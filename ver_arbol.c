#include "ver_arbol.h"


bool visitar(const char* clave, void* dato, void* extra){
	printf("%d.clave: %s\n",*(int*)extra,clave);
	*(int*)extra+=1;
	return true;
}

bool ver_arbol(abb_t* abb){
	int* i = malloc(sizeof(int));
	*i = 0;
	abb_in_order(abb,visitar,i);
	return true;
}
