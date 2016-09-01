#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "generadorMatrices.h"

float multiplicarVectores(int lado,float* fila, float* columna){
	float resultado = 0;
	for(int i = 0; i < lado; i++){
		resultado += fila[i] * columna[i];
	}
	return resultado;
}

float** multiplicarMatrices(int lado, float** matriz1, float** matriz2){
	float** resultado = (float**)malloc(lado*sizeof(float*));
	for(int i = 0; i < lado;i++){
		resultado[i] = (float*)malloc(lado*sizeof(float));
	}

	for(int i =0 ; i < lado;i++){
		for(int j =0 ; j < lado;j++){
			resultado[i][j] = multiplicarVectores(lado, matriz1[i], matriz2[j]);

		}

	}
	return resultado;

}

