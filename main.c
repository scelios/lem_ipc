#include "lemipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define SHM_KEY 0x1234
#define SHM_SIZE 1024
#define MAX_PROCESSES 10

typedef struct {
    int counter;
    int order[MAX_PROCESSES];
    char message[SHM_SIZE - sizeof(int) - sizeof(int) * MAX_PROCESSES];
} shared_memory;

int main(int argc, char *argv[])
{
    int shmid;
    shared_memory *shmaddr;
    int my_order;
    int is_first_process = 0;

    // Create or get the shared memory segment
    shmid = shmget(SHM_KEY, sizeof(shared_memory), IPC_CREAT | IPC_EXCL | 0666);
    if (shmid == -1) {
        // If the segment already exists, get the existing one
        shmid = shmget(SHM_KEY, sizeof(shared_memory), 0666);
        if (shmid == -1) {
            perror("shmget");
            exit(1);
        }
        printf("Attached to existing shared memory segment\n");
    } else {
        printf("Created new shared memory segment\n");
        is_first_process = 1;
    }

    // Attach to the shared memory segment
    shmaddr = (shared_memory *)shmat(shmid, NULL, 0);
    if (shmaddr == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    // If this is the first process, initialize the shared memory
    if (is_first_process) {
        shmaddr->counter = 1;
        shmaddr->order[0] = getpid();
        strcpy(shmaddr->message, "Hello world");
        my_order = 1;
    } else {
        // Increment the counter and record the order of arrival
        my_order = ++shmaddr->counter;
        shmaddr->order[my_order - 1] = getpid();
        printf("Shared memory content: %s\n", shmaddr->message);
    }

    printf("Process %d arrived as number %d %d\n", getpid(), my_order, shmaddr->counter);

    // Wait until at least 2 instances have attached
	printf("Waiting for other processes to attach\n");
    while (shmaddr->counter < 2) {
        sleep(1);
    }

    // Detach from the shared memory segment
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(1);
    }

    // Remove the shared memory segment if this is the last process
    if (argc == 2) {
        printf("Removing shared memory segment\n");
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(1);
        }
    }

    return 0;
}