#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <cuda_runtime.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>




typedef struct params_t
{
	int filaInicio;
	int filaFinal;
	int lado;
}params_t;

float multiplicarVector(int lado, float* fila, float* columna) {
	float resultado = 0;
	int i;
	for (i = 0; i < lado; i++) {
		resultado += fila[i] * columna[i];
	}
	return resultado;
}

float** multiplicarMatrices(int numFilas, int lado, float** matriz1, float** matriz2){
	float** resultado = (float**)malloc(numFilas *sizeof(float*));
	int i;
	for(i = 0; i < numFilas;i++){
		resultado[i] = (float*)malloc(lado*sizeof(float));
	}
	for(i = 0; i < numFilas;i++){
		int j;
		for(j = 0; j < lado;j++){
			resultado[i][j] = multiplicarVector(lado, matriz1[i], matriz2[j]);
		}
	}

	return resultado;
}

void escribirMatriz (char* nombreFichero, float** matriz, int filas, int columnas){

	FILE * f;
	f = fopen(nombreFichero, "w");

	if (f != NULL){
		//introducimos numero de filas y columnas
		//fwrite(&filas, sizeof(int), 1, f);
		fprintf(f,"%d \n",filas);
		//printf("%d\n", filas);
		//fwrite(&columnas, sizeof(int), 1, f);
		//printf("%d\n", columnas);

		int i,j;
		for ( i = 0; i < filas; i++) {
			for ( j = 0; j < columnas; j++){
				//fwrite(&(matriz[i][j]), sizeof(float), 1, f);
				fprintf(f,"%.6f ",matriz[i][j]);
			}
			if( i!=(filas-1))
				fprintf(f,"\n");
		}
	}
	else{
		printf("Error al abrir el archivo\n");
	}

	fclose(f);
}

float** leerMatriz (char* nombreFichero, int lado, int traspuesta){

	FILE * f;
	int i,j;
	f = fopen(nombreFichero, "r");

	float** auxiliar;

	if (f != NULL){
		float matame;
		fread(&(matame), sizeof(float), 1, f);
		auxiliar = (float**) malloc(sizeof(float*)*lado);

		for ( i = 0; i < lado; i++)
			auxiliar[i] = (float*) malloc(sizeof(float)*lado);

		if(traspuesta){
			for ( i = 0; i < lado; i++) {
				for ( j = 0; j < lado; j++){
					fread(&(auxiliar[j][i]), sizeof(float), 1, f);
					//printf("%f\n", auxiliar[i][j]);
				}
			}
		}
		else {
			for ( i = 0; i < lado; i++) {
				for ( j = 0; j < lado; j++){
					fread(&(auxiliar[i][j]), sizeof(float), 1, f);
					//printf("%f\n", auxiliar[i][j]);
				}
			}
		}

	}
	else{
		printf("Error al abrir el archivo\n");
	}

	fclose(f);
	return auxiliar;
}



