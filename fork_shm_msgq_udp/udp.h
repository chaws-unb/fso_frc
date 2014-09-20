#ifndef UDP_H
#define UDP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_MSG 100 	/* Max message length */
#define IP "127.0.0.1" 	/* Server IP */
#define PORT "9001"		/* Server PORT */

int sendUDP(const char * msg);
int receiveUDP(char * msg, int * length);

#endif