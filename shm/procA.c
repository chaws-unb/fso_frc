#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <sys/sem.h>

/* make it a 1K shared memory segment */
#define SHM_SIZE 1024  

/* Define o numero de semaforos a serem inicializados */
#define NUMERO_SEMAFOROS 2

/* Define o máximo de tentativas de desbloquear o semáforo */
#define MAX_RETRIES 10

/* Definir semun caso não esteja definida */
#ifdef _SEM_SEMUN_UNDEFINED
#undef _SEM_SEMUN_UNDEFINED
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};
#endif

/* Definir as operações sobre os semáforos */
struct sembuf UP_A = {0, 1, 0};
struct sembuf DOWN_A = {0, -1, 0};
struct sembuf UP_B = {1, 1, 0};
struct sembuf DOWN_B = {1, -1, 0};

/* Chave única usada pra gerar os ids pra memória compartilhada e semáforos */
key_t key = -1;

/* Representa a identificação do grupo de semáforos (no caso deste simples e trabalhoso programa) 
	só conterá um semaforo dentro do grupo...é a vida
*/
int grupo_semaforo_id = -1;

/* Executa uma operação com o grupo de semáforos do programa
*	operacao = -1, fecha o semáforo para todos os processos conecatos ao semáforo deste programa
*	operacao = 1, abre o semáforo para os outros processos, em ordem de chegada
*/
void operacao_semaforo(int operacao);

/* Pega o nome de um arquivo e gera a chave única key*/
void get_key(const char * arquivo);

/* Inicia o semáforo 
*	Retorna 0 em caso de sucesso, -1 em caso de falha
*/
int initsem();

/* Inicia a memória compartilhada 
*	Retorna o ponteiro para o endereço de memória compartilhada
*/
char * initshm();

/* Finaliza a memória compartilhada */
void endshm(char * data);

/* Vamos brincar! */
int main()
{	
	/* Tudo começa aqui...gera-se uma chave única a partir do nome do arquivo abaixo */
	get_key("arquivo_compartilhado");
	
	/* Inicia o grupo de semáforos */
	if(initsem() < 0)
	{
		perror("Erro na inicializacao de semaforos");
		exit(1);
	}
	
	/* Inicia a memória compartilhada */
    char * memoria_compartilhada = initshm();
    
    /* buffer, ou variavel temporaria para ler do teclado */
    char buffer_entrada[SHM_SIZE];
    
    /* lê do teclado até que seja digitado ':q' */
	for(;;){
		
		/* Bloqueia os outros processos que estejam conectados a esse semáforo */
		//operacao_semaforo(-1);
		
        printf("[escreva]: ");
        
        fgets(buffer_entrada, SHM_SIZE, stdin);
        
        /* Verifica condição de saída */
        if(!strcmp(buffer_entrada, "sair\n"))
        {
        	puts("Saindo!");
        	strncpy(memoria_compartilhada, buffer_entrada, SHM_SIZE);
        	operacao_semaforo(-1);
        	break;
        }
        
        /* Copia a entrada do teclado para a memória compartilhada */
        strncpy(memoria_compartilhada, buffer_entrada, SHM_SIZE);
        
        /* Bloqueia os outros processos que estejam conectados a esse semáforo */
		//operacao_semaforo(-1);
    } 

	/* Desanexa a memória */
	endshm(memoria_compartilhada);
	
    return 0;
}

char * initshm()
{
	if(key == -1)
	{
		puts("A chave deve ser gerada primeiro!");
		exit(1);
	}
	
    int shmid;
    char *data;

    /* Conecta(ou cria) uma área de memória compartilhada */
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    /* associa/anexa a memória a um ponteiro */
    data = shmat(shmid, (void *)0, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }
    return data;
}

void endshm(char * data)
{
	if(key == -1)
	{
		puts("A chave deve ser gerada primeiro!");
		exit(1);
	}

    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }
}

