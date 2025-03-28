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

void printTeam(sharedMemory *shmaddr, int team)
{
        for (int j = 0; j < MAX_PROCESSES; j++)
        {
            if (shmaddr->teams[team].players[j].isActive == true)
            printf("Player of team %d in %d %d is alive\n",team, shmaddr->teams[team].players[j].x, shmaddr->teams[team].players[j].y);
        }
}

void checkThisPlayerAlive(sharedMemory *shmaddr, player *player)
{
    for (int i = 0; i < MAX_TEAM; i++)
    {
        if (shmaddr->teams[i].isActive == false || i == player->team)
            continue;
        char counter[4] = {0,0,0,0};
        for (int j = 0; j < MAX_PROCESSES; j++)
        {
            if (shmaddr->teams[i].players[j].isActive == false)
                continue;
            if (isAdjacent(&shmaddr->teams[i].players[j], player) == true)
                counter[i]++;
            if (counter[i] == 2 )
            {
                player->willDie = true;
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
    for (int i = 0; i < MAX_TEAM; i++)
    {
        if (shmaddr->teams[i].isActive == false)
            continue;
        for (int j = 0; j < MAX_PROCESSES; j++)
        {
            if (shmaddr->teams[i].players[j].isActive == false)
                continue;
            checkThisPlayerAlive(shmaddr, &shmaddr->teams[i].players[j]);
        }
    }
    killWillDie(shmaddr);
    if (sem_post(sem) == -1) {
        perror("sem_post");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
}

void killWillDie(sharedMemory *shmaddr)
{
    for (int i = 0; i < MAX_TEAM; i++)
    {
        if (shmaddr->teams[i].isActive == false)
                continue;
        for (int j = 0; j < (MAX_PROCESSES / 4); j++)
        {
            if (shmaddr->teams[i].players[j].isActive == false)
                continue;
            if (shmaddr->teams[i].players[j].isActive == true && shmaddr->teams[i].players[j].willDie == true)
            {
                sendDeathMessage(shmaddr, &shmaddr->teams[i].players[j]);
            }
            // if (shmaddr->teams[i].players[j].isActive == true && shmaddr->teams[i].players[j].willDie == true)
            // {
            //     shmaddr->teams[i].nPlayers--;
            //     shmaddr->teams[i].players[j].isActive = false;
            //     shmaddr->teams[i].players[j].willDie = false;
            // }
            // // shmaddr->map[shmaddr->teams[i].players[j]->x][shmaddr->teams[i].players[j]->y].shmaddr->teams[i].players[j] = NULL;
            // if (shmaddr->teams[i].nPlayers == 0)
            // {
            //     shmaddr->teams[i].isActive = false;
            //     printf("team dead\n");
            // }
            // shmaddr->map[shmaddr->teams[i].players[j]->x][shmaddr->teams[i].players[j]->y].shmaddr->teams[i].players[j] = NULL;
        }
    }
}

void checkAtLeastTwoInOneTeam(sharedMemory *shmaddr)
{
    int teamNb = 0;
    for (int i = 0; i < MAX_TEAM; i++)
    {
        teamNb = 0;
        for (int j = 0; j < MAX_PROCESSES / 4; j++)
        {
            if (shmaddr->teams[i].players[j].isActive == true)
            {
                teamNb++;
            }
        }
        if (teamNb >= 2)
        {
            return;
        }
    }
    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    shmaddr->end = true;
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
    for (int i = 0; i < MAX_TEAM; i++)
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
        printf("Last one alive\n");
    }
    if (sem_post(sem) == -1) {
        perror("sem_post");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
}

void launchGame(sharedMemory *shmaddr, int team, int *index)
{
    bool shouldStop = false;

    while (shouldStop == false)
    {
        usleep(1000);
        if (sem_wait(sem) == -1) {
            perror("sem_wait");
            shmaddr->criticalError = true;
            exit(EXIT_FAILURE);
        }
        player *player = &shmaddr->teams[team].players[*index];
        receiveMessage(shmaddr, player);
        // printf("Team nPlayers = %d team =%d\n", shmaddr->teams[player->team].nPlayers, player->team);
        // printf("Team = %d, Nplayers = %d\n", player->team, shmaddr->teams[player->team].nPlayers);
        // checkAlive(shmaddr, player);
        shouldStop = (shmaddr->end == true || player->isActive == false || shmaddr->criticalError == true);
        if (shouldStop == true)
        {
            // printf("Should stop %d %d %d\n",shouldStop,shmaddr->end ,player->isActive);
            if (shmaddr->teams[player->team].nPlayers == 0)
                shmaddr->teams[player->team].isActive = false;
            // printf("I'm dead\n");
        }
        // printf("Continue = %d end = %d player = %d\n", shouldStop, shmaddr->end, player->isActive);
        if (sem_post(sem) == -1) {
            perror("sem_post");
            shmaddr->criticalError = true;
            exit(EXIT_FAILURE);
        }
    }

    // printf("End of game\n");
}


