#include "../debuffer.h"
#include "../buffer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int mainSIQUERESPROBAR (){
	FILE* file1 = fopen("DEbuff.jem","r");
	debuffer_t* debuff = debuffer_crear(file1);
	
	FILE* file2 = fopen("buff.jem","w");
	buffer_t* buff = buffer_crear(file2);
	
	for (int i = 0; i<80; i++)
		buffer_escribir_bit(buff,   debuffer_leer_bit(debuff)       );
	
	buffer_destruir(buff);
	debuffer_destruir(debuff);
	
	fclose(file1);
	fclose(file2);
	return 0;
}
