#include <stdlib.h>
#include "arbolHuff.h"
#include <stdbool.h>
#include <string.h>
#include "heap.h"

typedef struct nodo_huff{
	char* clave;
	unsigned int dato;
	struct nodo_huff* izq;
	struct nodo_huff* der;
	struct nodo_huff* padre;
//	bool valido;
}nodo_arbol_huff_t;

struct arbol_huff{
	nodo_arbol_huff_t* raiz;
	nodo_arbol_huff_t** vector_hojas;
};

void crear_con_base_de_arbol(arbol_huff_t* arbol, size_t b);
nodo_arbol_huff_t* arbol_huff_unir_nodos(nodo_arbol_huff_t* nodoIzq, nodo_arbol_huff_t* nodoDer);
nodo_arbol_huff_t* nodo_arbol_huff_crear(const char* clave, unsigned int dato);//, bool valido);


//Crea el arbol, asignando las funciones de comparacion y de destruccion
// de dato
arbol_huff_t* arbol_huff_crear(size_t b){
	arbol_huff_t* arbol = malloc(sizeof(arbol_huff_t));
	if (!arbol) return NULL;

	arbol->raiz=NULL;

	arbol->vector_hojas = malloc(sizeof(nodo_arbol_huff_t*)*b);
	if (!arbol->vector_hojas){
		free(arbol);
		return NULL;
	}

	crear_con_base_de_arbol(arbol,b);

	return arbol;

}

int comparacion_prefijo(const void* a, const void* b){
	nodo_arbol_huff_t* nodoIzq = (nodo_arbol_huff_t*)a;
	nodo_arbol_huff_t* nodoDer = (nodo_arbol_huff_t*)b;
	char* claveIzq =  nodoIzq->clave;
	char* claveDer =  nodoDer->clave;
	return atoi(claveDer)- atoi(claveIzq);
}

void crear_con_base_de_arbol(arbol_huff_t* arbol, size_t b){
	if(b== 0 || !arbol)
		return;

	heap_t* heap = heap_crear((cmp_func_t)comparacion_prefijo);
	if(!heap)
		return;

	char* clave;

	//FIXME: 6??
	clave = malloc(sizeof(char) * 6);

	if(!clave)
		return;

	strcpy(clave, "1");

	nodo_arbol_huff_t* nodo;
	unsigned int dato;
	for(unsigned int i=0; i<b;i++){
		dato = i;
		nodo = nodo_arbol_huff_crear(clave,dato);
		heap_encolar(heap, (void*) nodo);//, true) );
		arbol->vector_hojas[i] = nodo;
	}

	nodo_arbol_huff_t* nodoIzq;
	nodo_arbol_huff_t* nodoDer;

	while(heap_cantidad(heap) > 1){
		nodoIzq = heap_desencolar(heap);
		nodoDer = heap_desencolar(heap);
		//printf("izq: %s \n",nodoIzq->clave);
		//printf("der: %s \n",nodoDer->clave);
		heap_encolar(heap, arbol_huff_unir_nodos(nodoIzq,nodoDer));
	}

	arbol->raiz = heap_desencolar(heap);

	free(clave);
	heap_destruir(heap, NULL);

}


//Crea el nodo con la clave y el dato correspondientes
nodo_arbol_huff_t* nodo_arbol_huff_crear(const char* clave, unsigned int dato){//, bool valido){
	//Pido memoria para la estructura de nodo
	nodo_arbol_huff_t* nodo = malloc(sizeof(nodo_arbol_huff_t));
	if (!nodo) return NULL;

	//Pido memoria para la copia interna de la clave
	nodo->clave = malloc(sizeof(char)*(strlen(clave)+1));
	if (!nodo->clave){
		free(nodo);
		return NULL;
	}

	//Copio la clave
	strcpy(nodo->clave,clave);
	//No tiene hijos
	nodo->der=NULL;
	nodo->izq=NULL;
	//Le copio el dato
	nodo->dato=dato;

	//nodo->valido=valido;
	return nodo;
}


