#include "buscador.h"
#include "../Carpetas Compartidas/TDAs/hash.h"
#include "../Carpetas Compartidas/TDAs/abb.h"
#include "../Carpetas Compartidas/Manejo de Archivos/termino.h"
#include "../Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"
#include "Lexico/levantador.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/**************************************************************************************/
/*                                     BUSCADOR                                       */
/**************************************************************************************/

//Por ahora solo podemos tenemos la estructura donde guardamos el lexico, pero podriamos
//necesitar varias cosas mas!
struct buscador{
	hash_t* almacenador;
};


void destructor_terminos(void* dato){
	termino_t* termino = (termino_t*) dato;
	termino_destruir(termino);
}

bool guardar_aux(void* a, const char* b, void* c){
	return hash_guardar((hash_t*) a, b, c);
}

buscador_t* buscador_crear(const char* rutaFrontCoding, const char* rutaDiferentes){
	buscador_t* b = malloc (sizeof(buscador_t));
	if (!b) return NULL;

	b->almacenador = hash_crear(destructor_terminos);
	levantador_obtenerContenedorLexico(b->almacenador, guardar_aux, rutaFrontCoding, rutaDiferentes);

	return b;
}

resultado_t* buscador_buscar(buscador_t* buscador, lista_t* terminos_buscados, const char* dirOffsets){
	if (!buscador || !terminos_buscados || lista_largo(terminos_buscados) == 0){
		return NULL;
	}

	termino_t** vector_terminos = malloc (sizeof(termino_t*) * lista_largo(terminos_buscados));
	if (!vector_terminos) return NULL;
	size_t cont = 0;

	lista_iter_t* iter = lista_iter_crear(terminos_buscados);
	abb_t* arbol_buscados = abb_crear(strcmp,NULL);
	while (!lista_iter_al_final(iter)){
		//Esto es lo que hay que cambiar:
		char* termino = lista_iter_ver_actual(iter);

		if (hash_pertenece(buscador->almacenador, termino)){
			termino_t* term = hash_obtener(buscador->almacenador, termino);
			if (!abb_pertenece(arbol_buscados, termino)){
				vector_terminos[cont] = term;
				cont++;
			}
			abb_guardar(arbol_buscados, termino, NULL);
		}else{
			vector_terminos[cont] = NULL; 	//Aca podriamos decir que no hay ninguna solucion
			cont++;							//Posible, porque no se encontro uno de los terminos
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);

	for (size_t i = 0; i < cont; i++){
		termino_imprimir(vector_terminos[i]);
	}
	abb_destruir(arbol_buscados);
	resultado_t* resul = resultado_crear(vector_terminos, cont, dirOffsets);
	free(vector_terminos);
	return resul;
}


void buscador_destruir(buscador_t* busq){
	if (!busq) return;
	hash_destruir(busq->almacenador);
	free(busq);
}

void buscador_busquedaPuntual(buscador_t* buscador, const char* termino, const char* index, const char* paths, const char* offsetPaths){
	if (!buscador || !termino) return;
	printf("Busqueda por palabra: %s\n",termino);

	termino_t* term = hash_obtener(buscador->almacenador, termino);
	if (!term){
		printf("La palabra buscada no aparece en ningun documento\n");
		return;
	}

	lista_t* infoTermino = termino_decodificarPunteros(term, index);

	lista_t* documentos = lista_borrar_primero(infoTermino);
	lista_iter_t* iter = lista_iter_crear(documentos);
	printf("El termino se encuentra en los documentos:\n");
	while (!lista_iter_al_final(iter)){
		size_t doc = *((size_t*) (lista_iter_ver_actual(iter)));
		char* nomDoc = __obtenerNombreDoc(paths, offsetPaths, doc);
		printf("%s en las posiciones:\n", nomDoc);
		free(nomDoc);
		lista_t* posiciones = lista_borrar_primero(infoTermino);
		lista_iter_t* iterPos = lista_iter_crear(posiciones);
		while (!lista_iter_al_final(iterPos)){
			size_t numPos = *((size_t*)(lista_iter_ver_actual(iterPos)));
			printf("\t%u\n", numPos);
			lista_iter_avanzar(iterPos);
		}
		lista_iter_destruir(iterPos);
		lista_destruir(posiciones, free);
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	lista_destruir(documentos, free);
	lista_destruir(infoTermino, NULL);

}
