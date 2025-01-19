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
	if (*isFirst == true)
	{
		printf("first\n");
		(*shmaddr)->counter = 1;
		(*shmaddr)->sem = sem_open("/shmaddr_sem", O_CREAT, 0644, 1);
		if ((*shmaddr)->sem  == SEM_FAILED) {
			perror("sem_open");
			return false;
		}
		*myOrder = 1;
	}
	else
	{
		*myOrder = ++(*shmaddr)->counter;
		printf("counter not first: %d\n", (*shmaddr)->counter);
	}
	return true;
}
/*{
	key_t key = keygen();
	if (key == -1)
		return false;
	*shmid = shmget(key, sizeof(sharedMemory), IPC_CREAT | IPC_EXCL | 0666);
	if (*shmid == -1)
	{
		*shmid = shmget(key, sizeof(sharedMemory), 0666);
		if (*shmid == -1)
		{
			perror("shmget");
			return false;
		}
		printf("Attached to existing shared memory segment\n");
	}
	else
	{
		*isFirst = true;
		printf("Created new shared memory segment\n");
	}
	*shmaddr = (sharedMemory *)shmat(*shmid, NULL, 0);
	if (*shmaddr == (void *)-1)
	{
		perror("shmat");
		return false;
	
	}
	if (*isFirst == true)
	{
		strcpy((*shmaddr)->message, "memory");
		(*shmaddr)->sem = sem_open("/shmaddr_sem", O_CREAT, 0644, 1);
		if ((*shmaddr)->sem  == SEM_FAILED) {
			perror("sem_open");
			exit(EXIT_FAILURE);
		}
	}
	else
		printf("Message = %s\n", (*shmaddr)->message);
	return true;
}*/


void initSharedRessources(sharedMemory *shmaddr,int team, unsigned short int *myOrder, bool isFirst)
{
	// sem_wait(shmaddr->sem);
	if (isFirst == true)
		shmaddr->counter = 0;
	*myOrder = ++shmaddr->counter;
	printf("myOrder: %d\n", *myOrder);
	shmaddr->players[*myOrder].isActive = true;
	shmaddr->players[*myOrder].team = team;
	shmaddr->players[*myOrder].x = 0;
	shmaddr->players[*myOrder].y = 0;
	// sem_post(shmaddr->sem);
}



