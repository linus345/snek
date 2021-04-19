#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#define MAX_SNAKE_LENGTH 256
#define SPEED 3
#define CELL_SIZE 32

typedef struct {
    int x;
    int y;
} Pos;

enum Dir {
    Up,
    Down,
    Left,
    Right
};

typedef struct {
    int body_length;
    int speed;
    int vel_x;
    int vel_y;
    enum Dir dir;
    Pos head;
    Pos body[MAX_SNAKE_LENGTH];
    Pos tail;
} Snake;

Snake *new_snake(int player_nr);

typedef struct {
    int points;
    bool alive;
    int host;
    int port;
    Snake *snake;
} Player;

Player *new_player(int host, int port, int player_nr);

enum Fruit_type {
    Apple,
    Cherry,
    Pear,
    Mango
};

typedef struct {
    Pos pos;
    Fruit_type type;
    int points;
} Fruit;

Fruit *rand_fruit(int nr_of_players, Snake *snakes[], Fruit *previous_fruit); 

#endif
