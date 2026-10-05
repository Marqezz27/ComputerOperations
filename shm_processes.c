#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
    int ShmID;
    int *ShmPTR;
    pid_t pid;

    /*
     * Shared memory:
     * ShmPTR[0] = BankAccount
     * ShmPTR[1] = Turn
     */
    ShmID = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | 0666);

    if (ShmID < 0) {
        perror("shmget");
        exit(1);
    }

    ShmPTR = (int *) shmat(ShmID, NULL, 0);

    if ((int) ShmPTR == -1) {
        perror("shmat");
        exit(1);
    }

    /* Initialize shared variables */
    ShmPTR[0] = 0;  /* BankAccount */
    ShmPTR[1] = 0;  /* Turn */

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    /*
     * Parent process - Dear Old Dad
     */
    if (pid > 0) {

        srand(time(NULL) ^ getpid());

        for (int i = 0; i < 25; i++) {

            int account;
            int balance;

            /* Sleep between 0 and 5 seconds */
            sleep(rand() % 6);

            /* Copy BankAccount to local variable */
            account = ShmPTR[0];

            /* Wait for Dad's turn */
            while (ShmPTR[1] != 0)
                ;

            if (account <= 100) {

                /* Generate amount of money between 0 and 100 */
                balance = rand() % 101;

                if (balance % 2 == 0) {

                    account += balance;

                    printf("Dear old Dad: Deposits $%d / Balance = $%d\n",
                           balance, account);

                } else {

                    printf("Dear old Dad: Doesn't have any money to give\n");
                }

            } else {

                printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n",
                       account);
            }

            /* Copy local account back to shared memory */
            ShmPTR[0] = account;

            /* Give the turn to the child */
            ShmPTR[1] = 1;
        }

        /* Wait for child to finish */
        wait(NULL);

        /* Detach shared memory */
        shmdt((void *) ShmPTR);

        /* Remove shared memory */
        shmctl(ShmID, IPC_RMID, NULL);

        exit(0);
    }

    /*
     * Child process - Poor Student
     */
    else {

        srand(time(NULL) ^ getpid());

        for (int i = 0; i < 25; i++) {

            int account;
            int balance;

            /* Sleep between 0 and 5 seconds */
            sleep(rand() % 6);

            /* Copy BankAccount to local variable */
            account = ShmPTR[0];

            /* Wait for Student's turn */
            while (ShmPTR[1] != 1)
                ;

            /* Generate amount Student needs between 0 and 50 */
            balance = rand() % 51;

            printf("Poor Student needs $%d\n", balance);

            if (balance <= account) {

                account -= balance;

                printf("Poor Student: Withdraws $%d / Balance = $%d\n",
                       balance, account);

            } else {

                printf("Poor Student: Not Enough Cash ($%d)\n",
                       account);
            }

            /* Copy local account back to shared memory */
            ShmPTR[0] = account;

            /* Give the turn back to Dad */
            ShmPTR[1] = 0;
        }

        /* Detach shared memory */
        shmdt((void *) ShmPTR);

        exit(0);
    }

    return 0;
}