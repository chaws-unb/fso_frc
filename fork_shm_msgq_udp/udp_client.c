#include <udp.h>

static int sd; 							/* Socket Descriptor */
static struct sockaddr_in client_addr;  /* Client address */
static struct sockaddr_in server_addr; 	/* Server address */

/**
 *	Set up a udp client sender
 */
static void init_udp_client()
{
	/* Fill Server information */
	server_addr.sin_family 	  	= AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(IP);
	server_addr.sin_port 	   	= htons(atoi(PORT));

	/* Fill Client information */
	client_addr.sin_family 	 	= AF_INET;
	client_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
	client_addr.sin_port 	    = htons(0); /* free to use ports from 1024-5000*/

	/* Trying to create a Datagram Socket*/
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sd < 0) 
	{
		printf("Can't open datagram socket \n");
		exit(1); 
	}

	/* Adding Server and Client information */
	int rc;
	rc = bind(sd, (struct sockaddr *)&client_addr, sizeof(client_addr));
	if(rc < 0) 
	{
		printf("Can't use port %s, is it in use?\n", PORT);
		exit(1); 
	}
}

/**
 *	Send a UDP package
 */
int sendUDP(const char * msg)
{
	/* Just a smart control for intialization */
	static char init = 0;
	if(!init) init_udp_client(), init = 1;

	/* Return Control */ 
	int rc; 
	rc = sendto(sd, msg, strlen(msg), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(rc < 0) 
	{
		printf("Something wrong happened while sending out '%s' \n", msg);
		close(sd);
		exit(1); 
	}
}

// int main(int argc, char *argv[])
// {
// 	sendUDP("lalala");
// 	return 0;
// }
