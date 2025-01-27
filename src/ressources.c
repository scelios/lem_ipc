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

bool getSharedRessources(int *shm_fd, sharedMemory **shmaddr, unsigned short int *myOrder)
{
	// Try to create the shared memory object
	*shm_fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0666);
	if (*shm_fd == -1) {
		// If the shared memory object already exists, open it
		*shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
		if (*shm_fd == -1) {
			perror("shm_open");
			return false;
		}
		*myOrder = 2;

		printf("Attached to existing shared memory segment %d\n",*myOrder);
	} else {
		*myOrder = 1;
		printf("Created new shared memory segment %d\n",*myOrder);

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
	if (*myOrder == 1) {
		sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
		if (sem == SEM_FAILED) {
			perror("sem_open");
			return false;
		}
	} else {
		usleep(1000); //wait for the first process to create the semaphore
		// Open the existing semaphore
		sem = sem_open(SEM_NAME, 0);
		if (sem == SEM_FAILED) {
			perror("sem_open");
			return false;
		}
	}
	return true;
}
/*
Return in function of the index:
0 0
1 1
2 -1
3 2
4 -2
5 3
*/
short int minus(short int index)
{
	if (index % 2 == 0)
		return index / 2;
	else
		return -((index + 1) / 2);
}

void doPosition(sharedMemory *shmaddr, player *player, unsigned short int index, unsigned short int team)
{
	const unsigned short int tab[4][2] = {{MAP_SIZE / 2, 4}, 
							{MAP_SIZE / 2, MAP_SIZE - 4},
							{4, MAP_SIZE / 2},
							{MAP_SIZE - 4, MAP_SIZE / 2}};
	unsigned short int rest = index / 5;
	index = index % 5;
	// printf("team = %d,index = %d, rest = %d\n",team, index, rest);
	switch (team)
	{
	case 0:
		player->x = tab[team][0] + minus(index);
		player->y = tab[team][1] - rest;
		break;
	case 1:
		player->x = tab[team][0] + minus(index);
		player->y = tab[team][1] + rest;
		break;
	case 2:
		player->x = tab[team][0] - rest;
		player->y = tab[team][1] + minus(index);
		break;
	case 3:
		player->x = tab[team][0] + rest;
		player->y = tab[team][1] + minus(index);
		break;
	default:
		break;
	}
	shmaddr->map[player->x][player->y].team = team;
	shmaddr->map[player->x][player->y].player = player;
	// printf("Map position x = %d, y = %d\n", shmaddr->map[player->x][player->y].player->x, shmaddr->map[player->x][player->y].player->y);

}

void initMap(sharedMemory *shmaddr)
{
	for (unsigned short int i = 0; i < MAP_SIZE; i++)
	{
		for (unsigned short int j = 0; j < MAP_SIZE; j++)
		{
			shmaddr->map[i][j].team = 0;
			shmaddr->map[i][j].player = NULL;
		}
	}
}

void initSharedRessources(sharedMemory *shmaddr,int team, unsigned short int *myOrder)
{
	unsigned short int index;
	if (sem_wait(sem) == -1) {
		perror("sem_wait");
		shmaddr->criticalError = true;
		exit(EXIT_FAILURE);
	}


	if (*myOrder == 1)
	{
		shmaddr->launch = false;
		shmaddr->counter = 0;
		shmaddr->wichToPlay = 0;
		shmaddr->changed = true;
		shmaddr->criticalError = false;
		shmaddr->end = false;
		initMap(shmaddr);
		for (unsigned short int i = 0; i < MAX_TEAM; i++)
		{
			shmaddr->teams[i].isActive = false;
			shmaddr->teams[i].nPlayers = 0;
		}
	}
	if (shmaddr->counter >= MAX_PROCESSES)
	{
		printf("Max number of processes reached\n");
		if (sem_post(sem) == -1) {
			perror("sem_post");
		}
		exit(EXIT_FAILURE);
	}
	*myOrder = ++shmaddr->counter;
	index = shmaddr->teams[team].nPlayers++;
	shmaddr->teams[team].isActive = true;
	shmaddr->teams[team].players[index].isActive = true;
	if (index <= 20){
		printf("Myorder = %d\n", *myOrder);
		doPosition(shmaddr, &shmaddr->teams[team].players[index], index, team);
	}
	if (sem_post(sem) == -1) {
		perror("sem_post");
		shmaddr->criticalError = true;
		exit(EXIT_FAILURE);
	}
}

bool checkTeam(sharedMemory *shmaddr)
{
	unsigned short int teamNb = 0;
	bool atleast2 = false;

	for (int i = 1; i <= MAX_TEAM; i++)
	{
		if (shmaddr->teams[i].isActive == true)
			teamNb++;
		if (teamNb >= MAX_PROCESSES)
			return false;
		if (shmaddr->teams[i].nPlayers >= 2)
			atleast2 = true;
	}
	if (teamNb < 2 || atleast2 == false)
		return false;
	return true;
}

bool initGame(sharedMemory *shmaddr)
{
	if (sem_wait(sem) == -1) {
		perror("sem_wait");
		shmaddr->criticalError = true;
		exit(EXIT_FAILURE);
	}
	if (checkTeam(shmaddr) == false)
	{
		if (sem_post(sem) == -1) {
			perror("sem_post");
			shmaddr->criticalError = true;
			exit(EXIT_FAILURE);
		}
		return false;
	}
	shmaddr->wichToPlay = 1;
	if (sem_post(sem) == -1) {
		perror("sem_post");
		shmaddr->criticalError = true;
		exit(EXIT_FAILURE);
	}
	return true;
}

