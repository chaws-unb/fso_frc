/* Exercício 2 */

/* libs para io e padrão */
#include <stdio.h>
#include <stdlib.h>

/* libs pra trabalhar com thread */
#include <unistd.h>
#include <pthread.h>

/* lib para usar strcpy() */
#include <string.h>

/* variáveis globais para auxiliar no debugging */
char mensagem[] = "Alo mundo!";
int * retorno_pthread; // em especial, para ser retornado por 'imprime_algo'
*retorno_pthread = 666;

void * imprime_algo(void * msg) {
	printf("Executando uma thread. Argumento recebido: %i\n", *((int *) msg));
	printf("valor do pid filho = %d\n", getpid());
	sleep(3);

	/* copia "TCHAL!!!" em 'mensagem' */
	strcpy(mensagem, "TCHAU!!! ");
//	pthread_exit("Obrigado pelo tempo de CPU\n");

	/* faz o casting de (int *) para (void *) para servir de retorno ao finalizar a thread*/
	pthread_exit((void *)retorno_pthread);
} /* fim da funçao */

int main () {
	int			 i;
	pthread_t	 th;
	void		 *th_res;

	printf("valor do pid pai = %d\n", getpid());
	sleep(3);
	for (i=0;i<5;i++) {
		/* para passar um inteiro, é necessário fazer o casting
		para ponteiro de void e assim passar à função */
		pthread_create(&th, NULL, (void *) imprime_algo, (void *) &i);
	}
	printf("Esperando finalizacao da thread ...\n");
	pthread_join(th, &th_res);
	printf("valor do pid pai = %d\n", getpid());
	sleep(3);
	printf("A thread executada trouxe o seguinte retorno: %i\n", *((int *)th_res));
	printf("Novo valor da variavel mensagem: %s\n", mensagem);
	pthread_exit(0);
} /* fim main */

