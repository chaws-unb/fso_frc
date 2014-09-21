#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(void)
{
	int pid;
	printf("Iniciando a parada com pid: \n");
	FILE * f;
	char buff[64];

    switch (pid = fork()) {
		case (-1) : 
			perror ("erro ao criar o primeiro filho"); 
			exit(-1);
		
		case (0) :  
			// while(1)
			// {
			// 	scanf("%s", buff);
			// 	if()
			// }
			printf("Filho abrindo o arquivo pra escrever\n");
			f = fopen("test.txt", "w");
			if(f == NULL)
				printf("Filho Deu erro pra abrir o arquivo...\n");
			fprintf(f, "Testing...\n");
			fflush(f);
			printf("O filho escreveu: Testing...\n");
			printf("Esperando 4s pro pai ler\n");
			sleep(4);
			fclose(f);
			exit(0);
			
		default:
			/* espera um tempo até o filho se preparar */
			puts("Esperando o filho se preparar...");
			sleep(2);
			//pai();
			printf("Sou o pai\n");
			// while(1)
			// {
			// 	scanf("%s", buff);
			// 	if()
			// }
			printf("Pai abrindo o arquivo pra ler\n");
			f = fopen("test.txt", "r");
			if(f == NULL)
				printf("Pai Deu erro pra abrir o arquivo...\n");
			fscanf(f, "%s\n", buff);
			printf("O pai leu: %s\n", buff);
			fclose(f);
	}
    return 0;
}