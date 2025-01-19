#include "lemipc.h"

bool ft_str_is_numeric(const char *str)
{
	if (str == NULL)
		return false;
	while (*str)
	{
		if (ft_isdigit(*str) == false)
			return false;
		str++;
	}
	return true;
}

bool checkArgs(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s [team_number]\n", argv[0]);
		return false;
	}
	if (ft_strlen(argv[1]) > 2)
	{
		printf("team_number must be between 1 and %d\n", MAX_PROCESSES / 2);
		return false;
	}
	if (ft_str_is_numeric(argv[1]) == false)
	{
		printf("team_number must be a positive number\n");
		return false;
	}
	int teamNumber = ft_atoi(argv[1]);
	if (teamNumber < 1 || teamNumber > MAX_PROCESSES / 2)
	{
		printf("team_number must be between 1 and %d\n", MAX_PROCESSES / 2);
		return false;
	}
	return true;
}

bool isLast(sharedMemory *shmaddr)
{
	// sem_wait(shmaddr->sem);
	shmaddr->counter--;
	// sem_post(shmaddr->sem);
	return (shmaddr->counter == 1);
}

/*void cleanSharedRessources(int shmid, sharedMemory *shmaddr)
{
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		perror("shmctl");
		exit(1);
	}
	sem_close(shmaddr->sem);
	sem_unlink("/shmaddr_sem");
	shmctl(shmid, IPC_RMID, NULL);
}*/
void cleanSharedRessources(int shmid, sharedMemory *shmaddr)
{
    if (sem_close(shmaddr->sem) == -1) {
        perror("sem_close");
        exit(EXIT_FAILURE);
    }
    if (sem_unlink(SEM_NAME) == -1) {
        perror("sem_unlink");
        exit(EXIT_FAILURE);
    }
    if (munmap(shmaddr, sizeof(sharedMemory)) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }
}

void detachSharedRessources(sharedMemory *shmaddr)
{
	// if (shmdt(shmaddr) == -1) {
	// 	perror("shmdt");
	// 	exit(1);
	// }
}

void check(sharedMemory *shmaddr, unsigned short int myOrder)
{
	// sem_wait(shmaddr->sem);
	printf("counter: %d Order: %d\n", shmaddr->counter, myOrder);
	// sem_post(shmaddr->sem);
}

/*int main(int argc, char *argv[])
{
	// bool isFirst = false;
	// int shmid;
	// sharedMemory *shmaddr;
	// unsigned short int myOrder = 0;

	if (checkArgs(argc, argv) == false \
		|| getSharedRessources(&isFirst, &shmid, &shmaddr, &myOrder) == false)
	{
		return EXIT_FAILURE;
	}
	// initSharedRessources(shmaddr, ft_atoi(argv[1]), &myOrder, isFirst);
	check(shmaddr, myOrder);
	// waitForPlayers(shmaddr);
	// if (isFirst == true)
	// {
	// 	initGame(shmaddr);
	// 	// launchGraphics();
	// }
	// launchGame(shmaddr, myOrder);
	if (isLast(shmaddr) == true)
	{
		cleanSharedRessources(shmid, shmaddr);
		printf("Clean shared ressources\n");
	}
	detachSharedRessources(shmaddr); // ? should detach at the end
	printf("Detached shared ressources\n");
	return 0;
}*/

void remove_all_ipc() {
    struct shmid_ds shm_info;
    struct semid_ds sem_info;
    struct msqid_ds msg_info;
    int shm_id, sem_id, msg_id;
    key_t key;

    // Remove all shared memory segments
    for (shm_id = shmctl(0, SHM_STAT, &shm_info); shm_id >= 0; shm_id = shmctl(shm_id + 1, SHM_STAT, &shm_info)) {
        if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
            perror("shmctl");
        }
		printf("shmctl");
    }

    // Remove all semaphores
    for (sem_id = semctl(0, 0, SEM_STAT, &sem_info); sem_id >= 0; sem_id = semctl(sem_id + 1, 0, SEM_STAT, &sem_info)) {
        if (semctl(sem_id, 0, IPC_RMID) == -1) {
            perror("semctl");
        }
		printf("semctl");
    }

    // Remove all message queues
    for (msg_id = msgctl(0, MSG_STAT, &msg_info); msg_id >= 0; msg_id = msgctl(msg_id + 1, MSG_STAT, &msg_info)) {
        if (msgctl(msg_id, IPC_RMID, NULL) == -1) {
            perror("msgctl");
        }
		printf("msgctl");
    }
}

int main(int argc, char *argv[])
{
    int shmid;
    sharedMemory *shmaddr;
    unsigned short int myOrder = 0;
    bool isFirst = false;
    key_t key;
    key = 1234;
	bool launch = false;
	// remove_all_ipc();


	if (checkArgs(argc, argv) == false \
		|| getSharedRessources(&isFirst, &shmid, &shmaddr, &myOrder) == false)
	{
        exit(EXIT_FAILURE);
    }
	
    // If this is the first process, initialize the shared memory
	initSharedRessources(shmaddr, ft_atoi(argv[1]), &myOrder, isFirst);
	sem_wait(shmaddr->sem);
	sem_post(shmaddr->sem);

    // Wait until at least 2 instances have attached
	// printf("Waiting for other processes to attach\n");
	printf("counter: %d Order: %d\n", shmaddr->counter, myOrder);
    while (shmaddr->counter < 2) {
		// sem_wait(shmaddr->sem);
		// sem_post(shmaddr->sem);
        usleep(10000);
    }
	usleep(10000);
	

    // Remove the shared memory segment if this is the last process
   if (isLast(shmaddr) == true)
	{
		cleanSharedRessources(shmid, shmaddr);
		printf("Clean shared ressources\n");
	}
	printf("End of process %d\n", myOrder);
    return 0;
}