#include "lemipc.h"
sem_t *sem;

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
		printf("team_number must be between 1 and %d\n", MAX_TEAM);
		return false;
	}
	if (ft_str_is_numeric(argv[1]) == false)
	{
		printf("team_number must be a positive number\n");
		return false;
	}
	int teamNumber = ft_atoi(argv[1]);
	if (teamNumber < 1 || teamNumber > MAX_TEAM)
	{
		printf("team_number must be between 1 and %d\n", MAX_TEAM);
		return false;
	}
	return true;
}

bool isLast(sharedMemory *shmaddr)
{
	bool last = false;
	sem_wait(sem);
	shmaddr->counter--;
	last = shmaddr->counter <= 1 ? true : false;
	sem_post(sem);
	return (last);
}

void cleanSharedRessources(sharedMemory *shmaddr)
{
	if (sem_close(sem) == -1) {
		perror("sem_close");
		exit(EXIT_FAILURE);
	}
	
	if (munmap(shmaddr, sizeof(sharedMemory)) == -1) {
		perror("munmap");
		exit(EXIT_FAILURE);
	}
	// printf("shm_unlink %s\n",SHM_NAME);
	// system("ls /dev/shm/*");
	shm_unlink(SHM_NAME);
	sem_unlink(SEM_NAME);
	// if (shm_unlink(SHM_NAME) == -1) {
	// 	perror("shm_unlink");
	// 	// exit(EXIT_FAILURE);
	// }
	// if (sem_unlink(SEM_NAME) == -1) {
	// 	perror("sem_unlink");
	// 	// exit(EXIT_FAILURE);
	// }
	// return;
}

void waitForPlayers(sharedMemory *shmaddr)
{
	bool launch = false;

	while (launch != true) {
		if (sem_wait(sem) == -1) {
			perror("sem_wait");
			shmaddr->criticalError = true;
			exit(EXIT_FAILURE);
		}
		
		if (shmaddr->counter >= 2)
		{
			launch = true;
			shmaddr->launch = true;
		}
		if (sem_post(sem) == -1) {
			perror("sem_post");
			shmaddr->criticalError = true;
			exit(EXIT_FAILURE);
		}
		usleep(5000);
	}
}

void printTeamPosition(sharedMemory *shmaddr)
{
	for (int i = 0; i < MAX_TEAM; i++)
	{
		if (shmaddr->teams[i].isActive == true)
		{
			printf("Team %d\n", i);
			for (int j = 0; j < MAX_PROCESSES; j++)
			{
				if (shmaddr->teams[i].players[j].isActive == true)
				{
					printf("	Player %d: x=%d y=%d\n", j, shmaddr->teams[i].players[j].x, shmaddr->teams[i].players[j].y);
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int shmid;
	sharedMemory *shmaddr;
	unsigned short int myOrder = 0;

	if (checkArgs(argc, argv) == false \
		|| getSharedRessources(&shmid, &shmaddr, &myOrder) == false)
	{
		exit(EXIT_FAILURE);
	}
	// cleanSharedRessources(shmid, shmaddr);
	// return 0;
	initSharedRessources(shmaddr, ft_atoi(argv[1]) - 1, &myOrder); //set the default team to 0
	// waitForPlayers(shmaddr);
	if (myOrder == 1)
	{
		initGame(shmaddr);
		printTeamPosition(shmaddr);
		// do a fork here
		// launchGraphics(shmaddr);
		
		pid_t pid = fork();
		if (pid == -1)
		{
			perror("fork");
			shmaddr->criticalError = true;
			exit(EXIT_FAILURE);
		}
		if (pid != 0)
		{
			launchGraphics(shmaddr);
			printf("End of graphics\n");
			exit(EXIT_SUCCESS);
		}
		// launchGraphics();
	}
	usleep(50000);
	// launchGame(shmaddr, myOrder);
	sleep(5);
	// Remove the shared memory segment if this is the last process
	if (isLast(shmaddr) == true)
	{
		cleanSharedRessources(shmaddr);
		printf("Clean shared ressources %d\n",myOrder);
	}
	printf("End of process %d\n", myOrder);
	return 0;
}