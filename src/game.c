#include "lemipc.h"

// is adjacent work horizontally, vertically and diagonally
bool isAdjacent(player *player1,player *player2)
{
	if (abs(player1->x - player2->x) <= 1 && abs(player1->y - player2->y) <= 1
		&& (player1->x != player2->x || player1->y != player2->y))
	{
		// printf("Ma pos %d %d Killer pos %d %d\n",player1->x, player1->y,player2->x, player2->y);
		
		return true;
	}
	return false;
}

void checkThisPlayerAlive(sharedMemory *shmaddr, player *player)
{
	char counter[4] = {0,0,0,0};
	for (char i = 0; i < MAX_TEAM; i++)
	{
		if (shmaddr->teams[i].isActive == false || i == player->team)
			continue;
		for (char j = 0; j < MAX_PROCESSES; j++)
		{
			if (shmaddr->teams[i].players[j].isActive == false)
				continue;
			if (isAdjacent(&shmaddr->teams[i].players[j], player) == true)
				counter[i]++;
			if (counter[i] >= 2 )
			{
				player->isActive = false;
				printf("player dead %d %d",player->x, player->y);
				// shmaddr->map[player->x][player->y].player = NULL;
				shmaddr->teams[player->team].nPlayers--;
				if (shmaddr->teams[player->team].nPlayers == 0)
				{
					shmaddr->teams[player->team].isActive = false;
				}
				// printf("player team = %d i = %d\n",player->team, i);
				// printf("Ma pos %d %d Killer pos %d %d\n",player->x, player->y,shmaddr->teams[i].players[j].x, shmaddr->teams[i].players[j].y);
				write(1,"Dead\n",5);
			}
		}
	}
}

// is dead if 2 player of the same team are adjacent to him
void checkAlive(sharedMemory *shmaddr)
{
	if (sem_wait(sem) == -1) {
		perror("sem_wait");
		shmaddr->criticalError = true;
		exit(EXIT_FAILURE);
	}
	for (char i = 0; i < MAX_TEAM; i++)
	{
		if (shmaddr->teams[i].isActive == false)
			continue;
		for (char j = 0; j < MAX_PROCESSES; j++)
		{
			if (shmaddr->teams[i].players[j].isActive == false)
				continue;
			checkThisPlayerAlive(shmaddr, &shmaddr->teams[i].players[j]);
		}
	}
	if (sem_post(sem) == -1) {
		perror("sem_post");
		shmaddr->criticalError = true;
		exit(EXIT_FAILURE);
	}
}

void checkTeamAlive(sharedMemory *shmaddr)
{
	char oneAlive = 0;
	if (sem_wait(sem) == -1) {
		perror("sem_wait");
		shmaddr->criticalError = true;
		exit(EXIT_FAILURE);
	}
	for (char i = 0; i < MAX_TEAM; i++)
	{
		if (shmaddr->teams[i].isActive == false)
			continue;
		else
		{
			oneAlive++;
		}
	}
	if (oneAlive <= 1)
	{
		shmaddr->end = true;
	}
	if (sem_post(sem) == -1) {
		perror("sem_post");
		shmaddr->criticalError = true;
		exit(EXIT_FAILURE);
	}
}

void launchGame(sharedMemory *shmaddr, player *player)
{
	bool shouldContinue = true;

	while (shouldContinue == true)
	{
		usleep(1000000);
		if (sem_wait(sem) == -1) {
			perror("sem_wait");
			shmaddr->criticalError = true;
			exit(EXIT_FAILURE);
		}
		// printf("Team nPlayers = %d team =%d\n", shmaddr->teams[player->team].nPlayers, player->team);
		// printf("Team = %d, Nplayers = %d\n", player->team, shmaddr->teams[player->team].nPlayers);
		// checkAlive(shmaddr, player);
		shouldContinue = (shmaddr->end == false && player->isActive == true);
		if (shouldContinue == false)
		{
			shmaddr->teams[player->team].nPlayers--;
			if (shmaddr->teams[player->team].nPlayers == 0)
				shmaddr->teams[player->team].isActive = false;
		}
		// printf("Continue = %d end = %d player = %d\n", shouldContinue, shmaddr->end, player->isActive);
		if (sem_post(sem) == -1) {
			perror("sem_post");
			shmaddr->criticalError = true;
			exit(EXIT_FAILURE);
		}
	}

	// printf("End of game\n");
}


