#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "snake.h"

typedef struct {
    int points;
    bool alive;
    int client_id;
    int last_received_packet_nr;
    int packet_nr;
    Snake *snake;
    char name[16];
} Player;

Player *new_player(int id);

#endif
