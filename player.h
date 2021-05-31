#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "snake.h"

enum Colors {
    GREEN = 0,
    BLUE = 1,
    PURPLE = 2,
    YELLOW = 3,
    ORANGE = 4
};

typedef struct {
    int points;
    bool alive;
    int client_id;
    int last_received_packet_nr;
    int packet_nr;
    Snake *snake;
    char name[16];
    enum Colors color;
} Player;

Player *new_player(int id);

#endif
