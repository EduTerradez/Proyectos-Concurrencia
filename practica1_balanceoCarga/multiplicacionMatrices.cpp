#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <vector>
#include "generadorMatrices.h"
#include "multiplicacionMatrices.h"
using namespace std;

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

vector<params_t*> trabajos;
pthread_mutex_t cerrojito;


float multiplicarVectores(int lado,float* fila, float* columna){
	float resultado = 0;
	for(int i = 0; i < lado; i++){
		resultado += fila[i] * columna[i];
	}
	return resultado;
}

void* multiplicarMatrices(void* params){
	params_t* p1;
	while(trabajos.size() > 0){

		pthread_mutex_lock(&cerrojito);
		p1 = trabajos.back();
		trabajos.pop_back(); //cerojos
		pthread_mutex_unlock(&cerrojito);

		if(p1 != NULL){
			for(int i = p1->filaInicio; i < p1->filaFinal;i++){
				for(int j =p1->columnaInicio ; j < p1->columnaFinal;j++){
					p1->matrizResultado[i][j] = multiplicarVectores(p1->lado, p1->matriz1[i], p1->matriz2[j]);

				}

			}
		}


	}
	pthread_exit(NULL);
}

float** multiplicarMatricesParalelo(int lado, float** matriz1, float** matriz2, int numeroThreads, int m){
	float** resultado = (float**)malloc(lado*sizeof(float*));

	for(int i = 0; i < lado;i++){
		resultado[i] = (float*)malloc(lado*sizeof(float));
	}

	divideTrabajo(lado, numeroThreads, m, matriz1, matriz2, resultado);
	pthread_t* thread_id = (pthread_t*)malloc(numeroThreads* sizeof(pthread_t));
	pthread_mutex_init(&cerrojito,NULL);

	for(int i = 0; i < numeroThreads;i++){
		pthread_create(&thread_id[i], NULL, multiplicarMatrices, NULL);
	}


	for(int j =0 ; j < numeroThreads;j++){
		void* r;
		pthread_join(thread_id[j], &r);
	}


	pthread_mutex_destroy(&cerrojito);

	return resultado;
}

void divideTrabajo(int lado,int numThreads, int m, float** matriz1, float** matriz2, float** resultado){
	int numTrabajos = numThreads * m;
	int tamano = lado / numTrabajos;

	for(int i = 0; i < numTrabajos - 1;i++){
		params_t* p = new params_t();
		p->matriz1 = matriz1;
		p->filaInicio = (tamano * i);
		p->filaFinal = (tamano) * (i+1);

		p->matriz2 = matriz2;
		p->columnaInicio = 0;
		p->columnaFinal = lado;

		p->lado = lado;
		p->matrizResultado = resultado;

		trabajos.push_back(p);
	}

		params_t* p = new params_t();
		p->matriz1 = matriz1;
		p->filaInicio = (tamano * (numTrabajos - 1));
		p->filaFinal = lado;

		p->matriz2 = matriz2;
		p->columnaInicio = 0;
		p->columnaFinal = lado;

		p->lado = lado;
		p->matrizResultado = resultado;

		trabajos.push_back(p);

}



