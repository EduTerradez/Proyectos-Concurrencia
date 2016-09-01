#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "generadorMatrices.h"

typedef struct params_t {
	float* vector1;
	float* vector2;
	float** matrizResultado;
	int fila;
	int columna;
	int lado;
}params_t;

void* multiplicarVectores(void* params){
	params_t* estructura = (params_t *)params;
	float resultado = 0;
	int lado = estructura->lado;

	for(int i = 0; i < lado; i++){
		resultado += estructura->vector1[i] * estructura->vector2[i];
	}

	estructura->matrizResultado[estructura->fila][estructura->columna] = resultado;
	delete estructura;
	pthread_exit(NULL);
}

float** multiplicarMatrices(int lado, float** matriz1, float** matriz2){
	float** resultado = (float**)malloc(lado*sizeof(float*));

	for(int i = 0; i < lado;i++){
		resultado[i] = (float*)malloc(lado*sizeof(float));
	}

	pthread_t* thread_id = (pthread_t*)malloc(lado*lado * sizeof(pthread_t));

	for(int i =0 ; i < lado;i++){
		for(int j =0 ; j < lado;j++){
			params_t* p = new params_t();
			p->lado =lado;
			p->vector1 = matriz1[i];
			p->vector2 = matriz2[j];
			p->matrizResultado = resultado;
			p->fila = i;
			p->columna = j;
			//resultado[i][j] = multiplicarVectores(lado, matriz1[i], matriz2[j]);
			pthread_create(&thread_id[i*lado + j], NULL, multiplicarVectores, p);
		}

		for(int j =0 ; j < lado;j++){
				void* resultado;
				pthread_join(thread_id[i*lado + j], &resultado);
			}
	}

	/*for(int i =0 ; i < lado;i++){
		for(int j =0 ; j < lado;j++){
			void* resultado;
			pthread_join(thread_id[i*lado + j], &resultado);
		}
	}*/
	return resultado;
}
