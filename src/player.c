#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../includes/player.h"

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
    player->color = GREEN;
    // set default name
    snprintf(player->name, MAX_NAME_LENGTH, "Player %d", id);

    return player;
}

void next_player_color(Player* player)
{
    // go to next color
    player->color++;
    // start over from 0 if at the end of colors
    if(player->color >= NR_OF_COLORS) {
        player->color = 0;
    }
    printf("color: %d\n", player->color);
}

void prev_player_color(Player* player)
{
    int temp_color = player->color;
    // go to previous color
    temp_color--;
    // go to last color if less than 0
    if(temp_color < 0) {
        temp_color = NR_OF_COLORS - 1;
    }
    player->color = temp_color;
}
