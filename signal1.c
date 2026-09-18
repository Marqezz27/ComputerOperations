#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t signal_received = 0;

void handler(int signum)
{
    printf("Hello World!\n");
    signal_received = 1;
}

int main()
{
    signal(SIGALRM, handler);
    alarm(5);

    while(signal_received == 0)
    {
    }

    printf("Turing was right!\n");

    return 0;
}