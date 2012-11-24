#include "waiting.h"



//timeout flag
volatile int timesup = 0;

bool waiting(int time)
{
	signal(SIGALRM, handler);
	alarm(time);
	do{
		printf("looping\n");
	}while(!timesup);

	return 1;
}

void handler(int sig)
{
	timesup = 1;
	alarm(0);
}

void main(int argc, char *argv[])
{
	waiting(1);
	printf("leaving main\n");
	return;
}