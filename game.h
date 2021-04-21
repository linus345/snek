#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#define MAX_SNAKE_LENGTH 256
#define SPEED 300      // ms
#define CELL_SIZE 32

typedef struct {
    int x;
    int y;
} Pos;

typedef struct {
    int angle;
    bool is_turn;
    bool should_flip_vertical;
    bool should_flip_horizontal;
    int turn_rotation;
    Pos pos;
} Body_Part;

typedef struct {
    int angle;
    bool has_turned;
    Pos pos;
} Head_Part;

void load_texture(App *app, SDL_Texture **texture, char *path);

enum Dir {
    None, // None is used to indicate that there is no next direction for snake.next_dir
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
    enum Dir next_dir; // used to wait for snake to be aligned with 32x32 grid before changing velocity
    Head_Part head;
    Body_Part body[MAX_SNAKE_LENGTH];
    Body_Part tail;
} Snake;

Snake *new_snake(int player_nr);
void change_snake_velocity(Snake *snake);
void new_snake_pos(Snake *snake);
void snake_texture_rotation(Snake *snake);

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
    enum Fruit_type type;
    int points;
} Fruit;

Fruit *rand_fruit(int nr_of_players, int nr_of_fruits, Snake *snakes[], Fruit *fruits[]); 


#endif
