#include <pqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pqueue_init(pqueue * pq, int (* compare)(node *, node *))
{
	pq->compare = compare;
	pq->current = NULL;
}

int pqueue_insert(pqueue * pq, int id, unsigned char len, const char * content)
{
	node * n;
	n = (node *)malloc(sizeof(node));
	if(!n)
	{
		printf("Impossible to allocate memory!!!\n");
		return -1;
	}

	n->id = id;
	n->len = len;
	n->content = content;
	n->prev = n->next = NULL;

	/* is the queue empty ? */
	if(pq->current == NULL)
	{
		pq->current = pq->head = n;
		return 0;
	}

	/* if it's true, n is lower than current */
	char found = 0;
	node * prev;
	if(pq->compare(n, pq->current))
	{
		while(!found)
		{
			prev = pq->current;
			pq->current = pq->current->next;

			/* nowhere to go */
			if(!pq->current)
			{
				pq->current = n; /* current now is n */
				prev->next = pq->current; /* the next of the previous is the current */
				pq->current->prev = prev; /* the previous of the current is the previous */
				found = 1;
			}

			/* yeah, current is lower priority */
			if(!pq->compare(n, pq->current))
			{
				pq->current->prev->next = n; /* the next of the previous one is n */
				n->prev = pq->current->prev; /* previous of n is the previous of current*/

				n->next = pq->current; /* the next of n is the current */
				pq->current->prev = n; /* the previous of current is n */

				found = 1;
			}
		}
	}
	else
	{
		while(!found)
		{
			prev = pq->current;
			pq->current = pq->current->prev;

			/* nowhere to go */
			if(!pq->current)
			{
				pq->current = n; /* current now is n */
				prev->prev = pq->current; /* the next of the previous is the current */
				pq->current->next = prev; /* the previous of the current is the previous */
				pq->head = n;
				found = 1;
			}

			/* yeah, current is higher priority */
			if(pq->compare(n, pq->current))
			{
				pq->current->next->prev = n; /* the previous of the next one is n */
				n->next = pq->current->next; /* next of n is the next of current*/

				n->prev = pq->current; /* the previous of n is the current */
				pq->current->next = n; /* the next of current is n */
				
				found = 1;
			}
		}
	}

	return 0;
}

int pqueue_cross(pqueue * pq, void (* cross_func)(int id, unsigned char len, const char * content))
{
	if(!pq->head)
	{
		printf("Empty queue!\n");
		return -1;
	}

	/* find the first node */
	node * n;
	n = pq->head;
	do
	{
		cross_func(n->id, n->len, n->content);
		n = n->next;
	}
	while(n);
	return 0;
}