#include "lemipc.h"

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
    // Generate the file if it does not exist
    int fd = open(SHM_KEY_PATH, O_CREAT, 0666);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    close(fd);

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

        // printf("Attached to existing shared memory segment %d\n",*myOrder);
    } else {
        *myOrder = 1;
        // printf("Created new shared memory segment %d\n",*myOrder);

        // Set the size of the shared memory object
        if (ftruncate(*shm_fd, sizeof(sharedMemory)) == -1) {
            perror("ftruncate");
            return false;
        }
    }

    // Map the shared memory object
    *shmaddr = (sharedMemory *)mmap(NULL, sizeof(sharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, *shm_fd, 0);
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

short int minus(short int index)
{
    if (index % 2 == 0)
        return index / 2;
    else
        return -((index + 1) / 2);
    return 0;
}

void doposition(sharedMemory *shmaddr, player *player, unsigned short int index, unsigned short int team)
{
    int i = 0;
    int two_k = 2; //sum of 1 to n of 2k = n(n+1)
    
    (void)shmaddr;
    while (index > 0 && index >= two_k && i < 7)
    {
        index -= two_k;
        two_k += 2;
        i++;
    }
    // printf("index = %d, i = %d\n", index, i);

    switch (team)
    {
    case 0:// up
        player->x = MAP_SIZE / 2 + minus(index);
        player->y = MAP_SIZE / 2 - i - 2;
        break;
    case 1: // right
        player->x = MAP_SIZE / 2 + i + 1;
        player->y = MAP_SIZE / 2 + minus(index);
        break;
    case 2: // down
        player->x = MAP_SIZE / 2 + minus(index);
        player->y = MAP_SIZE / 2 + i + 1;
        break;
    case 3: // left
        player->x = MAP_SIZE / 2 - i - 2;
        player->y = MAP_SIZE / 2 + minus(index);
        break;
    default:
        break;
    }
}

void doPosition(sharedMemory *shmaddr, player *player, unsigned short int index, unsigned short int team)
{
    (void)shmaddr;
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
    // shmaddr->map[player->x][player->y].team = team;
    // shmaddr->map[player->x][player->y].player = player;
    // printf("Map position x = %d, y = %d\n", shmaddr->map[player->x][player->y].player->x, shmaddr->map[player->x][player->y].player->y);
}

void initSharedRessources(sharedMemory *shmaddr,int team, unsigned short int *myOrder, int *index)
{
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
        key_t key = keygen();
        int msqid = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
        if (msqid == -1) {
            perror("msgget");
            exit(EXIT_FAILURE);
        }
        shmaddr->msqid = msqid;
        for (unsigned short int i = 0; i < MAX_TEAM; i++)
        {
            shmaddr->teams[i].isActive = false;
            shmaddr->teams[i].nPlayers = 0;
        }
    }
    if (shmaddr->counter >= MAX_PROCESSES)
    {
        // printf("Max number of processes reached\n");
        if (sem_post(sem) == -1) {
            perror("sem_post");
        }
        exit(EXIT_FAILURE);
    }
    if (shmaddr->teams[team].nPlayers >= (MAX_PROCESSES / 4))
    {
        // printf("Max number of processes in this team reached\n");
        if (sem_post(sem) == -1) {
            perror("sem_post");
        }
        exit(EXIT_FAILURE);
    }

    *myOrder = ++shmaddr->counter;
    *index = shmaddr->teams[team].nPlayers++;
    shmaddr->teams[team].isActive = true;
    shmaddr->teams[team].players[*index].isActive = true;
    shmaddr->teams[team].players[*index].willDie = false;
    shmaddr->teams[team].players[*index].team = team;
    shmaddr->teams[team].players[*index].id = *myOrder;
    doposition(shmaddr, &shmaddr->teams[team].players[*index], *index, team);


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

