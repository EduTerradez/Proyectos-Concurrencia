#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

//poner la \ en cada linea menos en el ultimo parentesis
#define cudaCheckError(){ \
cudaError_t e = cudaGetLastError(); \
if (e != cudaSuccess){ \
	printf("Cuda failures %s: %d: '%s'\n", __FILE__,__LINE__, cudaGetErrorString(e)); \
	exit(0); \
	} \
}

__device__ float multiplicarVector(int lado, float* fila, float* columna) {
	float resultado = 0;
	for (int i = 0; i < lado; i++) {
		resultado += fila[i] * columna[i];
	}
	return resultado;
}

__global__ void multiplicarMatrices_kernel(int size, float** matriz1, float** matriz2, float** resultado) {
	int col = blockIdx.y * blockDim.x + threadIdx.x;
	int fil = blockIdx.x * blockDim.y + threadIdx.y;

	if((col >= size) || (fil>= size)){
		return;
	}
	//printf("fila: %d columna: %d dato1: %f dato2: %f \n", fil, col, matriz1[fil][col], matriz2[col][fil]);
	resultado[fil][col] = multiplicarVector(size, matriz1[fil], matriz2[col]);

}

__global__ void verContenido(float** resultado) {
	int col = blockIdx.y * blockDim.x + threadIdx.x;
	int fil = blockIdx.x * blockDim.y + threadIdx.y;
	printf("dato: %f \n", resultado[fil][col]);
}

float** leerMatriz(char* nombreFichero, bool traspuesta, int size){
		ifstream fichero(nombreFichero,std::fstream::in | std::fstream::binary);
		fichero.read((char*)&nombreFichero,sizeof(int));

		float** matriz = (float**)malloc(size*sizeof(float*));
		int i;
		for(i = 0; i < size;i++){
			matriz[i] = (float*)malloc(size*sizeof(float));
		}

		if(!traspuesta){

			for(int i = 0; i < size;i++){
				for(int j = 0; j < size;j++){
					float aux;
					fichero.read((char*)&aux,sizeof(float));
					matriz[i][j] = aux;
				}
			}
		}
		else{
			for(int i = 0; i < size;i++){
							for(int j = 0; j < size;j++){
								float aux;
								fichero.read((char*)&aux,sizeof(float));
								matriz[j][i] = aux;
							}
						}

		}
		fichero.close();
		return matriz;
}

void pasarMatrizFichero(int lado, char* nombreFichero, float** matriz) {
	fstream fichero(nombreFichero, std::fstream::out | std::fstream::binary);

	char straux[100];


	sprintf(straux, "%d ", lado);

	fichero.write(straux, strlen(straux));

	float aux;
	for (int i = 0; i < lado; i++) {
		sprintf(straux, "\n");
		fichero.write(straux, strlen(straux));
		for (int j = 0; j < lado; j++) {
			aux = matriz[i][j];


			sprintf(straux, "%f ", aux);
			fichero.write(straux, strlen(straux));
		}
	}
	fichero.close();
}

int main(int argc, char** argv) {
	int SIZE = 10;

	if (argc > 1) {
		SIZE = atoi(argv[1]);
	} else {
		printf("Error en los parametros\n");
	}
	cout << SIZE << endl;
	float** matriz1_CPU;
	float** matriz2_CPU;
	float** resultadoFinal_CPU;

	float** datosM1_CPU;
	float** datosM2_CPU;
	float **resultado_CPU;

	float** resultado_GPU;
	float** datosM1_GPU;
	float** datosM2_GPU;




	//obtener matrices de fichero
	matriz1_CPU = leerMatriz("matriz1", false, SIZE);
	matriz2_CPU =leerMatriz("matriz2", true, SIZE);

/*

	for(int i = 0; i<SIZE;i++){
		for(int j = 0; j < SIZE;j++){
			cout << " "<< matriz1_CPU[i][j];
		}
		cout << endl;
	}

	for(int i = 0; i<SIZE;i++){
			for(int j = 0; j < SIZE;j++){
				if(i == j){
					matriz2_CPU[i][j] = 1;
				}
				cout << " "<< matriz2_CPU[i][j];
			}
			cout << endl;
		}
*/
	//funsiona

	//reserva de memoria del resultado
	resultadoFinal_CPU= (float**)malloc(SIZE * sizeof(float*));
	for(int i = 0; i < SIZE;i++){
		resultadoFinal_CPU[i] = (float*)malloc(SIZE*sizeof(float));
	}

	datosM1_CPU = (float**)malloc(SIZE * sizeof(float*));
	datosM2_CPU = (float**)malloc(SIZE * sizeof(float*));
	resultado_CPU = (float**)malloc(SIZE * sizeof(float*));

	cudaMalloc((void**)&resultado_GPU,sizeof(float*)* SIZE);
	cudaMalloc((void**)&datosM2_GPU,sizeof(float*)* SIZE);
	cudaMalloc((void**)&datosM1_GPU,sizeof(float*)* SIZE);

	for(int i = 0; i < SIZE; i++){
		cudaMalloc((void**)&(datosM1_CPU[i]),sizeof(float)* SIZE);
		cudaMalloc((void**)&(datosM2_CPU[i]),sizeof(float)* SIZE);
		cudaMalloc((void**)&(resultado_CPU[i]),sizeof(float)* SIZE);
	}

	for(int i = 0; i < SIZE;i++){
		cudaMemcpy(datosM1_CPU[i], matriz1_CPU[i], SIZE * sizeof(float),cudaMemcpyHostToDevice);
		cudaMemcpy(datosM2_CPU[i], matriz2_CPU[i], SIZE * sizeof(float),cudaMemcpyHostToDevice);
	}

	cudaMemcpy(datosM1_GPU, datosM1_CPU, SIZE * sizeof(float*),cudaMemcpyHostToDevice);
	cudaMemcpy(datosM2_GPU, datosM2_CPU, SIZE * sizeof(float*),cudaMemcpyHostToDevice);
	cudaMemcpy(resultado_GPU, resultado_CPU, SIZE * sizeof(float*),cudaMemcpyHostToDevice);

	int blockSize = 32;

	dim3 gridDim = dim3((SIZE / blockSize) + 1, (SIZE / blockSize) + 1, 1);
	dim3 blockDim = dim3(blockSize, blockSize, 1);

	multiplicarMatrices_kernel <<<gridDim, blockDim>>> (SIZE, datosM1_GPU, datosM2_GPU, resultado_GPU);


	for(int i = 0; i < SIZE;i++){
		cudaMemcpy(resultadoFinal_CPU[i], resultado_CPU[i], SIZE * sizeof(float),cudaMemcpyDeviceToHost);
	}

	pasarMatrizFichero(SIZE, "MatrizResultado", resultadoFinal_CPU);

	//free

	for(int i = 0; i < SIZE; i++){
		free(matriz1_CPU[i]);
		free(matriz2_CPU[i]);
		free(resultadoFinal_CPU[i]);

		cudaFree(datosM1_CPU[i]);
		cudaFree(datosM2_CPU[i]);

		cudaFree(resultado_CPU[i]);

	}

	free(matriz1_CPU);
	free(matriz2_CPU);
	free(resultadoFinal_CPU);
	free(resultado_CPU);
	free(datosM1_CPU);
	free(datosM2_CPU);


	cudaFree(resultado_GPU);
	cudaFree(datosM1_GPU);
	cudaFree(datosM2_GPU);

	//free(resultadoAuxiliar_CPU);

}
