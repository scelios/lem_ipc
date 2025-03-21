#include "lemipc.h"


void sendDeathMessage(sharedMemory *shmaddr, player *player)
{
    message_buf sbuf;
    sbuf.mtype = (long) player->id;
    sprintf(sbuf.mtext, "DEAD");
    if (msgsnd(shmaddr->msqid, &sbuf, strlen(sbuf.mtext) + 1, IPC_NOWAIT) < 0) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
}

void sendMoveMessage(sharedMemory *shmaddr, player *player, int x, int y)
{
    message_buf sbuf;
    sbuf.mtype = (long) player->id;
    sprintf(sbuf.mtext, "MOVE %d %d",x, y);
    printf("msqid: %d, mtype: %ld, mtext: %s, size: %ld\n", 
        shmaddr->msqid, sbuf.mtype, sbuf.mtext, strlen(sbuf.mtext) + 1);
    if (msgsnd(shmaddr->msqid, &sbuf, strlen(sbuf.mtext) + 1, IPC_NOWAIT) < 0) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
}

void receiveMessage(sharedMemory *shmaddr, player *player)
{
    message_buf rbuf;
    if (msgrcv(shmaddr->msqid, &rbuf, MSGSZ, player->id, IPC_NOWAIT) < 0) {
        if (errno != ENOMSG) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        return;
    }
    printf("Received: %s DEAD : %d MOVE : %d\n", rbuf.mtext,ft_strncmp(rbuf.mtext, "DEAD", 4), ft_strncmp(rbuf.mtext, "MOVE", 4));
    int team, x, y;

    if (ft_strncmp(rbuf.mtext, "DEAD", 4) == 0)
    {
        player->isActive = false;
        player->willDie = false;
        
        shmaddr->teams[player->team].nPlayers--;
        printf("Player is dead\n");
        
        // shmaddr->map[shmaddr->teams[i].players[j]->x][shmaddr->teams[i].players[j]->y].shmaddr->teams[i].players[j] = NULL;
        if (shmaddr->teams[player->team].nPlayers == 0)
        {
            shmaddr->teams[player->team].isActive = false;
            printf("team dead\n");
        }
    }
    else if (ft_strncmp(rbuf.mtext, "MOVE", 4) == 0)
    {
        char *tmp = ft_split(rbuf.mtext, ' ')[1];
        int xMove = ft_atoi(tmp);
        free(tmp);
        tmp = ft_split(rbuf.mtext, ' ')[2];
        int yMove = ft_atoi(tmp);
        free(tmp);
        player->x = xMove;
        player->y = yMove;
        printf("Player moved x = %d, y = %d\n", player->x, player->y);
    }

}

