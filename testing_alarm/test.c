#include <stdio.h>
#include <signal.h>

/* number of times the handle will run: */
volatile int breakflag = 3;

void handle(int sig) {
    printf("Hello\n");
    --breakflag;
    alarm(1);
}

int main() {
    signal(SIGALRM, handle);
    alarm(1);
    while(breakflag) { sleep(1); }
    printf("done\n");
    return 0;
}