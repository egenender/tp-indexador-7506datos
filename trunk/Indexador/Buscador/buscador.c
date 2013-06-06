#include "buscador.h"
#include "../Carpetas Compartidas/TDAs/trie.h"
#include "../Carpetas Compartidas/Manejo de Archivos/termino.h"
#include "Lexico/levantador.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>


/**************************************************************************************/
/*                                     BUSCADOR                                       */
/**************************************************************************************/

//Por ahora solo podemos tenemos la estructura donde guardamos el lexico, pero podriamos
//necesitar varias cosas mas!
struct buscador{
	trie_t* almacenador;
};


void destructor_terminos(void* dato){
	termino_t* termino = (termino_t*) dato;
	termino_destruir(termino);
}

bool guardar_aux(void* a, const char* b, void* c){
	return trie_guardar((trie_t*) a, b, c);
}

buscador_t* buscador_crear(const char* rutaFrontCoding, const char* rutaDiferentes){
	buscador_t* b = malloc (sizeof(buscador_t));
	if (!b) return NULL;

	b->almacenador = trie_crear(destructor_terminos);
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

	while (!lista_iter_al_final(iter)){
		//Esto es lo que hay que cambiar:
		char* termino = lista_iter_ver_actual(iter);

		if (trie_pertenece(buscador->almacenador, termino)){
			termino_t* term =trie_obtener(buscador->almacenador, termino);
			vector_terminos[cont] = term;
		}else{
			vector_terminos[cont] = NULL; 	//Aca podriamos decir que no hay ninguna solucion
											//Posible, porque no se encontro uno de los terminos
		}
		cont++;
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	//heapsort((void*)vector_terminos, lista_largo(terminos_buscados), terminos_comparar);

	for (size_t i = 0; i < lista_largo(terminos_buscados); i++){
		termino_imprimir(vector_terminos[i]);
	}
	resultado_t* resul = resultado_crear(vector_terminos, lista_largo(terminos_buscados), dirOffsets);
	free(vector_terminos);
	return resul;
}


void buscador_destruir(buscador_t* busq){
	if (!busq) return;
	trie_destruir(busq->almacenador);
	free(busq);
}
