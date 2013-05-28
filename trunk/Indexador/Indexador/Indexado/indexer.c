#include "indexer.h"
#include "../../Carpetas Compartidas/Manejo de Archivos/buffer.h"
#include <stdio.h>
#include "../../Carpetas Compartidas/Manejo de Archivos/funcionesGeneralesArchivos.h"
#include "../../Carpetas Compartidas/Manejo de Archivos/registro.h"
#include "../../Carpetas Compartidas/Log/log.h"
#include "../../Carpetas Compartidas/TDAs/cola.h"
#define TAM_TERMINO_IMPOSIBLE 200


//void indexarEntrada(registro_t* actual, registro_t* anterior, FILE* archIndex, FILE* archLexico, unsigned long *frecDoc, unsigned long *frecPos, unsigned long* offset);

int indexer_indexar(const char* origen, const char* destino_index, const char* destino_lexico_frontCoding, const char* destino_lexico_diferentes){
	FILE* archOrigen = fopen(origen, lectura_archivos());
	FILE* archIndice = fopen(destino_index, escritura_archivos());
	FILE* archFrontCoding = fopen(destino_lexico_frontCoding, escritura_archivos());
	FILE* archDiferentes = fopen(destino_lexico_diferentes, escritura_archivos());
	if (!archOrigen || !archIndice || !archFrontCoding || ! archDiferentes){
		fclose(archOrigen);
		fclose(archIndice);
		fclose(archFrontCoding);
		fclose(archDiferentes);
		return INDEXER_ERROR;
	}

	buffer_t* buff_indice = buffer_crear(archIndice);
	buffer_t* buff_frontcoding =  buffer_crear(archFrontCoding);

	lista_t* posiciones = lista_crear();
	lista_t* documentos = lista_crear();
	registro_t* registro_anterior = NULL;

	log_emitir("Inicia el Indexado de archivos", LOG_ENTRADA_PROCESO);
	unsigned long i = 0;
	while (!feof(archOrigen)){
		emitir_impresion("Indexando Terminos y Punteros", i++, 12000);
		registro_t* registro = registro_leer(archOrigen);
		if (!registro && !feof(archOrigen)){
			log_emitir("Lectura del archivo ordenado incorrecto", LOG_ENTRADA_ERROR);
			continue;
		}

		registro_escribirEnIndice(registro, registro_anterior, buff_indice, buff_frontcoding, archDiferentes, documentos, posiciones);

		if (registro_anterior)
			registro_destruir(registro_anterior);
		registro_anterior = registro;

	}
	if (registro_anterior)
		registro_destruir(registro_anterior);

	registro_escribirEnIndice(NULL, registro_anterior, buff_indice, buff_frontcoding,archDiferentes, documentos, posiciones);

	log_emitir("Finalizo el Indexado de archivos", LOG_ENTRADA_PROCESO);

	buffer_rellenar(buff_indice);
	buffer_rellenar(buff_frontcoding);
	buffer_destruir(buff_indice);
	buffer_destruir(buff_frontcoding);
	lista_destruir(documentos,NULL);
	lista_destruir(posiciones, NULL);
	fclose(archOrigen);
	fclose(archFrontCoding);
	fclose(archDiferentes);
	fclose(archIndice);
	return INDEXER_OK;
}
