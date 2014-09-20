#include <msg.h>

#define KEY_STR "msg_receiver.c"

/**
 *	Keeps message queue id
 */
static int msqid;

/**
 *	Stores child as a thread
 */
static pthread_t child_th;

/**
 *	Executes parent's processing
 */
static void parent()
{
	#ifdef PRINT_DEBUG
		printf("msg_receiver: Initiating\n");
	#endif

    key_t key;	 /* Key to access message queue */

	/* Create a key  */
	key = ftok(KEY_STR, 'A');
    if(key == -1) 
    { 
    	perror("msg_receiver: Error while creating message key"); 
    	exit(1); 
	}
	#ifdef PRINT_DEBUG
		printf("msg_receiver: message key created: %i\n", key);
	#endif

	/* With the created key, tries connect/create to a message queue */
	msqid = msgget(key, 0644 | IPC_CREAT);
    if(msqid == -1)
    { 
    	perror("msg_receiver: Coundn't create or connect to a message queue"); 
    	exit(1); 
    }
    #ifdef PRINT_DEBUG
    	printf("msg_receiver: message id created: %i\n", msqid);
    #endif
}

/**
 *	Executes child's processing
 */
static void * child(void * params)
{
	#ifdef PRINT_DEBUG
		printf("msg_receiver-child: Initiating child thread\n");
	#endif

	int chars_read;	/* Number of read chars when receiving them from udp */
	int msgid;		/* message id, I'm not confident to use the static one */
	key_t key;		/* Key to access message queue */
	message msg;	/* Message buffer used to exchange information with child process */
	
	/* Create a key  */
	key = ftok(KEY_STR,'A');
	if(key == -1)
	{ 
		perror("msg_receiver-child: Error while creating message key");
		exit(1); 
	}
	#ifdef PRINT_DEBUG
		printf("msg_receiver-child: message key created: %i\n", key);
	#endif
	
	/* With the created key, tries connect/create to a message queue */
	msgid = msgget(key, 0644 | IPC_CREAT);
	if(msqid == -1)
	{ 
		perror("msg_receiver-child: Coundn't create or connect to a message queue"); 
		exit(1); 
	}
	#ifdef PRINT_DEBUG
		printf("msg_receiver-child: message id created: %i\n", msgid);
	#endif

	/* Keeps listening until msg is to quit */
	msg.mtype = 1;
    while(1) 
    { 
    	/* Keep waiting for udp packages */
    	recv_udp(msg.mtext, &chars_read);
    	msg.mtext[chars_read] = '\0';
		
		/* Send it to parent */
        if(msgsnd(msgid, &msg, chars_read, 0) == -1)
        {
            perror("msg_receiver-child: Error while adding message in queue");
            exit(1);
        }
        
        /* Check if it's time to quit */
		// if(!strncmp(QUIT_MSG, msg.mtext, strlen(QUIT_MSG)))
		// {
		// 	break;
		// }
    }
}

/**
 *	Initialize process and message queue
 */
void init_msg()
{
	#ifdef PRINT_DEBUG
		printf("Initiating msg_receiver\n");
	#endif

	/* Init udp server */
	init_udp();
	
	/* Now, let's parent do its work */	
	parent();

	/* Start the child as a thread */
	pthread_create(&child_th, NULL, child, NULL);
}

void destroy_msg()
{
	/* Destroys message queue */
    if(msgctl(msqid, IPC_RMID, NULL) == -1)
    { 
    	perror("msgctl"); 
    	exit(1); 
    }
}

int recv_msg(char * msg, int * length)
{
	#ifdef PRINT_DEBUG
		printf("msg_receiver: ready to wait for message queue\n");
	#endif

	/* Message buffer used to exchange information with child process */
	static message _msg = {1, "0"}; 

	/* Wait for the next message in the queue */
	*length = msgrcv(msqid, &_msg, MSG_SIZE, 0, 0);
	if(*length == -1) 
	{
		perror("Error while receive message from queue");
		exit(1);
	}
	strncpy(msg, _msg.mtext, *length);
	return 0;
}

