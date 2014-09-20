#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msg.h>

#define BUFFER_SIZE

int main()
{
	printf("Receiver\n");
	char buffer[BUFFER_SIZE + 1];/* Stores user's messages */
	int chars_read; 			 /* Stores user's typed chars*/
	init_msg();					 /* Initialize parent and child message queue */

	/* Keeps reading until buffer = quit */
	while(1) 
    {
    	printf("Waiting...");
        recv_msg(buffer, &chars_read);

        /* Check if it's time to quit */
        if(!strncmp(buffer, QUIT_MSG, strlen(QUIT_MSG)))
        {
        	printf("Quitting...");
        	break;
        }

        buffer[chars_read] = '\0';
        printf("received: '%s'\n", buffer);
    }
    destroy_msg();
    printf("quit!\n");
	return 0;
}
