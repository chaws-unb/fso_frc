#ifndef SNODE_H
#define SNODE_H

/*
*	Um nó de lista encadeada que armazena uma string
*/
typedef struct _SNode{
	char * value;
	struct _SNode * next;
} SNode;

#endif
