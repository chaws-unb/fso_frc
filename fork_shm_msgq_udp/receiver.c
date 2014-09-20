#include <msg.h>

/* Executa os processos do Pai */
void pai();

/* Executa os processos do Filho */
void filho();

int main(void)
{
	int pid;
    switch (pid = fork()) {
		case (-1) : 
			perror ("erro ao criar o primeiro filho"); 
			exit(-1);
		
		case (0) :  
			filho();
			exit(0);
			
		default:
			/* espera um tempo até o filho se preparar */
			puts("esperando o filho se preparar...");
			sleep(2);
			pai();
			
	}
	
    return 0;
}

/* Executa os processos do Pai */
void pai()
{
	mensagem msg_temporaria;
    int msqid;
    key_t key;

	/* Cria uma chave única com o nome do arquivo */
    if ((key = ftok("hostB.c", 'A')) == -1) { perror("ftok"); exit(1); }

	/* Conecta/cria uma fila de mensagens do sistema */
    if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) { perror("msgget"); exit(1); }
    
    printf("Mensagens recebidas do hostA:\n");

    msg_temporaria.mtype = 1; /* identificando o tipo de mensagem do meu programa */

    for(;;)
    {
    	if (msgrcv(msqid, &msg_temporaria, sizeof(msg_temporaria.mtext), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        printf("[hostA]: \"%s\"\n", msg_temporaria.mtext);
    }

    if (msgctl(msqid, IPC_RMID, NULL) == -1) { perror("msgctl"); exit(1); }
}

/* Executa os processos do Filho */
void filho()
{
	int semID, 								/* id do grupo de semáforos */
		shmID, 								/* id do segmento de memória compartilhada */
		msgID;								/* id da fila de mensagens */

	char *shmptr,							/* ponteiro para a memória compartilhada */
		 mem_temporaria[TAMANHO_MENSAGEM];	/* armazena o valor da memória compartilhada temporariamente */
	key_t chave_mem,						/* chave única para memória compartilhada */
		  chave_msg;						/* chave única para fila de mensagens */
	union semun semopts;					/* união para operações com semáforos */
	mensagem msg_temporaria;				/* estrutura temporaria para guardar mensagem da fila */
	
	
	struct sembuf UP_A = {0, 1, 0};		/* Desbloqueia A */
	struct sembuf DOWN_A = {0, -1, 0};	/* Bloqueia A */
	struct sembuf UP_B = {1, 1, 0};		/* Desbloqueia B */
	struct sembuf DOWN_B = {1, -1, 0};	/* Bloqueia B */
	
	
	/* Gera as chaves de memória e mensagem*/
	if( (chave_mem = ftok(".",'A')) == -1){ perror("Chave de memória"); exit(1); }
	if( (chave_msg = ftok("hostB.c",'A')) == -1){ perror("Chave de mensagem"); exit(1); }
	printf("[chave_mem=%i], [chave_msg=%i]...\n", chave_mem, chave_msg);
	
	/* Cria um grupo com 2 semáforos e um segmento de memória compartilhada */
	if( (semID = semget(chave_mem, 2, 0600 | IPC_CREAT)) == -1){ perror("ID de semáforo"); exit(1); }
	if( (shmID = shmget(chave_mem, TAMANHO_MENSAGEM*sizeof(char), 0600 | IPC_CREAT)) == -1){ perror("ID de memória compartilhada"); exit(1); }
	if( (msgID = msgget(chave_msg, 0644 | IPC_CREAT)) == -1){ perror("ID de mensagem:"); exit(1); }
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

