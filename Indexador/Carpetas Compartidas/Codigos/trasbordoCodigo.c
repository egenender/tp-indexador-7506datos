#include "trasbordoCodigo.h"


void comprimir_FrecuenciaDocumentos(buffer_t* cod, unsigned int num){
	codificador_codificarDelta(cod,num);
}

void comprimir_FrecuenciaPosiciones(buffer_t* cod, unsigned int num){
	codificador_codificarDelta(cod,num);
}

void comprimir_IndiceDistanciaDocumentos(buffer_t* cod, unsigned int num, size_t b){
	//codificador_codificarGamma(cod,num);
	codificador_codificarGolomb(cod, num, b);
}

void comprimir_IndiceDistanciaPosiciones(buffer_t* cod, unsigned int num){
	codificador_codificarDelta(cod, num);
}

void comprimir_LexicoRepetidos(buffer_t* cod, unsigned int num){
	codificador_codificarGamma(cod,num);
}

void comprimir_LexicoDiferentes(buffer_t* cod, unsigned int num){
	codificador_codificarGamma(cod,num);
}

void comprimir_LexicoOffset(buffer_t* cod, unsigned int num){
	codificador_codificarDelta(cod,num);
}

unsigned int descomprimir_FrecuenciaDocumentos(debuffer_t* cod){
	return decodificador_decodificarDelta(cod);
}

unsigned int descomprimir_FrecuenciaPosiciones(debuffer_t* cod){
	return decodificador_decodificarDelta(cod);
}

unsigned int descomprimir_IndiceDistanciaDocumentos(debuffer_t* cod, size_t b){
	//return decodificador_decodificarGamma(cod);
	return decodificador_decodificarGolomb(cod, b);
}

unsigned int descomprimir_IndiceDistanciaPosiciones(debuffer_t* cod){
	return decodificador_decodificarDelta(cod);
}

unsigned int descomprimir_LexicoRepetidos(debuffer_t* cod){
	return decodificador_decodificarGamma(cod);
}

unsigned int descomprimir_LexicoDiferentes(debuffer_t* cod){
	return decodificador_decodificarGamma(cod);
}

unsigned int descomprimir_LexicoOffset(debuffer_t* cod){
	return decodificador_decodificarDelta(cod);
}

void comprimir_BGolomb(buffer_t* cod, unsigned int num){
	codificador_codificarGamma(cod, num);
}

unsigned int descomprimir_BGolomb(debuffer_t* cod){
	return decodificador_decodificarGamma(cod);
}
