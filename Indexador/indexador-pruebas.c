#include <stdio.h>
#include "Parseo/parserIndex.h"
#include <stdlib.h>

int main (int argc, char** argv){

	char* cadenas[2];
	if (parserIndex_obtenerParametros(argc, argv, cadenas) == 0){
		printf("Se parseo bien. Nombre de repo: %s, Directorio: %s\n", cadenas[0], cadenas[1]);

		char** dirs = NULL;
		int cant;
		if (parserIndex_obtenerRutasDirectorios(cadenas[1], &dirs, &cant) == PARSERINDEX_OK){
			printf("Se encontrar los siguientes archivos:\n");
			for (int i = 0; i < cant; i++){
				printf("%s\n",dirs[i]);
			}
		}else{
			printf("Hubo algun error en la busqueda por directorios\n");
		}

		free(cadenas[0]);
		free(cadenas[1]);

		for (int j = 0; j < cant; j++)
			free(dirs[j]);
		free(dirs);
	}else{
		printf("Hubo un error en el parseo\n");
	}




	return 0;
}
