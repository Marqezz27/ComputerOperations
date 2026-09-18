#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(void)
{
  pid_t child1, child2;
  int status;

  /* Seed the random number generator */
  srandom(time(NULL));

  /* Create first child */
  child1 = fork();

  if (child1 < 0)
  {
    perror("fork");
    exit(1);
    }

  if (child1 == 0)
  {
    /* First child process */
    int i;
    int iterations = random() % 30 + 1;

    for (i = 0; i < iterations; i++)
    {
        printf("Child Pid: %d is going to sleep!\n", getpid());

        sleep(random() % 10 + 1);

        printf("Child Pid: %d is awake!\n", getpid());
        printf("Where is my Parent: %d?\n", getppid());
      }

      exit(0);
    }

  /* Parent creates second child */
  child2 = fork();

  if (child2 < 0)
    {
      perror("fork");
      exit(1);
    }

  if (child2 == 0)
    {
      /* Second child process */
      int i;
      int iterations = random() % 30 + 1;

      for (i = 0; i < iterations; i++)
        {
          printf("Child Pid: %d is going to sleep!\n", getpid());

          sleep(random() % 10 + 1);

          printf("Child Pid: %d is awake!\n", getpid());
          printf("Where is my Parent: %d?\n", getppid());
        }

      exit(0);
    }

  /* Parent process waits for both children */
  wait(&status);
  printf("Child Pid: %d has completed\n", child1);

  wait(&status);
  printf("Child Pid: %d has completed\n", child2);

  return 0;
}