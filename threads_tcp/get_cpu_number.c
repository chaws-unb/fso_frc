#include <unistd.h>
#include <stdio.h>

int main()
{

	long int number_of_cpus = sysconf(_SC_NPROCESSORS_ONLN);

	printf("The total number of CPUs in this computer is: %li\n", number_of_cpus);

	return 0;
}