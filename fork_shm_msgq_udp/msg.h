#ifndef MSG_H
#define MSG_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <udp.h>

#define MSG_SIZE UDP_MAX_MSG
#define QUIT_MSG "!!!quit!!!"

/**
 *	Message struct to be passed through message queue
 */
typedef struct _message 
{
    long mtype;
    char mtext[MSG_SIZE + 1];
} message;

void init_msg();
void destroy_msg();
int send_msg(const char * msg, int length);
int recv_msg(char * msg, int * length);

#endif
