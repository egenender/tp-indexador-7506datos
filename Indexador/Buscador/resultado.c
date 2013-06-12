#include "resultado.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../Carpetas Compartidas/TDAs/heap.h"
#include "../Carpetas Compartidas/TDAs/hash.h"
#include "../Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"
#include <string.h>

typedef struct{
	hash_t* hash;
	size_t pos;
	size_t freq;
}contenedorTermino_t;

struct resultado{
	contenedorTermino_t** hashes_terminos;
	size_t cantidad;
};

typedef struct solucion{
	size_t doc;
	size_t cant;
}solucion_t;

void destructorListas(void* a){
	lista_t* b = (lista_t*)a;
	lista_destruir(b, free);
}

int compararFrecuencias(const void* a,const void* b){
	contenedorTermino_t* c1 = (contenedorTermino_t*)a;
	contenedorTermino_t* c2 = (contenedorTermino_t*)b;
	return c1->freq - c2->freq;
}

char* __sizeToString(size_t num){
	size_t cant = 0;
	size_t aux = num;
	while (aux > 0){
		aux /= 10;
		cant++;
	}
	char* str = malloc (sizeof(char) * (cant+1));
	for (unsigned int i = 1; i <= cant; i++){
		str[cant - i ] = '0' + num % 10;
		num /= 10;
	}
	str[cant] = '\0';
	return str;
}


resultado_t* resultado_crear(termino_t** terminos, size_t cantidad, const char* dirOffsets, const char* ruta_tams){
	if (!terminos|| cantidad <= 1 || !dirOffsets)
		return NULL;
	resultado_t* resul = malloc (sizeof(resultado_t));
	resul->hashes_terminos = malloc (sizeof(contenedorTermino_t*) * cantidad);
	resul->cantidad = cantidad;

	for (size_t i = 0; i < cantidad; i++){
		resul->hashes_terminos[i] = malloc(sizeof(contenedorTermino_t));
		resul->hashes_terminos[i]->hash = hash_crear(destructorListas);
		resul->hashes_terminos[i]->pos = i;
	}

	for (size_t i = 0; i < cantidad; i++){

		termino_t* actual = terminos[i];
		lista_t* infoTerminos = termino_decodificarPunteros(actual, dirOffsets, ruta_tams);
		if (!infoTerminos){
			resultado_destruir(resul);
			return NULL;
		}

		lista_t* documentos = lista_borrar_primero(infoTerminos);
		resul->hashes_terminos[i]->freq = lista_largo(documentos);
		lista_iter_t* iter = lista_iter_crear(documentos);
		while (!lista_iter_al_final(iter)){
			size_t numDoc = *((size_t*)lista_iter_ver_actual(iter));
			char* clave = __sizeToString(numDoc);
			hash_guardar(resul->hashes_terminos[i]->hash, clave, lista_borrar_primero(infoTerminos));
			free(clave);
			lista_iter_avanzar(iter);
		}
		lista_iter_destruir(iter);
		lista_destruir(documentos, free);
		lista_destruir(infoTerminos, NULL);
	}

	heapsort((void*)resul->hashes_terminos, cantidad, compararFrecuencias);
	return resul;
}

lista_t* obtenerAparicionDocumentos (resultado_t* resul){

	lista_t* interseccion = lista_crear();
	hash_iter_t* iter = hash_iter_crear(resul->hashes_terminos[0]->hash);
	size_t i = 1;
	while (!hash_iter_al_final(iter)){
		const char* aux = hash_iter_ver_actual(iter);
		char* doc = malloc (sizeof(char) * (strlen(aux)+1));
		strcpy(doc, aux);
		lista_insertar_ultimo(interseccion, doc);
		if (i < hash_cantidad(resul->hashes_terminos[0]->hash))
			hash_iter_avanzar(iter);
		else
			break;
		i++;
	}

	hash_iter_destruir(iter);

	for (size_t i = 1; i < resul->cantidad && lista_largo(interseccion) > 0; i++){
		lista_iter_t* iterInt = lista_iter_crear(interseccion);
		lista_t* aux = lista_crear();
		while(!lista_iter_al_final(iterInt)){
			if (hash_pertenece(resul->hashes_terminos[i]->hash, (const char*) lista_iter_ver_actual(iterInt))){
				lista_insertar_primero(aux, lista_iter_ver_actual(iterInt));
			}else{
				free(lista_iter_ver_actual(iterInt));
			}

			lista_iter_avanzar(iterInt);
		}
		lista_iter_destruir(iterInt);
		lista_destruir(interseccion,NULL);
		interseccion = aux;
	}
	return interseccion;
}

