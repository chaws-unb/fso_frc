#include <pqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_sample_implementation(node * n1, node * n2)
{
	return (n1->id > n2->id ? 1 : 0);
}

void print_node(int id, unsigned char len, const char * content)
{
	static char buffer[255];
	strncpy(buffer, content, len);
	buffer[len] = 0;
	printf("%s", buffer);
}

int main()
{
	pqueue pq;
	const char * str1 = "Hello", * str2 = " this", * str3 = " is", * str4 = " cool!\n";

	/* Init the queue */
	pqueue_init(&pq, &compare_sample_implementation);

	/* Insert some elements, the id matters more than order of insertion */
	pqueue_insert(&pq, 4, strlen(str4), str4);
	pqueue_insert(&pq, 3, strlen(str3), str3);
	pqueue_insert(&pq, 1, strlen(str1), str1);
	pqueue_insert(&pq, 2, strlen(str2), str2);

	printf("Sucess!\n");

	/* Go through all of them printing them out */
	pqueue_cross(&pq, &print_node);

	return 0;
}