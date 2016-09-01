#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "generadorMatrices.h"


float** multiplicarMatrices(int lado, float** matriz1, float** matriz2);
float multiplicarVectores(int lado,float* fila, float* columna);
