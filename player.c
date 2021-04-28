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
    // -1 is used to indicate that the player doesn't have a client_id
    player->client_id = -1;
    player->snake = new_snake(player_nr);

    return player;
}
