#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/* Trata o sinal recebido */
void trata2(int sinal){
	printf("Processo abortado em funcao de timeout\n");
	raise(15);
	printf("sinal recebido: %i\n\n", sinal);
}

void trata1(int sinal){
	printf("Tempo esgotado....aguardando nova digitacao em 4s\n");
	signal(SIGALRM, trata2);
	alarm(4);
}

int main(){
	
	int a = 0;
	signal(SIGALRM, trata1);
	
	printf("Meu pid eh %i\n", getpid());
	alarm(7); // envia o sinal SIGALRM que será tratado por trata1 em 7 segundos
	puts("Digite um numero");
	scanf("%i", &a);
	alarm(0);// se o numero for digitado em menos de 7 segundos, desativa o alarme
	puts("Thanks!");
	
	return 0;
}
