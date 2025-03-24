#include "lemipc.h"

void	resize(int32_t width, int32_t height, void *param)
{
    screen	*screen;

    screen = (struct screen *)param;
    mlx_delete_image(screen->mlx, screen->img);
    if (!screen->img)
        exit(EXIT_FAILURE);
    if (width < height)
        width = height;
    else
        height = width;
    screen->img = mlx_new_image(screen->mlx, width, height);
    screen->width = width;
    screen->height = height;
    screen->moved = true;
    screen->resized = true;
    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        screen->shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    screen->shmaddr->changed = true;
    if (sem_post(sem) == -1) {
        perror("sem_post");
        screen->shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    mlx_image_to_window(screen->mlx, screen->img, 0, 0);
}

bool	someoneThere(sharedMemory *shmaddr, int x, int y)
{
    for (int i = 0; i < MAX_TEAM; i++)
    {
        if (shmaddr->teams[i].isActive == false)
            continue;
        for (int j = 0; j < MAX_PROCESSES; j++)
        {
            if (shmaddr->teams[i].players[j].isActive == true)
            {
                if (shmaddr->teams[i].players[j].x == x && shmaddr->teams[i].players[j].y == y)
                    return true;
            }
        }
    }
    return false;
}

void printMap(sharedMemory *shmaddr)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (someoneThere(shmaddr, i, j) == true)
                printf("x = %d, y = %d\n", i, j);
        }
    }
}

player *getPlayer(sharedMemory *shmaddr, int x, int y)
{
    // return shmaddr->map[x][y].player;
    for (int i = 0; i < MAX_TEAM; i++)
    {
        if (shmaddr->teams[i].isActive == false)
            continue;
        for (int j = 0; j < MAX_PROCESSES; j++)
        {
            if (shmaddr->teams[i].players[j].isActive == true)
            {
                if (shmaddr->teams[i].players[j].x == x && shmaddr->teams[i].players[j].y == y)
                    return &shmaddr->teams[i].players[j];
            }
        }
    }
    return NULL;
}

void unselectPlayer(sharedMemory *shmaddr)
{
    for (int i = 0; i < MAX_TEAM; i++)
    {
        if (shmaddr->teams[i].isActive == false)
            continue;
        for (int j = 0; j < MAX_PROCESSES; j++)
        {
            if (shmaddr->teams[i].players[j].isActive == true)
            {
                shmaddr->teams[i].players[j].isSelected = false;
            }
        }
    }
}

player *getIsSelected(sharedMemory *shmaddr)
{
    for (int i = 0; i < MAX_TEAM; i++)
    {
        if (shmaddr->teams[i].isActive == false)
            continue;
        for (int j = 0; j < MAX_PROCESSES; j++)
        {
            if (shmaddr->teams[i].players[j].isActive == true)
            {
                if (shmaddr->teams[i].players[j].isSelected == true)
                    return &shmaddr->teams[i].players[j];
            }
        }
    }
    return NULL;
}

void movePlayer(sharedMemory *shmaddr,player *player, int x, int y)
{
    // shmaddr->map[player->x][player->y].player = NULL;
    sendMoveMessage(shmaddr, player, x, y);
    // player->x = x;
    // player->y = y;
    // shmaddr->map[x][y].player = player;
    // printf("Player moved x = %d, y = %d\n", player->x, player->y);
}


bool validMove(sharedMemory *shmaddr, player *player, int x, int y)
{
    if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE)
        return false;
    if (abs(player->x - x) > 1 || abs(player->y - y) > 1)
        return false;
    if (abs(player->x - x) == 1 && abs(player->y - y) == 1)
        return false;
    if (someoneThere(shmaddr, x, y) == true)
        return false;
    return true;
}

