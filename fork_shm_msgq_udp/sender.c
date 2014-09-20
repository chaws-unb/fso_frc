#include <stdio.h>
#include <stdlib.h>
#include <msg.h>

#define BUFFER_SIZE UDP_MAX_MSG

int main()
{
	printf("Sender\n");

	char buffer[BUFFER_SIZE]; 	/* Stores user's messages */
	int chars_read; 			/* Stores user's typed chars*/
	init_msg();					/* Initialize parent and child message queue */

	/* Keeps sending until EOF (^D, Ctrl+D) */
    printf("msg ['%s' to exit]> ", QUIT_MSG);
	while(fgets(buffer, BUFFER_SIZE, stdin) != NULL) 
    {
    	printf("Sending...");
        chars_read = strlen(buffer) - 1; /* take out \n */
        send_msg(buffer, chars_read);
        printf("sent!\n");

        /* Check if it's time to quit */
        if(!strncmp(buffer, QUIT_MSG, strlen(QUIT_MSG)))
        {
        	printf("Quitting...");
        	break;
        }

        printf("msg ['%s' to exit]> ", QUIT_MSG);
    }

    /* If we got here, it's time to end the work */
    //send_msg(QUIT_MSG, strlen(QUIT_MSG));
    destroy_msg();
    printf("quit!\n");

	return 0;
}