void __copiarListas(lista_t* dst, lista_t* org, size_t pos){
lista_iter_t* iter = lista_iter_crear(org);
	while (!lista_iter_al_final(iter)){
		size_t* act = lista_iter_ver_actual(iter);
		if ((*act) >= pos){
			size_t* cpy = malloc (sizeof(size_t));
			*cpy = (*act) - pos;
			lista_insertar_ultimo(dst, cpy);
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
}

void intersecarPosiciones(lista_t** posiciones, lista_t* posSec, size_t num){
	lista_iter_t* iter = lista_iter_crear(*posiciones);
	lista_t* aux = lista_crear();

	while (!lista_iter_al_final(iter)){
		size_t* pos = lista_iter_ver_actual(iter);
		lista_iter_t* iterSec = lista_iter_crear(posSec);
		bool pasado = false;
		while(!lista_iter_al_final(iterSec) && !pasado){
			size_t* posSec = lista_iter_ver_actual(iterSec);
			pasado = (((*posSec) - num) > (*pos)) && (*posSec > num);
			if ((*posSec - num) == *pos){
				size_t* cpy = malloc (sizeof(size_t));
				*cpy = *pos;
				lista_insertar_ultimo(aux, cpy);
			}
			lista_iter_avanzar(iterSec);
		}
		lista_iter_destruir(iterSec);
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	lista_destruir(*posiciones, free);
	*posiciones = aux;
}

lista_t* resultado_realizarIntersecciones(resultado_t* resul){
	if (!resul) return NULL;

	//obtengo todos los documentos en los que se encuentren todos los terminos
	lista_t* documentos = obtenerAparicionDocumentos(resul);

	//Ahora veo, por cada documento, que haya interseccion:
	lista_t* soluciones = lista_crear();

	lista_iter_t* iter = lista_iter_crear(documentos);
	while (!lista_iter_al_final(iter)){
		const char* doc = lista_iter_ver_actual(iter);

		lista_t* posiciones = lista_crear();
		__copiarListas(posiciones, hash_obtener(resul->hashes_terminos[0]->hash, doc), resul->hashes_terminos[0]->pos);

		for (size_t i = 1; i < resul->cantidad && lista_largo(posiciones) > 0; i++){
			lista_t* posSec = hash_obtener(resul->hashes_terminos[i]->hash, doc);
			intersecarPosiciones(&posiciones, posSec, resul->hashes_terminos[i]->pos);
		}

		if (lista_largo(posiciones) > 0){
			//TENGO APARICION!
			solucion_t* solucion = malloc (sizeof(solucion_t));
			solucion->doc = atoi(doc);
			solucion->cant = lista_largo(posiciones);
			lista_insertar_ultimo(soluciones, solucion);
		}
		lista_destruir(posiciones, free);
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	lista_destruir(documentos, free);
	return soluciones;
}


void solucion_emitir(lista_t* soluciones, const char* paths, const char* offsets){
	if (lista_largo(soluciones) == 0){
		printf ("No se encontro la frase buscada\n");
		return;
	}
	printf("La frase buscada se encontro en %u documentos\n", lista_largo(soluciones));
	printf("Los documentos fueron:\n");
	lista_iter_t* iter = lista_iter_crear(soluciones);
	while (!lista_iter_al_final(iter)){
		solucion_t* solucion = lista_iter_ver_actual(iter);
		char* nombre_doc = __obtenerNombreDoc(paths, offsets, solucion->doc);
		printf("En %s se encontro la frase %u veces\n", nombre_doc, solucion->cant);
		free(nombre_doc);
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
}

void destructor_solucion(void* a){
	solucion_t* sol = (solucion_t*)a;
	free(sol);
}

void resultado_destruir(resultado_t* resul){
	if (!resul) return;
	for (size_t i = 0; i < resul->cantidad; i++){
		hash_destruir(resul->hashes_terminos[i]->hash);
		free(resul->hashes_terminos[i]);
	}
	free(resul->hashes_terminos);
	free(resul);
}
