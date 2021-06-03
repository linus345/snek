#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "snake.h"

#define MAX_NAME_LENGTH 20

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
    char name[MAX_NAME_LENGTH];
    enum Colors color;
} Player;

Player *new_player(int id);

#endif
