#ifndef TERMINO_H_
#define TERMINO_H_
#include <stdio.h>
#include "../../Carpetas Compartidas/Manejo de Archivos/debuffer.h"

// Definicion de la estructura de termino.
typedef struct termino termino_t;


/* Permite crear un termino a partir de un string, un offset (al archivo de indice)
 * y la frecuencia.
 * Memoria: pide memoria del heap para almacenar el termino.*/
termino_t* termino_crear (const char* term, size_t offset, size_t frec);

/* Libera la memoria utilizada por el termino en cuestion.*/
void termino_destruir(termino_t*);

/* Permite obtener un término a partir de un archivo de front coding, y otro con
 * los caracteres diferentes. Ademas necesita conocer el termino anterior, para
 * utilizar el frontCoding*/
termino_t* termino_leer(termino_t* termino_anterior, debuffer_t* debuff_FrontCoding, FILE* archDiferentes);

/* Permite obtener la cadena perteneciente a un termino (la palabra)
 * Memoria: guarda memoria para una copia de la palabra*/
char* termino_obtenerPalabra(termino_t*);

/* Permite comparar dos terminos.*/
int terminos_comparar(const void*,const void*);

/* Funcion auxiliar que permite imprimir un termino por pantalla*/
void termino_imprimir(termino_t*);

/* Devuelve la frecuencia de un termino, es decir, la cantidad de documentos
 * en los que aparece */
size_t termino_obtener_frecuencia(termino_t*);

/* Devuelve la posición en bits en la que comienzan los datos de un termino */
size_t termino_obtener_offset(termino_t*);

#endif /* TERMINO_H_ */
