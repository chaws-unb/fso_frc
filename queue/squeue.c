#include "squeue.h"
#include <string.h>
#include <stdio.h>

/*
*	Inicia a lista passando como parametro o tamanho da string 
*/
SQueue * 
SQueue_init(int nlen)
{
	SQueue * list;
	list = (SQueue *)malloc(sizeof(SQueue));
	if(!list)
	{
		puts("Sem memoria para a lista!");
		return 0;
	}
	list->nlen = nlen;
	return list;
}

/*
*	Adiciona uma string a lista de mensagens
*/
void 
SQueue_add(SQueue * list, const char * message)
{
	short int empty_list = SQueue_empty(list);
	if(empty_list == -1)
		return;
	
	if(!message)
	{
		puts("Mensagem nula!");
		return;
	}
	
	if(strlen(message) > list->nlen)
	{
		puts("Numero de caracteres da mensagem excede o buffer!");
		return;
	}
	
	SNode * node;
	node = (SNode *)malloc(sizeof(SNode));
	if(!node)
	{
		puts("Sem memoria para o no!");
		return;
	}
	
	/* Copia a mensagem para o nó 
	*  Se chegou até aqui, o tamanho da mensagem é aceitavel
	*/
	char * buffer = (char *)malloc(strlen(message) * sizeof(char));
	strcpy(buffer, message);
	node->value = buffer;

	/* Se a lista não estiver vazia */
	if(empty_list)
	{
		list->tail->next = node;
		list->tail = node;
	} else { /* Se a lista estiver vazia */
		list->head = list->tail = node;
	}
	
	/* Incrementa o número de itens na lista */
	list->size++;
}

/*
*	Pega o primeiro elemento da lista, mas não o remove
*	Retorna 0 se retornou ou -1 se não removeu
*/
short int 
SQueue_first(SQueue * list, char ** message)
{
	if(SQueue_empty(list) != 1)
		return -1;
	
	/* Copia o valor do primeiro nó para a mensagem */
	char * buffer = (char *)malloc(strlen(list->head->value) * sizeof(char));
	puts("lalal-------");
	strcpy(buffer, list->head->value);
	printf("buffer aponta para [%p]%s\n", buffer, buffer);
	*message = buffer;
	
	return 0;
}

/*
*	Remove o primeiro elemento da lista
*	Retorna o número de elementos removidos (0 ou 1)
*/
short int 
SQueue_remove(SQueue * list)
{
	if(SQueue_empty(list) != 1)
		return 0;
		
	if(list->size == 1)
	{
		free(list->head);
		list->head = list->tail = 0;
	} else {
		SNode * temp;
		temp = list->head;
		list->head = temp->next;
		free(temp);
	}
	
	/* Decrementa a quantidade de elementos na lista */
	list->size--;
	
	return 1;
}

/*
*	Verifica se a lista está vazia ou nula
*	Retorna 0 se vazia ou nula, 1 caso contrário
*/
short int 
SQueue_empty(SQueue * list)
{
	if(!list)
	{
		puts("Lista ainda nao iniciada!");
		return -1;
	}
	else if(!list->head)
	{
		puts("Lista vazia!");
		return 0;
	}
		
	return 1;
}

/*
*	Imprime a lista
*/
void SQueue_print(SQueue * list)
{
	if(SQueue_empty(list) != 1)
		return;
	
	SNode * temp;
	temp = list->head;
	int i = list->size;
	while(i--)
	{
		puts(temp->value);
		temp = temp->next;
	}
}

