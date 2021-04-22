#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "player.h"

Player *new_player(int host, int port, int player_nr)
{
    Player *player = malloc(sizeof(Player));

    player->points = 0;
    player->alive = true;
    player->host = host;
    player->port = port;
    player->snake = new_snake(player_nr);

    return player;
}