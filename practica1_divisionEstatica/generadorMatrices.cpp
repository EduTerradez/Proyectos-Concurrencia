#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
//#include <string>
#include <string.h>
#include "generadorMatrices.h"
using namespace std;

void generarMatrizCuadrada(int lado, char* nombreFichero){
	srand (time(NULL));
	ofstream fichero(nombreFichero, std::fstream::out | std::fstream::binary);
	fichero.write((char*)&lado,sizeof(int));
	for(int i = 0; i < lado *lado;i++){
		float aux = ((float)(rand() % 1000) + 1) / 100.0f;
		fichero.write((char*)&aux,sizeof(float));
	}
	fichero.close();
}

void generarMatrizIdentidad(int lado, char* nombreFichero){
	ofstream fichero(nombreFichero, std::fstream::out | std::fstream::binary);
	fichero.write((char*)&lado,sizeof(int));
	float aux ;
	for(int i = 0; i < lado ;i++){
		for(int j = 0; j < lado;j++){

			if(i == j){
				aux = 1.0f;
			}
			else{
				aux = 0.0f;
			}
			fichero.write((char*)&aux,sizeof(float));

		}
	}
	fichero.close();
}

void pasarMatrizFichero(int lado, char* nombreFichero, float** matriz){
	fstream fichero(nombreFichero, std::fstream::out | std::fstream::binary);

	char straux[100];
	//sprintf(char *str, const char *format, ...);

	sprintf(straux, "%d ",lado);
	//fichero.write((char*)&lado,sizeof(int));
	fichero.write(straux,strlen(straux));



	float aux;
	for(int i = 0; i < lado ;i++){
		sprintf(straux, "\n");
		fichero.write(straux,strlen(straux));
			for(int j = 0; j < lado;j++){
				aux = matriz[i][j];

	//			fichero.write((char*)&aux,sizeof(float));
				sprintf(straux, "%f ",aux);
				fichero.write(straux,strlen(straux));
			}
	}
	fichero.close();
}

void imprimirMatriz(char* nombreFichero){
	ifstream fichero(nombreFichero,std::fstream::in | std::fstream::binary);
		int auxiliar;
		fichero.read((char*)&auxiliar,sizeof(int));
		cout << auxiliar << endl;
		for(int i = 0; i < auxiliar;i++){
			for(int j = 0; j < auxiliar;j++){
				float aux;
				fichero.read((char*)&aux,sizeof(float));
				cout << aux << " ";
			}
			cout << endl;
		}
		fichero.close();
}

float** leerMatriz(char* nombreFichero, bool traspuesta){
	ifstream fichero(nombreFichero,std::fstream::in | std::fstream::binary);
		int auxiliar;
		fichero.read((char*)&auxiliar,sizeof(int));

		float** matriz = (float**)malloc(auxiliar*sizeof(float*));

		for(int i = 0; i < auxiliar;i++){
			matriz[i] = (float*)malloc(auxiliar*sizeof(float));
		}

		if(!traspuesta){

			for(int i = 0; i < auxiliar;i++){
				for(int j = 0; j < auxiliar;j++){
					float aux;
					fichero.read((char*)&aux,sizeof(float));
					matriz[i][j] = aux;
				}
			}
		}
		else{
			for(int i = 0; i < auxiliar;i++){
							for(int j = 0; j < auxiliar;j++){
								float aux;
								fichero.read((char*)&aux,sizeof(float));
								matriz[j][i] = aux;
							}
						}

		}
		fichero.close();
		return matriz;
}


