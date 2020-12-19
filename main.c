#include <pthread.h> //biblioteca necess�ria para utilizar threads
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h> //biblioteca para o uso de sem�foros
#include <time.h> //biblioteca para usar o tempo, no nosso caso usaremos isto para a fun��o srand()

#define N 20 //n�mero m�ximo de mensagens do pombo

/*
	Ol�! Esta � a tarefa de programa��o da equipe EzequiLeo. 
	Para compilar o programa, use gcc nomedoarquivo nomedoprograma -o -lpthread
	Para parar a execu��o, basta inserir qualquer caractere e dar um enter.

*/

int contPostIt=0; //vari�vel global que controlar� o n�mero de post-its colados

sem_t S1; //Este primeiro sem�foro ser� usado para controlar as threads de usu�rios
sem_t S2; //Este segundo sem�foro servir� para controlar a thread pombo!

void ColaPostIt(){ //apenas uma fun��o para mostrar que o post-it foi colado no pombo
	printf("PostIt Colado!\n");
}

void leva_mochila_ate_B_e_volta(){ //essa fun��o serve para o pombo levar as mensagens e voltar, levando de 1 a 3 segundos para fazer isso, aleatoriamente.
	printf("Levando Mensagens...\n");
	int a;
	srand(time(NULL)); //aqui definimos que a base para calcularmos o n�mero aleat�rio ser� o n�mero de segundos atual.
	a = 1+rand()%3; //aqui, a fun��o rand() vai sortear um n�mero aleat�rio at� o n�mero 3, e adicionamos 1 para o tempo variar de 1 a 3 segundos.
	sleep(a); //aqui, fazemos a thread "dormir" por 'a' segundos.
	
}
void dorme_aleatorio(){ //essa fun��o faz o usu�rio dormir por alguns segungos.
	int a;
	srand(time(NULL)); //aqui definimos que a base para calcularmos o n�mero aleat�rio ser� o n�mero de segundos atual.
	a = 1+rand()%3; //aqui, a fun��o rand() vai sortear um n�mero aleat�rio at� o n�mero 3, e adicionamos 1 para o tempo variar de 1 a 3 segundos.
	sleep(a); //aqui, fazemos a thread "dormir" por 'a' segundos.
}

void* usuario() { 
	//Esta � a nossa fun��o do usu�rio, a mesma do enunciado, apenas adicionado as premissas dos sem�foros.
	//Nessa fun��o, o usu�rio, assim que desbloqueado pelo sem�foro, colar� um post-it e liberar� o sem�foro do pombo, para que o ato de colar seja s�ncrono ao do pombo. 
	
 	while(1){
 		sem_wait(&S1); //A fun��o sem_wait() decrementa o sem�foro, bloqueando o usu�rio caso o valor fique menor que 0. Apenas a fun��o Pombo pode liberar o sem�foro do usu�rio.
 		
		dorme_aleatorio();
		ColaPostIt();
		contPostIt++;
		if (contPostIt == N)
		{
			printf("mochila cheia\n");
			
		}
		sem_post(&S2); //A fun��o sem_post() incrementa o sem�foro do pombo, liberando para que ele atue mais uma vez.
 	}
}

void* pombo(){
	//Esta � a fun��o Pombo, tal como no enunciado, com apenas as premissas dos sem�foros adicionadas.
	
	
	while(1){
		
		leva_mochila_ate_B_e_volta(); //Essa fun��o faz o Pombo dormir por alguns segundos.
		contPostIt = 0; //j� que o pombo j� enviou as mensagens, zeramos o contador aqui.
		int i;
		for (i=0; i<N; i++)
		{
			sem_post(&S1); //Aqui, o sem�foro vai liberar para que uma �nica thread usu�rio execute a fun��o usuario.
			printf("Adic. msg %d\n", i);
			sem_wait(&S2); //Ap�s colar a mensagem, pausamos a thread pombo, que ser� liberada apenas por um usu�rio.
		}
	}
}

int main (int argc, char *argv[])
{
	int users; //Este ser� o n�mero de usu�rios definido pelo usu�rio.
	
	printf("Informe o numero de Usuarios: ");
	scanf("%d", &users);
	
	sem_init(&S1, 0, 0); //aqui, iniciamos o sem�foro dos usu�rios valendo 0.
	sem_init(&S2, 0, 0);// aqui, iniciamos o sem�foro do pombo valendo 0.
	
	int i, rc;
	
	pthread_t thread_usuarios[users]; //aqui, declaramos o n�mero de usu�rios conforme pedido.
	pthread_t thread_pombo; //aqui, declaramos a thread pombo.
	
	//as threads ainda n�o foram inicializadas, apenas instanciadas.
	
	for(i=0;i<users;i++){
		rc = pthread_create(thread_usuarios, NULL, usuario, NULL); //nesta linha, a cada rodada do for, instanciamos uma thread usu�rio
   		if(rc){
   			printf("Erro ao criar Thread.");
   			exit(-1);
		}
	}
	
	rc = pthread_create(&thread_pombo, NULL, pombo, NULL); //aqui, de fato, iniciamos a thread pombo.
  	if(rc){
  		printf("Erro ao criar Thread.");
  		exit(-1);
  	}	
	
	//Nosso programa roda ininterruptamente, com in�meras coladas de post-its e in�meras idas do pombo ao local B
	
	//Utilizaremos a t�cnica a seguir para parar o programa quando for digitado qualquer tecla.
	int stop; //usamos o tipo int por ser mais f�cil de tratar. 
	scanf("%d", stop); //assim que receber o caractere, mesmo que n�o seja um int, o programa vai ser interrompido
   
   return(0);
}
