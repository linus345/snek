#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "snake.h"

typedef struct {
    int points;
    bool alive;
    int host;
    int port;
    int client_id;
    Snake *snake;
} Player;

Player *new_player(int host, int port, int player_nr);

#endif
