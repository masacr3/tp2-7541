char * enpaquetar(char **datos){
  if(!datos) return;

  char **paquete = malloc ( sizeof(char*) * 3);

  if ( !paquete) return;

  paquete[0] = datos[6]; //date
  paquete[1] = datos[0]; //flight_number
  paquete[2] = NULL;


  return join(paquete,',');
}

//suponemos que a , b tienen datos
int cmp_abb(const char *a, const char *b){
  char **tiempo1 = split(a,',');
  char **tiempo2 = split(b,',');

  time_t t1 = iso8601_to_time(a[0]); //tiempos
  time_t t2 = iso8601_to_time(b[0]); //tiempos

  if (diff_time(t2,t1) > 0){
    free_strv(tiempo1);
    free_strv(tiempo2);
    return 1;
  }
  if (diff_time(t2,t1) < 0){
    free_strv(tiempo1);
    free_strv(tiempo2);
    return -1;
  }

  if (strcmp(tiempo1[1],tiempo2[1]) < 0){
    free_strv(tiempo1);
    free_strv(tiempo2);
    return 1;
  }
  if (strcmp(tiempo1[1],tiempo2[1]) > 0 ){
    free_strv(tiempo1);
    free_strv(tiempo2);
    return -1;
  }

  free_strv(tiempo1);
  free_strv(tiempo2);

  return 0;
}
