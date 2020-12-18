#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#define N 20

int contPostIt=0;
int last_cont=0; 
sem_t S1;
sem_t S2;

void ColaPostIt(){
	
}

void leva_mochila_ate_B_e_volta(){
	int a;
	srand(time(NULL));
	a = 1000+rand()%3000;
	sleep(a);
}
void dorme_aleatorio(){
	int a;
	srand(time(NULL));
	a = 1000+rand()%3000;
	sleep(a);
}

void* usuario() {
 	while(1){
 		sem_wait(&S1);
		dorme_aleatorio();
		
		ColaPostIt();
		contPostIt++;
		
		if (contPostIt == N)
		{
			printf("mochila cheia\n");
			
		}
		sem_post(&S2);
 	}
}

void* pombo(){
	while(1){
		
		leva_mochila_ate_B_e_volta();
		contPostIt = 0;
		int i;
		for (i=0; i<N; i++)
		{
			sem_post(&S1);
			printf("Adic. msg %d\n", i);
			sem_wait(&S2);
		}
	}
}

int main (int argc, char *argv[])
{
	int users;
	
	printf("Informe o numero de Usuarios: ");
	scanf("%d", &users);
	
	sem_init(&S1, 0, 0);
	sem_init(&S2, 0, 0);
	
	int i, rc;
	
	pthread_t thread_usuarios[users];
	pthread_t thread_pombo;
	

	
	for(i=0;i<users;i++){
		rc = pthread_create(thread_usuarios, NULL, usuario, NULL);
   		if(rc){
   			printf("Erro ao criar Thread.");
   			exit(-1);
		}
	}
	
	rc = pthread_create(&thread_pombo, NULL, pombo, NULL);
  	if(rc){
  		printf("Erro ao criar Thread.");
  		exit(-1);
  	}
	
	while(1){
		
		
	}
   
   return(0);
}
