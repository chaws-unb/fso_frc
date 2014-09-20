#include <msg.h>

#define KEY_STR "msg_sender.c"

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
		printf("msg_sender: Initiating\n");
	#endif

    key_t key;	 /* Key to access message queue */

	/* Create a key  */
	key = ftok(KEY_STR, 'A');
    if(key == -1) 
    { 
    	perror("msg_sender: Error while creating message key"); 
    	exit(1); 
	}
	#ifdef PRINT_DEBUG
		printf("msg_sender: message key created: %i\n", key);
	#endif

	/* With the created key, tries connect/create to a message queue */
	msqid = msgget(key, 0644 | IPC_CREAT);
    if(msqid == -1)
    { 
    	perror("msg_sender: Coundn't create or connect to a message queue"); 
    	exit(1); 
    }
    #ifdef PRINT_DEBUG
    	printf("msg_sender: message id created: %i\n", msqid);
    #endif
}

/**
 *	Executes child's processing
 */
static void * child(void * params)
{
	#ifdef PRINT_DEBUG
		printf("msg_sender-child: Initiating child thread\n");
	#endif

	int chars_read;	/* Number of read chars when receiving them from udp */
	int msgid;		/* message id, I'm not confident to use the static one */
	key_t key;		/* Key to access message queue */
	message msg;	/* Message buffer used to exchange information with child process */
	
	/* Create a key  */
	key = ftok(KEY_STR,'A');
	if(key == -1)
	{ 
		perror("msg_sender-child: Error while creating message key");
		exit(1); 
	}
	#ifdef PRINT_DEBUG
		printf("msg_sender-child: message key created: %i\n", key);
	#endif
	
	/* With the created key, tries connect/create to a message queue */
	msgid = msgget(key, 0644 | IPC_CREAT);
	if(msqid == -1)
	{ 
		perror("msg_sender-child: Coundn't create or connect to a message queue"); 
		exit(1); 
	}
	#ifdef PRINT_DEBUG
		printf("msg_sender-child: message id created: %i\n", msgid);
	#endif

	/* Keeps listening until msg is to quit */
	msg.mtype = 1;
    while(1) 
    { 
    	chars_read = msgrcv(msgid, &msg, MSG_SIZE, 0, 0);
        if(chars_read == -1) 
        {
            perror("msg_sender-child: Error while receive message from queue");
            exit(1);
        }
        
		/* Send it through udp */
		send_udp(msg.mtext, chars_read);

		/* Check if it's time to quit */
		if(!strncmp(QUIT_MSG, msg.mtext, strlen(QUIT_MSG)))
		{
			break;
		}
    }
}

/**
 *	Initialize process and message queue
 */
void init_msg()
{
	/* Init udp client */
	init_udp();

	/* Start the child as a thread */
	pthread_create(&child_th, NULL, child, NULL);
	
	/* Now, let's parent do its work */	
	parent();
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

int send_msg(const char * msg, int length)
{
    /* Message buffer used to exchange information with child process */
    static message _msg = {1, "0"}; 

	/* Add msg to the queue */
	strncpy(_msg.mtext, msg, length);
	if(msgsnd(msqid, &_msg, length, 0) == -1)
	{
		perror("msg_sender: Error while writing message to queue");
		exit(1);
	}
	return 0;
}