void	cursor(double xpos, double ypos, void *param)
{
    screen		*screen = (struct screen *)param;
    sharedMemory		*shmaddr = screen->shmaddr;
    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    shmaddr->changed = true;
    if (sem_post(sem) == -1) {
        perror("sem_post");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    // the player is selected if the cursor is on the player
    // the player is in square in screen->width / MAP_SIZE
    // if (screen->isClicked == false)
    // 	return;
    // screen->isClicked = false;
    
    // xpos = floor((floor(xpos) / ((double) screen->width / MAP_SIZE))) * ((double) screen->width / MAP_SIZE);
    // ypos = floor((floor(ypos) / ((double) screen->height / MAP_SIZE))) * ((double) screen->height / MAP_SIZE);
    // xpos = (xpos / (double) screen->width) * MAP_SIZE;
    // ypos = (ypos / (double) screen->height) * MAP_SIZE;
    screen->x = xpos;
    screen->y = ypos;
    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    shmaddr->changed = true;
    if (sem_post(sem) == -1) {
        perror("sem_post");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    // write(1, "cursor\n", 7);
}

unsigned short int getNextTeam(sharedMemory *shmaddr, unsigned short int order)
{
    int j = 0;
    for (int i = 0; i < MAX_TEAM; i++)
    {
        if (order == shmaddr->order[i])
        {
            j = i;
            break;
        }
    }
    do
    {
        j = (j + 1) % MAX_TEAM;
    } while (shmaddr->order[j] == 5 || shmaddr->teams[j].isActive == false);
    // printf("Next team %d\n", shmaddr->order[j]);
    return (shmaddr->order[j]);
}

void	mousehook(mouse_key_t button, action_t action, modifier_key_t mods, \
        void *param)
{
    screen	*screen = (struct screen *)param;
    sharedMemory	*shmaddr = screen->shmaddr;
    double	xpos = screen->x, ypos = screen->y;
    (void)mods;
    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    shmaddr->changed = true;
    if (sem_post(sem) == -1) {
        perror("sem_post");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    xpos = floor((floor(xpos) / ((double) screen->width / MAP_SIZE))) * ((double) screen->width / MAP_SIZE);
    ypos = floor((floor(ypos) / ((double) screen->height / MAP_SIZE))) * ((double) screen->height / MAP_SIZE);
    xpos = (xpos / (double) screen->width) * MAP_SIZE;
    ypos = (ypos / (double) screen->height) * MAP_SIZE;
    if (button == MLX_MOUSE_BUTTON_LEFT && action != MLX_RELEASE)
    {	
        if (sem_wait(sem) == -1) {
            perror("sem_wait");
            shmaddr->criticalError = true;
            exit(EXIT_FAILURE);
        }
        shmaddr->changed = true;
        if (someoneThere(shmaddr, (int) xpos, (int) ypos) == false)
        {
            player *player = getIsSelected(shmaddr);
            // printf("Wich to play %d\n", shmaddr->wichToPlay);
            if (player != NULL && player->team == shmaddr->wichToPlay &&validMove(shmaddr, player, (int) xpos, (int) ypos) == true)
            {
                movePlayer(shmaddr,player, (int) xpos, (int) ypos);
                shmaddr->wichToPlay = getNextTeam(shmaddr, shmaddr->wichToPlay);
            }
            unselectPlayer(shmaddr);
            if (sem_post(sem) == -1) {
                perror("sem_post");
                shmaddr->criticalError = true;
                exit(EXIT_FAILURE);
            }
            return;
        }
        
        if (someoneThere(shmaddr, (int) xpos, (int) ypos) == true)
        {
            unselectPlayer(shmaddr);
            // getPlayer(shmaddr, (int) xpos, (int) ypos)->isSelected = true;
            player *player = getPlayer(shmaddr, (int) xpos, (int) ypos);
            // printf("Wich to play %d\n", shmaddr->wichToPlay);

            if (player->team == shmaddr->wichToPlay)
                player->isSelected = true;
        }

        if (sem_post(sem) == -1) {
            perror("sem_post");
            shmaddr->criticalError = true;
            exit(EXIT_FAILURE);
        }
    }
    // if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_RELEASE)
    // 	screen->camera.is_clicked = false;
}

void	keyhook(mlx_key_data_t keydata, void *param)
{
    screen	*screen = (struct screen *)param;
    sharedMemory	*shmaddr = screen->shmaddr;

    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    shmaddr->changed = true;
    if (sem_post(sem) == -1) {
        perror("sem_post");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    // if (keydata.key == MLX_KEY_D && keydata.action != MLX_RELEASE)
    // 	screen->camera.pos.x += speed;
    // if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_A
    // 	|| keydata.key == MLX_KEY_SPACE || keydata.key == MLX_KEY_LEFT_CONTROL
    // 	|| keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_S
    // 	|| keydata.key == MLX_KEY_Q)
    // 	screen->moved = true;
    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
    {
        mlx_close_window(screen->mlx);
    }
}

int get_rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void putCadrillage(screen *screen)
{
    unsigned short int cadrillage = screen->width / MAP_SIZE;

    for (int i = 0; i < screen->width; i ++)
    {
        for (int j = 0; j < screen->height; j ++)
        {
            if (i % cadrillage == 0 || j % cadrillage == 0)
                mlx_put_pixel(screen->img, i, j, get_rgba(255, 0, 0, 255));
            }
    }
}

int teamColor(unsigned short int team)
{
    // write(1, "teamColor\n", 10);
    switch (team)
    {
    case 0:
        return get_rgba(0, 0, 255, 255);
        break;
    case 1:
        return get_rgba(0, 255, 0, 255);
        break;
    case 2:
        return get_rgba(255, 0, 0, 255);
        break;
    case 3:
        return get_rgba(255, 255, 0, 255);
        break;
    default:
        return get_rgba(255, 255, 255, 255);
        break;
    }
    // write(1, "teamColor end\n", 10);

}

void drawSquare(screen *screen, int x0, int y0, unsigned short int team)
{
    unsigned short int cadrillage = screen->width / MAP_SIZE;
    unsigned short int radius = cadrillage / 4;

    x0 = x0 * cadrillage + cadrillage / 2;
    y0 = y0 * cadrillage + cadrillage / 2;

    for (int i = x0 - radius; i <= x0 + radius; i++)
    {
        for(int j = y0 - radius; j <= y0 + radius; j++)
        {
            mlx_put_pixel(screen->img, i, j, teamColor(team));
        }
    }


}

void putPlayer(screen *screen, sharedMemory *shmaddr)
{
    for (int i = 0; i < MAX_TEAM; i++)
    {
        if (shmaddr->teams[i].isActive == false)
            continue;
        for (int j = 0; j < MAX_PROCESSES; j++)
        {
            if (shmaddr->teams[i].players[j].isActive == true)
            {
                if (shmaddr->teams[i].players[j].isSelected == true)
                    drawSquare(screen, shmaddr->teams[i].players[j].x, shmaddr->teams[i].players[j].y, 4);
                else
                    drawSquare(screen, shmaddr->teams[i].players[j].x, shmaddr->teams[i].players[j].y, i);
            }
        }
    }
}

bool somethingChanged(sharedMemory *shmaddr)
{
    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    if (shmaddr->changed == true)
    {
        if (sem_post(sem) == -1) {
            perror("sem_post");
            shmaddr->criticalError = true;
            exit(EXIT_FAILURE);
        }
        return true;
    }
    if (sem_post(sem) == -1) {
        perror("sem_post");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    return false;
}

void printBlack(screen *screen)
{
    for (int i = 0; i < screen->width; i++)
    {
        for (int j = 0; j < screen->height; j++)
        {
            // if (i % (screen->width / MAP_SIZE) == 0 || j % (screen->height / MAP_SIZE) == 0)
            // 	continue;
            mlx_put_pixel(screen->img, i, j, get_rgba(0, 0, 0, 255));
        }
    }
}

bool shouldStop(sharedMemory *shmaddr)
{
    bool check = false;
    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    check = shmaddr->criticalError == true || shmaddr->end == true;
    // printf("Check %d %d %d\n",check,shmaddr->criticalError,shmaddr->end);
    if (sem_post(sem) == -1) {
        perror("sem_post");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    return check;
}

void	hook(void *mini)
{
    screen		*screen;
    sharedMemory	*shmaddr;

    screen = (struct screen *) mini;
    shmaddr = screen->shmaddr;
    while (somethingChanged(shmaddr) == false)
    {
        usleep(1000);
    }
    usleep(10000);
    if (shouldStop(shmaddr))
    {
        // printf("shoudl stop\n");
        mlx_close_window(screen->mlx);
    }
    // write(1, "hook\n", 5);
    printBlack(screen);
    putCadrillage(screen);
    putPlayer(screen, screen->shmaddr);
    
    checkAlive(shmaddr);
    checkTeamAlive(shmaddr);
    checkAtLeastTwoInOneTeam(shmaddr);

}

void closeScreen(void *param)
{
    screen	*screen = (struct screen *)param;
    sharedMemory	*shmaddr = screen->shmaddr;

    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
    shmaddr->end = true;
    shmaddr->changed = true;
    // printf("End by function\n");
    if (sem_post(sem) == -1) {
        perror("sem_post");
        shmaddr->criticalError = true;
        exit(EXIT_FAILURE);
    }
}

void launchGraphics(sharedMemory *shmaddr)
{
    // return;
    screen screen;
    screen.width = WIDTH;
    screen.height = HEIGHT;
    screen.shmaddr = shmaddr;
    screen.moved = false;
    screen.resized = false;
    screen.isClicked = false;
    // printf("Graphics launched\n");
    screen.mlx = mlx_init(screen.width, screen.height, "lemipc", true);
    if (!screen.mlx)
    {
        if (sem_wait(sem) == -1) {
            perror("sem_wait");
            exit(EXIT_FAILURE);
        }
        shmaddr->criticalError = true;
        if (sem_post(sem) == -1) {
            perror("sem_post");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
    }
    screen.img = mlx_new_image(screen.mlx, screen.width, screen.height);
    mlx_image_to_window(screen.mlx, screen.img, 0, 0);
    mlx_resize_hook(screen.mlx, &resize, &screen);
    mlx_loop_hook(screen.mlx, &hook, &screen);
    mlx_cursor_hook(screen.mlx, &cursor, &screen);
    mlx_key_hook(screen.mlx, &keyhook, &screen);
    mlx_mouse_hook(screen.mlx, &mousehook, &screen);
    mlx_loop(screen.mlx);
    // mlx_close_hook(screen.mlx, &closeScreen, &screen);
    mlx_delete_image(screen.mlx, screen.img);
    mlx_terminate(screen.mlx);
    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }
    shmaddr->end = true;
    if (sem_post(sem) == -1) {
        perror("sem_post");
        exit(EXIT_FAILURE);
    }
    // write(1, "Graphics terminated\n", 20);
}