nodo_arbol_huff_t* arbol_huff_unir_nodos(nodo_arbol_huff_t* nodoIzq, nodo_arbol_huff_t* nodoDer){
	if(!nodoIzq || !nodoDer)
		return NULL;

	unsigned int numIzq = atoi(nodoIzq->clave);
	unsigned int numDer = atoi(nodoDer->clave);
	unsigned int suma = numIzq + numDer;

	//FIXME: ojo con esto
	char* claveUnion = malloc(sizeof(char)* 12);

	//itoa(suma, claveUnion, 10);

	sprintf(claveUnion,"%u", suma);

	nodo_arbol_huff_t* nodoUnion = nodo_arbol_huff_crear(claveUnion,DATO_NULO);//, false);

	nodoUnion->izq = nodoIzq;
	nodoUnion->der = nodoDer;
	nodoIzq->padre = nodoUnion;
	nodoDer->padre = nodoUnion;

	free(claveUnion);

	return nodoUnion;
}

bool encontrar_en_arbol(nodo_arbol_huff_t* nodo, unsigned int num, lista_t* lista){

	if(!nodo)
		return false;

	//printf("encontrar: %u \n",nodo->dato);
	//if(nodo->valido && nodo->dato == num)
	if(nodo->dato == num)
		return true;

	Byte_t* direccion = malloc(sizeof(Byte_t));
	if(!direccion)
		return false;

	if(encontrar_en_arbol(nodo->izq,num,lista)){
		*direccion = DIR_IZQ;
		lista_insertar_primero(lista,(void*)direccion);
		return true;
	}


	if(encontrar_en_arbol(nodo->der,num,lista)){
		*direccion = DIR_DER;
		lista_insertar_primero(lista,(void*)direccion);
		return true;
	}

	free(direccion);
	return false;

}

void encontrar_desde_nodo(arbol_huff_t* arbol,unsigned int num,lista_t* lista){

	nodo_arbol_huff_t* nodo_actual = arbol->vector_hojas[num];
	nodo_arbol_huff_t* nodo_anterior;

	while(nodo_actual != arbol->raiz){
		Byte_t* direccion = malloc(sizeof(Byte_t));
		if(!direccion)
			return ;


		nodo_anterior = nodo_actual;
		nodo_actual = nodo_actual->padre;

		if(nodo_actual->izq == nodo_anterior)
			*direccion = DIR_IZQ;
		else
			*direccion = DIR_DER;

		lista_insertar_primero(lista,direccion);
	}
}


lista_t* arbol_huff_obtener_lista_bits(arbol_huff_t* arbol,unsigned int num){

	lista_t* lista = lista_crear();

	if(!lista)
		return NULL;

	//encontrar_en_arbol(arbol->raiz,num,lista);
	encontrar_desde_nodo(arbol,num,lista);

	return lista;
}


unsigned int arbol_huff_obtener_numero(arbol_huff_t* arbol,debuffer_t* debuffer){
	if(!arbol || !debuffer){
		return 0;
	}

	unsigned int numero;
	Byte_t direccion;

	nodo_arbol_huff_t* nodo_actual = arbol->raiz;

	while(nodo_actual->dato == DATO_NULO){
		direccion = debuffer_leer_bit(debuffer);
		if (direccion == DIR_IZQ)
				nodo_actual = nodo_actual->izq;
		else //es DIR_DER
			nodo_actual = nodo_actual->der;
	}

	numero = nodo_actual->dato;

	return numero;
}

//Se encarga de eliminar toda las estructuras almacenadas en el nodo.
void nodo_arbol_huff_destruir(nodo_arbol_huff_t *nodo){
	//si no hay nodo, no hay qué destruir
	if(!nodo)	return;
	//detruyo el hijo izq
	nodo_arbol_huff_destruir(nodo->izq);
	//detruyo el hijo der
	nodo_arbol_huff_destruir(nodo->der);
	//libero la clave
	free(nodo->clave);
	//libero la estructura nodo
	free(nodo);
}

//Se encarga de eliminar toda las estructuras almacenadas en el arbol.
void arbol_huff_destruir(arbol_huff_t *arbol){
	//si no hay nodo, no hay qué destruir
	if(!arbol)	return;
	//tomo la raiz
	nodo_arbol_huff_t* nodo = arbol->raiz;
	if(!nodo){
		free(arbol);
		return;
	}
	//detruyo el hijo izq
	nodo_arbol_huff_destruir(nodo->izq);
	//detruyo el hijo der
	nodo_arbol_huff_destruir(nodo->der);
	//libero la clave
	free(nodo->clave);
	//libero la estructura nodo
	free(nodo);
	//libero el vector
	free(arbol->vector_hojas);
	//libero la estructura arbol
	free(arbol);
}
