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

/*bool getSharedRessources(bool *isFirst, int *shmid, sharedMemory **shmaddr, unsigned short int *myOrder)
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
		(*shmaddr)->sem = sem_open("/shmaddr_sem", O_CREAT, 0644, 1);
		if ((*shmaddr)->sem == SEM_FAILED) {
			perror("sem_open");
			return false;
		}
		// sem_wait((*shmaddr)->sem);
		// (*shmaddr)->counter = 0;
		// sem_post((*shmaddr)->sem);
		
	}
	return true;
}*/

bool getSharedRessources(bool *isFirst, int *shm_fd, sharedMemory **shmaddr, unsigned short int *myOrder)
{
    const char *shm_name = SHM_NAME;
    const char *sem_name = SEM_NAME;
    *isFirst = false;

    // Try to create the shared memory object
    *shm_fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0666);
    if (*shm_fd == -1) {
        // If the shared memory object already exists, open it
        *shm_fd = shm_open(shm_name, O_RDWR, 0666);
        if (*shm_fd == -1) {
            perror("shm_open");
            return false;
        }
        printf("Attached to existing shared memory segment\n");
    } else {
        printf("Created new shared memory segment\n");
        *isFirst = true;

        // Set the size of the shared memory object
        if (ftruncate(*shm_fd, sizeof(sharedMemory)) == -1) {
            perror("ftruncate");
            return false;
        }
    }

    // Map the shared memory object
    *shmaddr = mmap(NULL, sizeof(sharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, *shm_fd, 0);
    if (*shmaddr == MAP_FAILED) {
        perror("mmap");
        return false;
    }

    if (*isFirst) {
        // Initialize the semaphore in shared memory
        (*shmaddr)->sem = sem_open(sem_name, O_CREAT, 0644, 1);
        if ((*shmaddr)->sem == SEM_FAILED) {
            perror("sem_open");
            return false;
        }
        // Initialize the counter
        (*shmaddr)->counter = 0;
    } else {
        // Open the existing semaphore
        (*shmaddr)->sem = sem_open(sem_name, 0);
        if ((*shmaddr)->sem == SEM_FAILED) {
            perror("sem_open");
            return false;
        }
    }

    return true;
}

void initSharedRessources(sharedMemory *shmaddr,int team, unsigned short int *myOrder, bool isFirst)
{
	// sem_wait(shmaddr->sem);
	if (sem_wait(shmaddr->sem) == -1) {
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }


	if (isFirst == true)
	{
		shmaddr->launch = false;
		shmaddr->counter = 0;
	}
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
	// Release semaphore
    if (sem_post(shmaddr->sem) == -1) {
        perror("sem_post");
        exit(EXIT_FAILURE);
    }
}



