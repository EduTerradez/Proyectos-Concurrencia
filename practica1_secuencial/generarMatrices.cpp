
#include "multiplicacionMatrices.h"
//argc-> argument count
//argv-> argument variables
int main(int argc,  char** argv){
	int lado = 0;

	if(argc > 1){
		lado = atoi(argv[1]);
	}
	else{
		printf("Error en los parametros");
	}

	generarMatrizCuadrada(lado, "matriz1");
	generarMatrizIdentidad(lado, "matriz2");

	//imprimirMatriz("matrizResultado");


}
