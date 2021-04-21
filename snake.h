#ifndef SNAKE_H
#define SNAKE_H

#include <stdbool.h>
#include "game.h"
#define MAX_SNAKE_LENGTH 256
#define SPEED 500      // ms

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
Body_Part new_snake_body_part(Pos *last_body_part_pos, int angle, int *body_length);

#endif
