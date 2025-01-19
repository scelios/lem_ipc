#include "lemipc.h"
#define SHM_KEY_PATH "/tmp/lemipc_key"
#define SHM_KEY_ID 65
/**
 * keygen() - Generates a System V key.
 * @i: To generates different key if needed (need to be incremented each
 *     time a new key is needed).
 *
 * Return: The System V key on success, -1 on error.
*/
key_t keygen()
{
	key_t key;
    if ((key = ftok(SHM_KEY_PATH, SHM_KEY_ID)) == -1) {
        perror("ftok");
        return -1;
    }
    return key;
}

bool getSharedRessources(bool *isFirst, int *shmid, sharedMemory **shmaddr, unsigned short int *myOrder)
{
	key_t key = keygen();
	if (key == -1)
		return false;
	*shmid = shmget(key, sizeof(sharedMemory), IPC_CREAT | IPC_EXCL | 0666);
    if (*shmid == -1) {
        // If the segment already exists, get the existing one
        *shmid = shmget(key, sizeof(sharedMemory), 0666);
        if (*shmid == -1) {
            perror("shmget");
            return false;
        }
        printf("Attached to existing shared memory segment\n");
    } else {
        printf("Created new shared memory segment\n");
        *isFirst = true;
    }
	*shmaddr = (sharedMemory*) shmat(*shmid, NULL, 0);
    if (*shmaddr == (void *) -1) {
        perror("shmat");
        return false;
    }

	return true;
}


void initSharedRessources(sharedMemory *shmaddr,int team, unsigned short int *myOrder, bool isFirst)
{
	// sem_wait(shmaddr->sem);
	if (isFirst == true)
		shmaddr->counter = 0;
	if (shmaddr->counter >= MAX_PROCESSES)
	{
		printf("Max number of processes reached\n");
		exit(EXIT_FAILURE);
	}
	*myOrder = ++shmaddr->counter;
	shmaddr->players[*myOrder].isActive = true;
	shmaddr->players[*myOrder].team = team;
	shmaddr->players[*myOrder].x = 0;
	shmaddr->players[*myOrder].y = 0;
	// sem_post(shmaddr->sem);
}



