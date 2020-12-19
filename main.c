#include <pthread.h> //biblioteca necessária para utilizar threads
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h> //biblioteca para o uso de semáforos
#include <time.h> //biblioteca para usar o tempo, no nosso caso usaremos isto para a função srand()

#define N 20 //número máximo de mensagens do pombo

/*
	Olá! Esta é a tarefa de programação da equipe EzequiLeo. 
	Para compilar o programa, use gcc nomedoarquivo nomedoprograma -o -lpthread
	Para parar a execução, basta inserir qualquer caractere e dar um enter.

*/

int contPostIt=0; //variável global que controlará o número de post-its colados

sem_t S1; //Este primeiro semáforo será usado para controlar as threads de usuários
sem_t S2; //Este segundo semáforo servirá para controlar a thread pombo!

void ColaPostIt(){ //apenas uma função para mostrar que o post-it foi colado no pombo
	printf("PostIt Colado!\n");
}

void leva_mochila_ate_B_e_volta(){ //essa função serve para o pombo levar as mensagens e voltar, levando de 1 a 3 segundos para fazer isso, aleatoriamente.
	printf("Levando Mensagens...\n");
	int a;
	srand(time(NULL)); //aqui definimos que a base para calcularmos o número aleatório será o número de segundos atual.
	a = 1+rand()%3; //aqui, a função rand() vai sortear um número aleatório até o número 3, e adicionamos 1 para o tempo variar de 1 a 3 segundos.
	sleep(a); //aqui, fazemos a thread "dormir" por 'a' segundos.
	
}
void dorme_aleatorio(){ //essa função faz o usuário dormir por alguns segungos.
	int a;
	srand(time(NULL)); //aqui definimos que a base para calcularmos o número aleatório será o número de segundos atual.
	a = 1+rand()%3; //aqui, a função rand() vai sortear um número aleatório até o número 3, e adicionamos 1 para o tempo variar de 1 a 3 segundos.
	sleep(a); //aqui, fazemos a thread "dormir" por 'a' segundos.
}

void* usuario() { 
	//Esta é a nossa função do usuário, a mesma do enunciado, apenas adicionado as premissas dos semáforos.
	//Nessa função, o usuário, assim que desbloqueado pelo semáforo, colará um post-it e liberará o semáforo do pombo, para que o ato de colar seja síncrono ao do pombo. 
	
 	while(1){
 		sem_wait(&S1); //A função sem_wait() decrementa o semáforo, bloqueando o usuário caso o valor fique menor que 0. Apenas a função Pombo pode liberar o semáforo do usuário.
 		
		dorme_aleatorio();
		ColaPostIt();
		contPostIt++;
		if (contPostIt == N)
		{
			printf("mochila cheia\n");
			
		}
		sem_post(&S2); //A função sem_post() incrementa o semáforo do pombo, liberando para que ele atue mais uma vez.
 	}
}

void* pombo(){
	//Esta é a função Pombo, tal como no enunciado, com apenas as premissas dos semáforos adicionadas.
	
	
	while(1){
		
		leva_mochila_ate_B_e_volta(); //Essa função faz o Pombo dormir por alguns segundos.
		contPostIt = 0; //já que o pombo já enviou as mensagens, zeramos o contador aqui.
		int i;
		for (i=0; i<N; i++)
		{
			sem_post(&S1); //Aqui, o semáforo vai liberar para que uma única thread usuário execute a função usuario.
			printf("Adic. msg %d\n", i);
			sem_wait(&S2); //Após colar a mensagem, pausamos a thread pombo, que será liberada apenas por um usuário.
		}
	}
}

int main (int argc, char *argv[])
{
	int users; //Este será o número de usuários definido pelo usuário.
	
	printf("Informe o numero de Usuarios: ");
	scanf("%d", &users);
	
	sem_init(&S1, 0, 0); //aqui, iniciamos o semáforo dos usuários valendo 0.
	sem_init(&S2, 0, 0);// aqui, iniciamos o semáforo do pombo valendo 0.
	
	int i, rc;
	
	pthread_t thread_usuarios[users]; //aqui, declaramos o número de usuários conforme pedido.
	pthread_t thread_pombo; //aqui, declaramos a thread pombo.
	
	//as threads ainda não foram inicializadas, apenas instanciadas.
	
	for(i=0;i<users;i++){
		rc = pthread_create(thread_usuarios, NULL, usuario, NULL); //nesta linha, a cada rodada do for, instanciamos uma thread usuário
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
	
	//Nosso programa roda ininterruptamente, com inúmeras coladas de post-its e inúmeras idas do pombo ao local B
	
	//Utilizaremos a técnica a seguir para parar o programa quando for digitado qualquer tecla.
	int stop; //usamos o tipo int por ser mais fácil de tratar. 
	scanf("%d", stop); //assim que receber o caractere, mesmo que não seja um int, o programa vai ser interrompido
   
   return(0);
}
