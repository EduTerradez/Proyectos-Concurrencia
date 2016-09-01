#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "generadorMatrices.h"


void* multiplicarMatrices(void* params);
float multiplicarVectores(int lado,float* fila, float* columna);
float** multiplicarMatricesParalelo(int lado, float** matriz1, float** matriz2, int numeroThreads, int m);
void divideTrabajo(int lado,int numThreads, int m, float** matriz1, float** matriz2, float** resultado);
