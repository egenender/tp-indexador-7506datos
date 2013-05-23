#ifndef BUSCADOR_H_
#define BUSCADOR_H_
#include "../Carpetas Compartidas/TDAs/lista.h"
typedef struct buscador buscador_t;


/* Permite crear un buscador a partir de las rutas de los archivos de Front
 * Coding y otro con los caracteres diferentes.
 * Memoria: Almacena memoria para el buscador*/
buscador_t* buscador_crear(const char* rutaFrontCoding, const char* rutaDiferentes);

/* Permite liberar la memoria reservada para un buscador*/
void buscador_destruir(buscador_t*);

//FIXME: esto deberia devolver un solucion_t*, por ahora solo va a imprimir los OK :P
void buscador_buscar(buscador_t* buscador, lista_t* terminos_buscados);



#endif /* BUSCADOR_H_ */
