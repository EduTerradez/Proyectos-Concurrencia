#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct params_t {
	char* buffer;
	int index;
	int i;

}params_t;

pthread_mutex_t cerrojo;

void* copiamensaje(void* params){
	params_t* global_param = (params_t *)params;


	char msg[100];
	msg[0] = '\0';
	sprintf(msg, "Hola desde el hilo %d\n", global_param->i);



	pthread_mutex_lock(&cerrojo);

	for(int j = 0; j <21;j++){
		global_param->buffer[global_param->index] = msg[j];
		global_param->index++;
		}

	pthread_mutex_unlock(&cerrojo);


	int* retorno = new int[1];
	retorno[0]= 10;
	pthread_exit(retorno);
}

int main(int argc,  char** argv){
	params_t * params = new params_t[10];
	char* buffer = new char[1000];
	pthread_t thread_id[10];

	pthread_mutex_init(&cerrojo, NULL);



	for(int i = 0; i < 10; i++){
		params[i].i = i;
		params[i].buffer = buffer;
		params[i].index = 21*i;
		pthread_create(&thread_id[i], NULL, copiamensaje, &params[i]);

	}

	for(int i= 0; i < 10;i++)
	{
		void* result;
		pthread_join(thread_id[i], &result);
		printf("thread finalizado: %d,valor devuelto %d\n", i, *((int *)result));
	}
	printf("%s", params->buffer);
	delete params->buffer;
	delete params;
	pthread_mutex_destroy(&cerrojo);
	return 0;
}
