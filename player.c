#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "player.h"

Player *new_player(int id)
{
    Player *player = malloc(sizeof(Player));

    player->points = 0;
    player->alive = true;
    // -1 is used to indicate that the player doesn't have a client_id
    player->client_id = id;
    // initialize last received packet number to 0
    player->last_received_packet_nr = 0;
    player->packet_nr = 0;
    player->snake = new_snake(id);

    return player;
}