int initsem()
{
	if(key == -1)
	{
		puts("A chave deve ser gerada primeiro!");
		exit(1);
	}

	printf("Iniciando o trabalho com semaforos...\n");
	
    /* 
    struct sembuf {
		unsigned short sem_num;   numero do semáforo (dentro do grupo de semáforos, começando a contar em 0)
        short          sem_op;    operação a ser realizada com o semáforo 
        short          sem_flg;   operation flags 
	};
    */
    struct sembuf semaforo_temporario;
    
    /* 
	union semun {
		int val;
		struct semid_ds *buf; estrutura para o grupo de semáforos
		ushort *array;
	};
	*/
	union semun argumento; 
	
	/* 
	  struct semid_ds {
		   struct ipc_perm sem_perm;   Permissões, tipo 0666, e as que vieram na criação
		   time_t          sem_otime;  Tempo (em segundos) que a ultima operação foi realizada
		   time_t          sem_ctime;  Last change time 
		   unsigned short  sem_nsems;  número de semaforos associados àquele id
	   };
	*/
	struct semid_ds grupo_temporario_semaforo; 

	/* Tenta criar um grupo de semáforos */
    grupo_semaforo_id = semget(key, NUMERO_SEMAFOROS, IPC_CREAT | IPC_EXCL | 0666);

	/* grupo_semaforo_id >= 0 se o grupo de semáforos foi criado, grupo_semaforo_id é o id do grupo */
    if (grupo_semaforo_id >= 0) 
    {   
        printf("Criando um grupo de semaforos[id=%i]<enter> para continuar...", grupo_semaforo_id); 
        getchar();
        
        semaforo_temporario.sem_op = 1; /* ira fazer uma operação */
        semaforo_temporario.sem_flg = 0; /* não seta flag */
        semaforo_temporario.sem_num = 0; /* começa apontando para o semáforo 0 */
        
        while(semaforo_temporario.sem_num < NUMERO_SEMAFOROS) 
        { 
            /* Aqui libera os semáforos, acordando os processos que dependiam deste semáforo */
            if (semop(grupo_semaforo_id, &semaforo_temporario, 1) == -1) 
            { /* Em caso de erro, remove o grupo de semáforos */
                int e = errno;
                semctl(grupo_semaforo_id, 0, IPC_RMID); /* clean up */
                errno = e;
                return -1; /* error, check errno */
            }
            
            /* Pula para o próximo semáforo */
            semaforo_temporario.sem_num++;
        }

    } 
    /* Se o grupo de semáforos ja existe, cai pra cá */
    else if (errno == EEXIST) 
    { 
    	/* Se chegou aqui, significa que o grupo de semáforos foi criado...então pega apenas o id do grupo */
        grupo_semaforo_id = semget(key, NUMERO_SEMAFOROS, 0); 
        
        /* Quaaase impossível de acontecer esse erro abaixo...mas vai que, neh ? */
        if (grupo_semaforo_id < 0) 
        	return grupo_semaforo_id; 
    
    	printf("Conectando-se a um grupo de semaforos[id=%i] <enter> para continuar...", grupo_semaforo_id); 
        getchar();

        /* Espera 10 segundos para que outros processos se conectem ao grupo de semáforo */
        argumento.buf = &grupo_temporario_semaforo;
        argumento.val = 1;
        int i = 0, ready = 0;
        
        while(i++ < MAX_RETRIES && !ready) {
        	/* Copia os dados referentes aquele grupo de semáforos do kernel para o argumento
        	* IPC_STAT = status
        	*/
            if(semctl(grupo_semaforo_id, NUMERO_SEMAFOROS - 1, IPC_STAT, argumento) < 0)
            {
            	perror("Erro durante a copia das informacoes dos semaforos");
            	exit(1);
            }
            
            puts("até aqui");
            /* Verifica o tempo passado desde a última operação */
            if (argumento.buf->sem_otime != 0) 
                ready = 1;
            else 
                sleep(1);     
        }
        if (!ready) 
        {
            errno = ETIME;
            return -1;
        }
    }
    return grupo_semaforo_id;
}

void operacao_semaforo(int operacao)
{
    /* conecta/cria um grupo de semaforo, de tamanho 1, com a chave obtida acima */
    if (grupo_semaforo_id == -1) {
        perror("Erro no uso de semaforos");
        exit(1);
    }

    printf("<enter> para comecar operacao");
    getchar();

    /* Semáforo */
    struct sembuf semaforo_temporario;
    
    /* Numero do semaforo, pois só precisarei de 1 */
    semaforo_temporario.sem_num = 0;
    
    /* Seta como o numero de operações a serem realizadas com o recurso 
    	é negativo pois torna-se um count
    */
    semaforo_temporario.sem_op = operacao;
    
    /* Quando terminar o programa, o sistema desfazerá as alterações do semaforo */
    semaforo_temporario.sem_flg = SEM_UNDO;
    
    /* semid = id do grupo de semaforo
    *  sb = semaforo buffer, essa estrutura declarada acima
    *  1 = numero de opperações a serem realizadas (ou o incremento de sb_sem_op)
    *		é isso aqui que vai bloquear o processo até que outro libere-o
    */
    if (semop(grupo_semaforo_id, &semaforo_temporario, 1) == -1) {
        perror("Erro durante operacao com semaforos");
        exit(1);
    }
}

void get_key(const char * arquivo)
{
	if ((key = ftok(arquivo, 'A')) == -1) {
        perror("ftok");
        exit(1);
    }
    printf("Criando chave [key=%i]...\n", key);
}

