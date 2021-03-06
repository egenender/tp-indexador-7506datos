#ifndef MERGER_H_
#define MERGER_H_
#define MERGER_OK 0
#define MERGER_ERROR 1

/* Recibe un vector con rutas a los archivos, y la cantidad de rutas.
 * Hace la unión de todos estos archivos, a partir de tener todos estos ya ordenados
 * (cada uno por separado).
 * Memoria: none
 * Post: Si salio bien, devuelve MERGER_OK, sino MERGER_ERROR. (Despues se ve si es
 * conveniente tener mas constantes).*/
int merger_MergearArchivos(char** rutas, int cant, const char* salida_final);


#endif /* MERGER_H_ */
