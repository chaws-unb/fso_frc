#include <msg.h>
#include <udp.h>

/**
 *	Keeps message queue id
 */
static int msqid;

/**
 *	Executes parent's processing
 */
static void parent();

/**
 *	Executes child's processing
 */
static void child();

/**
 *	Initialize process and message queue
 */
void init_msg()
{
	/* Init udp server */
	init_udp();

	/* Mainly forks parent's process */
	int pid;
    switch (pid = fork()) 
    {
		case -1: 
			perror ("Something wrong happened while forking process"); 
			exit(-1);

		/* This case is not parent process anymore, it's going to be
		   an entire new process that will call 'child()' function and that's it! */
		case 0:  
			child();
			exit(0);
			
		default:
			/* Wait two seconds, enough time time to have child's process ready to work */
			printf("Waiting for child process to begin [PID: %i]...", pid);
			sleep(2);
			parent();
	}
	
    return 0;
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

void parent()
{
    key_t key;	 /* Key to access message queue */

	/* Create a key  */
	key = ftok("msg_receiver.c", 'A');
    if(key == -1) 
    { 
    	perror("Error while creating message key"); 
    	exit(1); 
	}

	/* With the created key, tries connect/create to a message queue */
	msqid = msgget(key, 0644 | IPC_CREAT)
    if(msqid == -1)
    { 
    	perror("Coundn't create or connect to a message queue"); 
    	exit(1); 
    }
}

/* Executa os processos do Filho */
void filho()
{
	int msgID;								/* id da fila de mensagens */
	key_t chave_msg;						/* chave única para fila de mensagens */
	mensagem msg_temporaria;				/* estrutura temporaria para guardar mensagem da fila */
	
	/* Gera as chaves de memória e mensagem*/
	if( (chave_msg = ftok("hostB.c",'A')) == -1){ perror("Chave de mensagem"); exit(1); }
	
	/* Cria um grupo com 2 semáforos e um segmento de memória compartilhada */
	if( (msgID = msgget(chave_msg, 0644 | IPC_CREAT)) == -1){ perror("ID de mensagem:"); exit(1); }
	printf("[semID=%i], [shmID=%i], [msgID=%i]...\n", semID, shmID, msgID);

	/* Attach */
	if( (shmptr = shmat(shmID, NULL, 0)) == (char *)-1){ perror("Conexão ao segmento de memória"); exit(1); }
	printf("memória compartilhada anexada ao processo [shptr=%p]...\n", shmptr);
    
    printf("[filho]: pronto pra receber mensagens do hostA...\n");

	/* O filho termina à comando do pai */
	int len;
	msg_temporaria.mtype = 1;
    for(;;) { 
    	/* Bloqueia este processo até que seja desbloqueado pelo hostA */
    	if(semop(semID, &DOWN_B, 1) == -1){ perror("Erro na operação de semáforos"); exit(1); }
    	
    	/* Escreve o valor da memória para a mensagem de texto */
		strcpy(msg_temporaria.mtext, shmptr);
		
        len = strlen(msg_temporaria.mtext);

        /* troca a nova linha pelo fechamento de string */
        if (msg_temporaria.mtext[len-1] == '\n') 
        	msg_temporaria.mtext[len-1] = '\0';
		
		/* Envia a mensagem pra lista */
        if( msgsnd(msgID, &msg_temporaria, len+1, 0) == -1) /* +1 for '\0' */
            perror("msgsnd");
        
		if(!strcmp("sair\n", msg_temporaria.mtext))
			break;
    }
    
    /* Detach */
	if(shmdt(shmptr) == -1){ perror("Erro na desanexação"); exit(1);}
}

int recvmsg(char * msg, int * length)
{
	static message _msg = {1, NULL}; /* Message buffer used to exchange information with child process */

	/* Wait for the next message in the queue */
	*length = msgrcv(msqid, &_msg, MSG_SIZE, 0, 0);
	if(*length == -1) 
	{
		perror("Error while receive message from the queue");
		exit(1);
	}
	strncpy(msg, _msg.mtext, *length);
	return 0;
}

