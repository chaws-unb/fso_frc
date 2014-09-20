#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/* Trata o sinal recebido */
void tratasinal(int sinal){
	printf("\n\n********** Processo Chuck Norris *********\n");
	printf("[PID=%i]sinal recebido: %i\n\n", getpid(), sinal);
	sleep(1);
}

int main(){

	int s = 0;
	int lele;
	int * ponteirodoido;

	/* Utiliza a mesma função para tratar os sinais de 1 a 35 */
	while(++s < 35)
		signal(s, tratasinal);

	// Força uma segmentation fault
	ponteirodoido = 0x10001111;
	lele = *ponteirodoido;
	
	// Força uma divisão por 0
	int coiso = lele / 0;

	printf("PID = %i\n", getpid());
	while(1){
		printf("[PID=%i]...\n", getpid());
		sleep(3);
	}
}
