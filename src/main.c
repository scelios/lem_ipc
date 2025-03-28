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
    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    shmaddr->counter--;
    last = shmaddr->counter == 0;
    if (sem_post(sem) == -1) {
        perror("sem_post");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    return (last);
}

void cleanSharedRessources(sharedMemory *shmaddr)
{
    printf("Cleaning shared ressources\n");
    if (sem_close(sem) == -1) {
        perror("sem_close");
        exit(EXIT_FAILURE);
    }
    if (msgctl(shmaddr->msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }
    if (remove(SHM_KEY_PATH) == -1) {
        if (errno != ENOENT) {
            perror("remove");
            exit(EXIT_FAILURE);
        }
    }
    if (munmap(shmaddr, sizeof(sharedMemory)) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }
    if (sem_unlink(SEM_NAME) == -1) {
        perror("sem_unlink");
        exit(EXIT_FAILURE);
    }
}

bool atLeastTwoplayerInOneTeam(sharedMemory *shmaddr)
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
            return true;
        }
    }

    return false;
}

bool atLeastTwoTeam(sharedMemory *shmaddr)
{
    int teamNb = 0;

    for (int i = 0; i < MAX_TEAM; i++)
    {
        if (shmaddr->teams[i].isActive == true)
        {
            teamNb++;
        }
        if (teamNb >= 2)
        {
            return true;
        }
    }

    return false;
}

void waitForPlayers(sharedMemory *shmaddr)
{
    bool launch = false;

    time_t start = time(NULL);
    while (launch != true) {
        if (sem_wait(sem) == -1) {
            perror("sem_wait");
            shmaddr->criticalError = true;
            exit(EXIT_FAILURE);
        }
        
        if (shmaddr->counter >= 3 && atLeastTwoplayerInOneTeam(shmaddr) == true\
        && atLeastTwoTeam(shmaddr) == true)
        {
            launch = true;
            shmaddr->launch = true;
        }
        if (sem_post(sem) == -1) {
            perror("sem_post");
            shmaddr->criticalError = true;
            exit(EXIT_FAILURE);
        }
        // usleep(5000);
        if (time(NULL) - start > 1)
        {
            printf("Timeout\n");
            shmaddr->criticalError = true;
            if (isLast(shmaddr) == true)
            {
                cleanSharedRessources(shmaddr);
            }
            exit(EXIT_FAILURE);
        }
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
    int index;
    if (checkArgs(argc, argv) == false \
    || getSharedRessources(&shmid, &shmaddr, &myOrder) == false)
    {
        exit(EXIT_FAILURE);
    }
    // printf("myOrder %d\n", myOrder);
    // cleanSharedRessources(shmaddr);
    // return 0;
    // printf("Myorder %d\n", myOrder);
    initSharedRessources(shmaddr, ft_atoi(argv[1]) - 1, &myOrder, &index); //set the default team to 0
    signal(SIGINT, handleSigint);
    waitForPlayers(shmaddr);
    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    if (shmaddr->launchGraphics == false)
    {
        shmaddr->launchGraphics = true;

        if (sem_post(sem) == -1) {
            perror("sem_post");
            shmaddr->criticalError = true;
            exit(EXIT_FAILURE);
        }
        initGame(shmaddr);
        // printTeamPosition(shmaddr);
        // printMap(shmaddr);

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            shmaddr->criticalError = true;
            exit(EXIT_FAILURE);
        }
        if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
        }
        else if (pid == 0)
        {
            launchGraphics(shmaddr);
            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        if (sem_post(sem) == -1) {
            perror("sem_post");
            shmaddr->criticalError = true;
            exit(EXIT_FAILURE);
        }
    }

    launchGame(shmaddr, ft_atoi(argv[1]) - 1, &index);
    // Remove the shared memory segment if this is the last process
    if (isLast(shmaddr) == true)
    {
        printf("Last process\n");
        cleanSharedRessources(shmaddr);
    }
    return 0;
}