#ifndef LEMIPC_H
#define LEMIPC_H

#include "../lib/libft/includes/ft_printf.h"
# include "../lib/MLX42/include/MLX42/MLX42.h"
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/msg.h>
# include <sys/mman.h>
# include <fcntl.h>
# include <semaphore.h>
# include <sys/sem.h>
# include <errno.h>
# include <math.h>
# include <time.h>
# include <signal.h>

# define SHM_KEY 65
# define SHM_SIZE 1024
# define MAX_TEAM 4
# define MAP_SIZE 16
# define MAX_PROCESSES ((MAP_SIZE * MAP_SIZE) - (2 * MAP_SIZE))
// Max = 16 * 16 - 2 * 16 = 224
// Come from Area = size of square * size of square - 2 * size of square (for the diagonals)

# define MSGSZ 128

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

# define SHM_NAME "/shm_exampleeeey"
# define SEM_NAME "/shmaddr_sem_meeeey"

#define SHM_KEY_PATH "/tmp/lemipc_kee"
#define SHM_KEY_ID 65

# define WIDTH 496
# define HEIGHT 496

extern bool *sigintReceived;
extern sem_t *sem;
typedef struct msg_buf {
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
    bool isSelected;
    bool willDie;
    int id;
    unsigned short int team;
    unsigned short int x;
    unsigned short int y;
} player;

typedef struct team{
    bool isActive;
    player players[MAX_PROCESSES];
    unsigned short int nPlayers;
} team;

typedef struct map{
    unsigned short int team;
    struct player *player;
} map;

/** 
* @sem: semaphore
* @counter: number of processes currently active

* @nTeams: number of teams
* @players: array of players
* @message: message to be sent
*/
typedef struct sharedMemory{
    sem_t *sem;
    bool launch;
    unsigned short int counter;
	unsigned short int order[MAX_TEAM];
    unsigned short int nTeams;
    // map map[MAP_SIZE][MAP_SIZE];
    team teams[MAX_TEAM];
    bool criticalError;
    bool end;
    bool changed;
    int msqid;
    int wichToPlay;
    // char message[SHM_SIZE - sizeof(int) - sizeof(int) * MAX_PROCESSES];
} sharedMemory;

typedef struct screen
{
    mlx_t			*mlx;
    mlx_image_t		*img;
    int32_t			width;
    int32_t			height;
    double			x;
    double			y;
    bool			moved;
    bool			resized;
    bool			isClicked;
    sharedMemory	*shmaddr;
}	screen;

/* Ressources */
bool getSharedRessources(int *shmid, sharedMemory **shmaddr, unsigned short int *myOrder);
void initSharedRessources(sharedMemory *shmaddr,int team, unsigned short int *myOrder, int *index);
bool initGame(sharedMemory *shmaddr);
void doPosition(sharedMemory *shmaddr, player *player, unsigned short int index, unsigned short int team);


/* Graphics */
void launchGraphics(sharedMemory *shmaddr);
void printMap(sharedMemory *shmaddr);
bool	someoneThere(sharedMemory *shmaddr, int x, int y);
player *getPlayer(sharedMemory *shmaddr, int x, int y);



/* hooks */
void closeScreen(void *param);
void cursor(double xpos, double ypos, void *param);
void resize(int32_t width, int32_t height, void *param);
void hook(void *param);

/* game */

void launchGame(sharedMemory *shmaddr, int team, int *index);
void killWillDie(sharedMemory *shmaddr);

void checkAlive(sharedMemory *shmaddr);
void checkTeamAlive(sharedMemory *shmaddr);

void movePlayer(sharedMemory *shmaddr,player *player, int x, int y);


/* message */
void sendDeathMessage(sharedMemory *shmaddr, player *player);
void sendMoveMessage(sharedMemory *shmaddr, player *player, int x, int y);
void receiveMessage(sharedMemory *shmaddr, player *player);
void checkAtLeastTwoInOneTeam(sharedMemory *shmaddr);


void handleSigint(int sig);

#endif