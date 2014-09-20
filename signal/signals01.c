#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int a;

int main(){
	printf("PID = %i\n", getpid());
	while(1){
		printf("valor de a = %i\n", a++);
		sleep(3);
	}
}
