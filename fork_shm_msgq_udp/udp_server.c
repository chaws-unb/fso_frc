#include <udp.h>

static int sd;							/* Socket Descriptor */
static struct sockaddr_in server_addr; 	/* Server address */

static void init_udp_server()
{
	/* Trying to create a Datagram Socket*/
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sd < 0) 
	{
		printf("Can't open datagram socket \n");
		exit(1);  
	}

	/* Fill Server information */
	server_addr.sin_family 	  	= AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(IP); 
	server_addr.sin_port 	  	= htons(atoi(PORT));

	/* Trying to bind server to listen to the address:port */
	int rc;
	rc = bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(rc < 0) 
	{
		printf("Can't use port %s, is it in use?\n", PORT);
		exit(1); 
	}
}

int receiveUDP(char * msg, int * length)
{
	/* Just a smart control for intialization */
	static char init = 0;
	if(!init) init_udp_server(), init = 1;

	/* Wait until next package arrives */
	*length = recvfrom(sd, msg, MAX_MSG, 0, NULL, NULL);
	if(*length < 0) 
	{
		printf("Something wrong happened and can't receive any data\n");
		exit(1);
	}
}

// int main(int argc, char *argv[]) 
// {
// 	char buffer[MAX_MSG];
// 	int length;
// 	receiveUDP(buffer, &length);
// 	printf("Received message is '%s', %i bytes\n", buffer, length);
// 	return 0;
// }
