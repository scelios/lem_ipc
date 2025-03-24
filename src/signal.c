#include "lemipc.h"

bool *sigintReceived;

void handleSigint(int sig)
{
    (void)sig;
    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }
    if (sigintReceived != NULL) {
        *sigintReceived = true;
    }
    if (sem_post(sem) == -1) {
        perror("sem_post");
        exit(EXIT_FAILURE);
    }
}