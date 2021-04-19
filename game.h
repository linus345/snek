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

typedef struct {
    int angle;
    Pos pos;
} Body_Part;

void load_texture(App *app, SDL_Texture **texture, char *path);

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
    int angle;
    enum Dir dir;
    Body_Part head;
    Body_Part body[MAX_SNAKE_LENGTH];
    Body_Part tail;
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

#endif
