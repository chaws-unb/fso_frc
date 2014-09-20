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

#define UDP_MAX_MSG 100 /* Max message length */
#define IP "127.0.0.1" 	/* Server IP */
#define PORT "9002"		/* Server PORT */

void init_udp();
int send_udp(const char * msg, int length);
int recv_udp(char * msg, int * length);

#endif