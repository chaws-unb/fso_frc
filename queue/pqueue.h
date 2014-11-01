#ifndef PQUEUE
#define PQUEUE

/* Node structure, intended to store packe-like structures, such as packages of strings or TCP */
typedef struct _node
{
	int id;
	unsigned char len;
	const char * content;
	struct _node * prev;
	struct _node * next;
} node;

/* Pqueue structure */
typedef struct _pqueue
{
	/* head of the queue */
	node * head;
	
	/* keep the current node, so it's faster if insertion is always growing*/
	node * current;

	/* comparing function, if returns 1, first node has lower priority than second node, move it behind */
	int (* compare)(node *, node *);
} pqueue;

/* Initializes a priority queue */
int pqueue_init(pqueue * pq, int (* compare)(node *, node *));

/* Insert a node into the queue */
int pqueue_insert(pqueue * pq, int id, unsigned char len, const char * content);

/* Cross the entire queue, calling cross_func for every node */
int pqueue_cross(pqueue * pq, void (* cross_func)(int id, unsigned char len, const char * content));

#endif