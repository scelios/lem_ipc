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
	return (shmaddr->counter == 0);
}

void cleanSharedRessources(int shmid, sharedMemory *shmaddr)
{
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		perror("shmctl");
		exit(1);
	}
	sem_close(shmaddr->sem);
	sem_unlink("/shmaddr_sem");
	shmctl(shmid, IPC_RMID, NULL);
}


void detachSharedRessources(sharedMemory *shmaddr)
{
	if (shmdt(shmaddr) == -1) {
		perror("shmdt");
		exit(1);
	}
}

void check(sharedMemory *shmaddr, unsigned short int myOrder)
{
	// sem_wait(shmaddr->sem);
	printf("counter: %d Order: %d\n", shmaddr->counter, myOrder);
	// sem_post(shmaddr->sem);
}

int main(int argc, char *argv[])
{
	bool isFirst = false;
	int shmid;
	sharedMemory *shmaddr;
	unsigned short int myOrder = 0;

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
}