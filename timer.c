#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

volatile sig_atomic_t alarm_count = 0;
time_t start_time;

void alarm_handler(int signum)
{
    alarm_count++;
    alarm(1);
}

void interrupt_handler(int signum)
{
    time_t end_time = time(NULL);
    double elapsed_time = difftime(end_time, start_time);

    printf("\nNumber of alarms: %d\n", alarm_count);
    printf("Total execution time: %.0f seconds\n", elapsed_time);

    exit(0);
}

int main()
{
    signal(SIGALRM, alarm_handler);
    signal(SIGINT, interrupt_handler);

    start_time = time(NULL);

    alarm(1);

    while(1)
    {
    }

    return 0;
}