#include <msg.h>

/**
 *	Executes parent's processing
 */
void parent();

/**
 *	Executes child's processing
 */
void child();

int main()
{
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
			puts("Waiting for child process to begin...");
			sleep(2);
			parent();
	}
	
    return 0;
}

void parent()
{
	message msg; /* Message buffer used to exchange information with child process */
    int msqid;	 /* IDentificator for message queue */
    key_t key;	 /* Key to access message queue */

	/* Create a key  */
	key = ftok("hostA.c", 'A');
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
    
    printf("Entre com a message, ^D para sair:\n");

    /* I have no idea why, but I set type to 1 */
    msg.mtype = 1; 

	printf("[pai]:");
	int len;
    while(fgets(msg.mtext, sizeof(msg.mtext), stdin) != NULL) 
    {
        len = strlen(msg.mtext);

        /* troca a nova linha pelo fechamento de string */
        if (msg.mtext[len-1] == '\n') 
        	msg.mtext[len-1] = '\0';
		
		/* Envia a message pra lista */
        if (msgsnd(msqid, &msg, len+1, 0) == -1) /* +1 for '\0' */
            perror("msgsnd");
            
        sleep(1);
        printf("[pai]:");
    }

    /* Destroys message queue */
    if(msgctl(msqid, IPC_RMID, NULL) == -1)
    { 
    	perror("msgctl"); 
    	exit(1); 
    }
}

/* Executa os processos do Filho */
void child())
{
	int semID, 								/* id do grupo de semáforos */
		shmID, 								/* id do segmento de memória compartilhada */
		msgID;								/* id da fila de mensagens */

	char *shmptr,							/* ponteiro para a memória compartilhada */
		 mem_temporaria[TAMANHO_MESSAGE];	/* armazena o valor da memória compartilhada temporariamente */
	key_t chave_mem,						/* chave única para memória compartilhada */
		  chave_msg;						/* chave única para fila de mensagens */
	union semun semopts;					/* união para operações com semáforos */
	message msg_temporaria;				/* estrutura temporaria para guardar message da fila */
	
	
	struct sembuf UP_A = {0, 1, 0};		/* Desbloqueia A */
	struct sembuf DOWN_A = {0, -1, 0};	/* Bloqueia A */
	struct sembuf UP_B = {1, 1, 0};		/* Desbloqueia B */
	struct sembuf DOWN_B = {1, -1, 0};	/* Bloqueia B */
	
	
	/* Gera as chaves de memória e message*/
	if( (chave_mem = ftok(".",'A')) == -1){ perror("Chave de memória"); exit(1); }
	if( (chave_msg = ftok("hostA.c",'A')) == -1){ perror("Chave de message"); exit(1); }
	printf("[chave_mem=%i], [chave_msg=%i]...\n", chave_mem, chave_msg);
	
	/* Cria um grupo com 2 semáforos e um segmento de memória compartilhada */
	if( (semID = semget(chave_mem, 2, 0600 | IPC_CREAT)) == -1){ perror("ID de semáforo"); exit(1); }
	if( (shmID = shmget(chave_mem, TAMANHO_MESSAGE*sizeof(char), 0600 | IPC_CREAT)) == -1){ perror("ID de memória compartilhada"); exit(1); }
	if( (msgID = msgget(chave_msg, 0644 | IPC_CREAT)) == -1){ perror("ID de message:"); exit(1); }
	printf("[semID=%i], [shmID=%i], [msgID=%i]...\n", semID, shmID, msgID);
	
	/* Inicializar os semáforos (não é necessário pois já estão a 0) */
	semopts.val = 0;
	if( semctl(semID, 0, SETVAL, semopts) == -1){ perror("Controle de semáforo"); exit(1); }
	semopts.val = 0;
	if( semctl(semID, 1, SETVAL, semopts) == -1){ perror("Controle de semáforo"); exit(1); }
	puts("configurações de semáforos prontas...");

	/* Attach */
	if( (shmptr = shmat(shmID, NULL, 0)) == (char *)-1){ perror("Conexão ao segmento de memória"); exit(1); }
	printf("memória compartilhada anexada ao processo [shptr=%p]...\n", shmptr);
    
    printf("[filho]: pronto pra receber mensagens...\n");

	/* O filho termina à comando do pai */
    for(;;) 
    { 
        if (msgrcv(msgID, &msg_temporaria, sizeof(msg_temporaria.mtext), 0, 0) == -1) 
        {
            perror("msgrcv");
            exit(1);
        }
        printf("[filho/enviando message...]: \"%s\"\n", msg_temporaria.mtext);
        
		/* Escrever valores na mem. partilhada */
		strcpy(shmptr, msg_temporaria.mtext);
	
		/* Desbloquear o B */
		if(semop(semID, &UP_B, 1) == -1){ perror("Erro na operação de semáforos"); exit(1); }
	
		if(!strncmp("sair\n", msg_temporaria.mtext, 4))
			break;
		puts("Message enviada!");
    }
    
    /* Detach */
	if(shmdt(shmptr) == -1){ perror("Erro na desanexação"); exit(1);}
}

