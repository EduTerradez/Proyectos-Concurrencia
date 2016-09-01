#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>


void generarMatrizCuadrada(int lado, char* nombreFichero);

void generarMatrizIdentidad(int lado, char* nombreFichero);

void imprimirMatriz(char* nombreFichero);

float** leerMatriz(char* nombreFichero, bool traspuesta);

void pasarMatrizFichero(int lado, char* nombreFichero, float** matriz);
