#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The main string to be processed */
const char * my_string = "This is a very huuuuuuuuuuuuuuuuge string...!"
"Well, for starters, multiple threads can run on multiple CPUs, providing a performance improvement."
" A multithreaded application works just as well on a single-CPU system, but without the added speed."
" As multi-core processors become commonplace, such as Dual-Core processors and Intel Pentium 4's with "
" HyperThreading, multithreading will be one of the simplest ways to boost performance";

size_t my_string_length;

/* Shared resource, after a thread reads string[pos], it must increment it */
int pos;

/*[Mut]ual [Ex]clusion, controls race condition between threads*/
pthread_mutex_t mutex;

void * read_string(void * arg)
{
	char current_char;
	int id = *(int *)arg;
	char exit_now = 0;
	printf("Thread %i just started...\n", id);

	while(1)
	{
		/* Try to get mutex in order to perform some work, sleeps until that happen */
		pthread_mutex_lock (&mutex);

		/* Check whether there's no more stuff to read */
		if(pos >= my_string_length)
			exit_now = 1;
		else
			/* Now it's safe to read the string */
			current_char = my_string[pos++];

		/* Finally, let other threads work */
		pthread_mutex_unlock (&mutex);

		if(exit_now)
			break;

		/* Do our dirty job (replace it for what you really want to do: ciphering)*/
		int i;
		int crazy_senseless_value = 0;
		for(i = 0; i < current_char; i++)
			crazy_senseless_value += i;

		printf("T[%i]: '(%i)%c' => %i\n", id, pos, current_char, crazy_senseless_value);
	}

	/* Time to leave! */
	pthread_exit((void*) 0);
}

int main (int argc, char *argv[])
{
	/* Starts at 0 */
	pos = 0;

	/* Sets max length, so when pos >= length, threads must exit */
	my_string_length = strlen(my_string);

	/* Create 4 threads */
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
	for(i = 0; i < 4; i++)
	{
		/* Each thread receives its own thread_id number */
		printf("Thread %i go!\n", i);
		pthread_create(&callThd[i], &attr, read_string, (void *)&i);
	}

	pthread_attr_destroy(&attr);

	/* Wait on the other threads */
	void * status;
	for(i = 0; i < 4; i++)
		pthread_join(callThd[i], &status);

	printf("All 4 threads finished...\n");
	pthread_mutex_destroy(&mutex);
	//pthread_exit(NULL);
}  