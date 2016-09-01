#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "generadorMatrices.h"
#include "multiplicacionMatrices.h"

typedef struct params_t {
	float** matriz1;
	int filaInicio;
	int filaFinal;

	float** matriz2;
	int columnaInicio;
	int columnaFinal;

	int lado;
	float** matrizResultado;
}params_t;

float multiplicarVectores(int lado,float* fila, float* columna){
	float resultado = 0;
	for(int i = 0; i < lado; i++){
		resultado += fila[i] * columna[i];
	}
	return resultado;
}
void* multiplicarMatrices(void* params){
		params_t* p1 = (params_t*) params;
		for(int i = p1->filaInicio; i < p1->filaFinal;i++){
			for(int j =p1->columnaInicio ; j < p1->columnaFinal;j++){
				p1->matrizResultado[i][j] = multiplicarVectores(p1->lado, p1->matriz1[i], p1->matriz2[j]);

			}

		}
		pthread_exit(NULL);
}

float** multiplicarMatricesParalelo(int lado, float** matriz1, float** matriz2, int numeroThreads){
	float** resultado = (float**)malloc(lado*sizeof(float*));

	for(int i = 0; i < lado;i++){
		resultado[i] = (float*)malloc(lado*sizeof(float));
	}
	pthread_t* thread_id = (pthread_t*)malloc(numeroThreads* sizeof(pthread_t));

	for(int i = 0; i < numeroThreads;i++){
		params_t* p = new params_t();
		p->matriz1 = matriz1;
		p->filaInicio = ((lado/numeroThreads) * i);
		p->filaFinal = (lado/ numeroThreads) * (i+1);

		p->matriz2 = matriz2;
		p->columnaInicio = 0;
		p->columnaFinal = lado;


		p->lado = lado;
		p->matrizResultado = resultado;
		pthread_create(&thread_id[i], NULL, multiplicarMatrices , p);
	}

	for(int j =0 ; j < numeroThreads;j++){
		void* resultado;
		pthread_join(thread_id[j], &resultado);
	}
	return resultado;
}
