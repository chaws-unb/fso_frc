#include <stdio.h>
#include <stdlib.h>
#include <msg.h>

#define BUFFER_SIZE

int main()
{
	printf("Sender\n");

	char buffer[BUFFER_SIZE]; 	/* Stores user's messages */
	int chars_read; 			/* Stores user's typed chars*/
	init_msg();					/* Initialize parent and child message queue */

	/* Keeps sending until EOF (^D, Ctrl+D) */
	while(fgets(buffer, BUFFER_SIZE, stdin) != NULL) 
    {
    	printf("Sending...");
        chars_read = strlen(buffer);
        sendmsg(buffer, chars_read);
        printf("sent!");
    }

    /* If we got here, it's time to end the work */
    sendmsg(QUIT_MSG, strlen(QUIT_MSG));

	return 0;
}
