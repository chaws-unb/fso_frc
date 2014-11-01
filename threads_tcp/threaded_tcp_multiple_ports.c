
/* Sample TCP server */

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <pthread.h>

int port_number;
pthread_mutex_t mutex;

void * tcp_server(void * port)
{
   int listenfd,connfd,n;
   struct sockaddr_in servaddr,cliaddr;
   socklen_t clilen;
   pid_t     childpid;
   char mesg[1000];

   listenfd=socket(AF_INET,SOCK_STREAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr("0.0.0.0");

   int p;
   p = 32000;

   /* Port to listen TCP connections */
   /* Try to get mutex in order to perform some work, sleeps until that happen */
   pthread_mutex_lock (&mutex);

   p = port_number;
   port_number++;

   /* Finally, let other threads work */
   pthread_mutex_unlock (&mutex);

   printf("Listening on port %i\n", p);

   servaddr.sin_port=htons(p);
   bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   listen(listenfd,1024);

   for(;;)
   {
      clilen=sizeof(cliaddr);
      connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);

      if ((childpid = fork()) == 0)
      {
         close (listenfd);

         for(;;)
         {
            n = recvfrom(connfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&clilen);
            sendto(connfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
            printf("Received on port %i\n", p);
            printf("-------------------------------------------------------\n");
            mesg[n] = 0;
            printf("Received the following:\n");
            printf("%s",mesg);
            printf("-------------------------------------------------------\n");
         }
         
      }
      close(connfd);
   }
}


int main(int argc, char**argv)
{
   port_number = 32000;

   /* 4 threads */
   pthread_t callThd[4];

   /* Initialize the mutes */
   pthread_mutex_init(&mutex, NULL);

   /* Create threads to perform the dotproduct  */
   pthread_attr_t attr;
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   /* Start all threads */
   printf("Starting 4 threads...\n");
   int i;
   int port = 32000;
   for(i = 0; i < 4; i++)
   {
      /* Each thread receives its own thread_id number */
      port = port + i;
      printf("Thread on port %i go!\n", port);
      pthread_create(&callThd[i], &attr, tcp_server, (void *)&port);
   }

   pthread_attr_destroy(&attr);

   /* Wait on the other threads */
   void * status;
   for(i = 0; i < 4; i++)
      pthread_join(callThd[i], &status);

   pthread_mutex_destroy(&mutex);

   printf("Finished!\n");
}
