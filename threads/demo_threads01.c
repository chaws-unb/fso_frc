/* demo_threads01.c */

/* libs para io e padrão */
#include <stdio.h>
#include <stdlib.h>

/* libs pra trabalhar com thread */
#include <unistd.h>
#include <pthread.h>

/* variáveis globais para auxiliar no debugging */
char mensagem[] = "alo mundo";
int j = 0;

/* imprime o argumento */
void *imprime_msg(void *argumento) {

	/* faz o casting de (void *) para string (char *) */
	printf("Imprimindo mensagem... %s\n", (char *) argumento);

	/* mostra o pid da thread e um valor que diz a ordem de criação da thread */
	printf("PID do filho = %d, j = %i\n", getpid(), j++);
	sleep(10);
	//! pthread_exit();
} /* fim imprime_msg |*/
int main() {
	int 		result, i;

	/* estrutura para guardar dados de uma thread */
	pthread_t	th;


	for (i=0; i<5; i++) {
		/* cria uma thread para executar a função 'imprime_msg' 
		e passa um ponteiro de caracteres como parâmetro */
		pthread_create(&th, NULL, imprime_msg, (void *) mensagem);
	} /* fim - for */

	printf("Esperando finalizacao das threads filhas ... \n");
	printf("Pai (PID= %d) caindo fora ... :-)\n", getpid());
	pthread_exit(NULL);
	return 0;
}/*fim main */

