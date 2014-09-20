#ifndef MSG_H
#define MSG_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define MSG_SIZE 128
#define QUIT_MSG "quit"

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
int sendmsg(const char * msg, int length);
int recvmsg(char * msg, int * length);

#endif
