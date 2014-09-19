#include <stdio.h>
#include <stdlib.h>
#include "squeue.h"

#define STRING_LEN 128

int main(){

	SQueue * mensagens;
	mensagens = SQueue_init(STRING_LEN);
	SQueue_add(mensagens, "olá mundo!");
	SQueue_add(mensagens, "Mensagem dois!");
	
	char * msg = 0;
	SQueue_first(mensagens, &msg);
	printf("msg aponta para[%p]%s\n", msg, msg);
	msg = "lalalala";
	
	printf("msg %s\n", msg);
	puts("imprimindo mensagens 1:");
	SQueue_print(mensagens);
	puts("imprimindo mensagens 2:");
	SQueue_remove(mensagens);
	SQueue_print(mensagens);
	puts("imprimindo mensagens 3:");
	SQueue_remove(mensagens);
	SQueue_print(mensagens);
	SQueue_add(mensagens, "Mensagem tres!");
	puts("imprimindo mensagens 4:");
	SQueue_print(mensagens);
	
	return 0;
}