int main(int argc, char** argv)
{
	// /usr/local/cuda/bin/nvcc -c main.cu -o main.o
	// sh copy_files.sh
	// mpirun -np 3 -hostfile host_file ~/workspace/PracticaClusters/mpi_main


	/*
	10.4.0.23 slots=1
	10.4.0.17 slots=1
	10.4.0.11 slots=1
	10.4.0.20 slots=1
	10.4.0.17 slots=1
	 */

	int SIZE = 1000;

	/*if (argc > 1) {
		SIZE = atoi(argv[1]);
	} else {
		printf("Error en los parametros\n");
	}*/

	int rank; //identificador
	int nproc; //numero de procesos en caso de que sea un maestro.

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);



	if(rank == 0)
	{
		//Preguntar declaracion
		float** Matriz1;
		float** Matriz2;
		float** MatrizResultado;
		Matriz1 = leerMatriz("matriz1", SIZE, 0);
		Matriz2 = leerMatriz("matriz2", SIZE, 1);
		MatrizResultado = (float**)malloc(SIZE * sizeof(float*));

		MPI_Status status;

		params_t array[nproc -1];

		//envio datos
		int i;
		for(i = 1; i < nproc;i++){
			int numeroEsclavos = nproc-1;

			array[i-1].filaInicio = ((SIZE/numeroEsclavos) * (i-1));
			array[i-1].filaFinal = (SIZE/numeroEsclavos) * i;
			if(i == (nproc - 1)){
				array[i-1].filaFinal = SIZE;
			}

			array[i-1].lado = SIZE;

			MPI_Send(&array[i-1], sizeof(params_t), MPI_BYTE, i, 0,MPI_COMM_WORLD);

			int fila;
			for(fila= array[i-1].filaInicio; fila < array[i-1].filaFinal;fila++){
				MPI_Send(Matriz1[fila], SIZE * sizeof(float), MPI_BYTE, i, 0,MPI_COMM_WORLD);
			}
			int columna;
			for(columna= 0; columna < SIZE;columna++){
				MPI_Send(Matriz2[columna], SIZE * sizeof(float), MPI_BYTE, i, 0,MPI_COMM_WORLD);
			}

			printf("Trozo enviado al nodo %d \n", i);
		}


		//recibir datos
		for(i = 1; i < nproc;i++){

			//int numFilasEsclavo = array[i-1].filaFinal - array[i-1].filaInicio;

			int fila;
			for(fila = array[i-1].filaInicio; fila < array[i-1].filaFinal; fila++){
				MatrizResultado[fila] = (float*) malloc(SIZE * sizeof(float));
				MPI_Recv(MatrizResultado[fila], sizeof(float) * SIZE, MPI_BYTE,i,0, MPI_COMM_WORLD, &status);
			}

			printf("trozo recibido del nodo %d \n", i);


			//sincronizacion
			//int final = 1;
			//MPI_Send(&final, sizeof(int), MPI_BYTE, i, 0,MPI_COMM_WORLD);
		}
		escribirMatriz ("matrizResultado", MatrizResultado, SIZE, SIZE);

		//guardar en fichero
		//print momentaneo

	/*	int j;
		for(i = 0; i < SIZE; i++){
			for(j = 0; j < SIZE; j++){
				printf("%f ", MatrizResultado[i][j]);
			}
			printf("\n");
		}*/


		//liberar memoria
		//free(MatrizResultado);

	}
	else
	{
		int i;
		int j;
		MPI_Status status;

		//esclavos recibiendo datos
		printf("Esclavo %d recibiendo datos... \n", rank);

		//Recibir datos:
		//Recibir cabecera de trabajo del maestro (Filas/columnas  e inicio/final de cada matriz)
		params_t datos;
		MPI_Recv(&datos, sizeof(params_t), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status);

		printf("Esclavo %d recibido datos %d %d %d... \n",rank,datos.filaInicio,datos.filaFinal,datos.lado);

		//Reservar espacio para los datos de las matrices
		float** subMatriz1 = (float**)malloc(sizeof(float*) * (datos.filaFinal - datos.filaInicio));
		float** subMatriz2 = (float**)malloc(sizeof(float*) * SIZE);

		//Recibir los datos de las matrices

		for(i = 0; i < (datos.filaFinal - datos.filaInicio); i++){
			subMatriz1[i] = (float*) malloc(sizeof(float) * SIZE);
			MPI_Recv(subMatriz1[i], sizeof(float) * SIZE, MPI_BYTE,0,0, MPI_COMM_WORLD, &status);
		}
		printf("Esclavo %d ha recibido m1\n", rank);

	/*	for(i = 0; i < (datos.filaFinal - datos.filaInicio); i++){
			for(j = 0; j < SIZE; j++){
				printf("%f ", subMatriz1[i][j]);
			}
			printf("\n");
		}*/

		printf("Esclavo %d empiezo a redibir m2 \n", rank);

		for(i = 0; i < SIZE; i++){
			subMatriz2[i] = (float *) malloc(sizeof(float) * SIZE);
			//printf("Esclavo %d recibo %d \n", rank,i);
			MPI_Recv(subMatriz2[i], sizeof(float) * SIZE, MPI_BYTE,0,0, MPI_COMM_WORLD, &status);
			//printf("Esclavo %d recibido %d \n", rank,i);
		}
		printf("Esclavo %d ha recibido m2\n", rank);
		/*for(i = 0; i < SIZE; i++){
				for(j = 0; j < SIZE; j++){
					printf("%f ", subMatriz2[i][j]);
				}
				printf("\n");
			}*/

		//Multiplicar matrices (Reutilizar el código de la sección de multiplicación de matrices en cuda)
		//Reservar matriz resultado
		float** matrizResultado = (float**)malloc(sizeof(float*) * (datos.filaFinal - datos.filaInicio));

		for(i = 0; i < (datos.filaFinal - datos.filaInicio);i++){
			matrizResultado[i]= (float*)malloc(sizeof(float) * SIZE);
		}

		printf("Esclavo %d multiplicando... \n", rank);

		//Multiplicar matrices
		matrizResultado = multiplicarMatrices((datos.filaFinal - datos.filaInicio), SIZE, subMatriz1, subMatriz2);

		printf("Esclavo %d ya ha multiplicado! \n", rank);
		/*for(i = 0; i < SIZE; i++){
					for(j = 0; j < SIZE; j++){
						printf("%f ", matrizResultado[i][j]);
					}
					printf("\n");
				}*/

		//Enviar datos resultado (el master sabe el tamaño de la sub matriz resultado, se pueden enviar los datos directamente)
		//Enviar matriz resultado
		for(i = 0; i < (datos.filaFinal - datos.filaInicio);i++){
			MPI_Send(matrizResultado[i], SIZE * sizeof(float), MPI_BYTE, 0, 0,MPI_COMM_WORLD); //¿?¿? buen size?
		}
		printf("Esclavo %d enviando datos... \n", rank);

		//Esperar mensaje de finalización del maestro (sincronizar)
		//int final;
		//MPI_Recv(final, sizeof(int), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status);

	}

	//Finalización de MPI
	MPI_Finalize();
	return 0;
}
