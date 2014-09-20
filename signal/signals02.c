#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int a;

/* Trata o sinal recebido */
void tratahup(int sinal){
	printf("sinal recebido: %i\n", sinal);
	a = 0;
}

void fechar(int sinal){
	puts("salvando os arquivos antes de fechar!");
	exit(0);
}

int main(){
	signal(10, tratahup);
	signal(SIGTERM, fechar);
	printf("PID = %i\n", getpid());
	while(1){
		printf("valor de a = %i\n", a++);
		sleep(3);
	}
}
