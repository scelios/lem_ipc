#ifndef LEMIPC_H
#define LEMIPC_H

#include "../libft/includes/ft_printf.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <errno.h>

#define SHM_KEY 65
#define SHM_SIZE 1024
#define MAX_PROCESSES 20
#define MSGSZ 128

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

/*
* @isActive: true if the process is active
* @team: wich team the player is in
* @x: x position of the player
* @y: y position of the player
*/
typedef struct player{
	bool isActive;
	unsigned short int team;
	unsigned short int x;
	unsigned short int y;
} player;

/*
* @sem: semaphore
* @counter: number of processes currently active
* @wichToPlay: wich team is playing
* @nTeams: number of teams
* @players: array of players
* @message: message to be sent
*/
typedef struct sharedMemory{
	sem_t *sem;
    unsigned short int counter;
	unsigned short int wichToPlay;
	unsigned short int nTeams;
    player players[MAX_PROCESSES];
    char message[SHM_SIZE - sizeof(int) - sizeof(int) * MAX_PROCESSES];
} sharedMemory;

/* Ressources */
bool getSharedRessources(bool *isFirst, int *shmid, sharedMemory **shmaddr, unsigned short int *myOrder);
void initSharedRessources(sharedMemory *shmaddr,int team, unsigned short int *myOrder, bool isFirst);



#endif