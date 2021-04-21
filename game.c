#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include "app.h"
#include "game.h"

Snake *new_snake(int player_nr)
{
    // allocate memory on heap
    Snake *snake = malloc(sizeof(Snake));

    // snake initially has 1 body part + head and tail
    snake->body_length = 1;
    // initialize start speed
    snake->speed = SPEED;
    snake->next_dir = None;
    snake->head.has_turned = false;
    snake->body[0].is_turn = false;
    snake->body[0].should_flip_vertical = false;
    snake->body[0].should_flip_horizontal = false;
    snake->body[0].turn_rotation = 0;

    // position snake differently depending on player
    switch(player_nr) {
        // position snake top left, moving to the right
        case 1:
            snake->vel_x = CELL_SIZE;
            snake->vel_y = 0;
            snake->head.angle = 90;
            snake->body[0].angle = 90;
            snake->tail.angle = 90;
            snake->head.pos.x = 100;
            snake->head.pos.y = CELL_SIZE;
            snake->dir = Right;
            snake->body[0].pos.x = snake->head.pos.x - CELL_SIZE;
            snake->body[0].pos.y = snake->head.pos.y;
            snake->tail.pos.x = snake->head.pos.x - CELL_SIZE * 2;
            snake->tail.pos.y = snake->head.pos.y;
            break;
        // position snake top right, moving to downwards
        case 2:
            snake->vel_x = 0;
            snake->vel_y = CELL_SIZE;
            snake->head.angle = 180;
            snake->body[0].angle = 180;
            snake->tail.angle = 180;
            snake->head.pos.x = WINDOW_WIDTH - 100;
            snake->head.pos.y = CELL_SIZE * 3;
            snake->dir = Down;
            snake->body[0].pos.x = snake->head.pos.x;
            snake->body[0].pos.y = snake->head.pos.y - CELL_SIZE;
            snake->tail.pos.x = snake->head.pos.x;
            snake->tail.pos.y = snake->head.pos.y - CELL_SIZE * 2;
            break;
        // position snake bottom right, moving to the left
        case 3:
            snake->vel_x = CELL_SIZE;
            snake->vel_y = 0;
            snake->head.angle = 270;
            snake->body[0].angle = 270;
            snake->tail.angle = 270;
            snake->head.pos.x = WINDOW_WIDTH - CELL_SIZE * 3;
            snake->head.pos.y = WINDOW_HEIGHT - 100;
            snake->dir = Left;
            snake->body[0].pos.x = snake->head.pos.x + CELL_SIZE;
            snake->body[0].pos.y = snake->head.pos.y;
            snake->tail.pos.x = snake->head.pos.x + CELL_SIZE * 2;
            snake->tail.pos.y = snake->head.pos.y;
            break;
        // position snake bottom left, moving upwards
        case 4:
            snake->vel_x = 0;
            snake->vel_y = CELL_SIZE;
            snake->head.angle = 0;
            snake->body[0].angle = 0;
            snake->tail.angle = 0;
            snake->head.pos.x = 100;
            snake->head.pos.y = WINDOW_HEIGHT - CELL_SIZE * 3;
            snake->dir = Up;
            snake->body[0].pos.x = snake->head.pos.x;
            snake->body[0].pos.y = snake->head.pos.y + CELL_SIZE;
            snake->tail.pos.x = snake->head.pos.x;
            snake->tail.pos.y = snake->head.pos.y + CELL_SIZE * 2;
            break;
    }

    return snake;
}

void change_snake_velocity(Snake *snake)
{
    switch(snake->dir) {
        case Up:
            snake->vel_x = 0;
            snake->vel_y = -CELL_SIZE;
            snake->next_dir = None;
            snake->head.angle = 0;
            snake->head.has_turned = true;
            snake->next_dir = Up;
            break;
        case Down:
            snake->vel_x = 0;
            snake->vel_y = CELL_SIZE;
            snake->next_dir = None;
            snake->head.angle = 180;
            snake->head.has_turned = true;
            snake->next_dir = Down;
            break;
        case Left:
            snake->vel_x = -CELL_SIZE;
            snake->vel_y = 0;
            snake->next_dir = None;
            snake->head.angle = 270;
            snake->head.has_turned = true;
            snake->next_dir = Left;
            break;
        case Right:
            snake->vel_x = CELL_SIZE;
            snake->vel_y = 0;
            snake->next_dir = None;
            snake->head.angle = 90;
            snake->head.has_turned = true;
            snake->next_dir = Right;
            break;
        default:
            break;
    }
}

void new_snake_pos(Snake *snake)
{
    // update tail position
    snake->tail.pos.x = snake->body[snake->body_length-1].pos.x;
    snake->tail.pos.y = snake->body[snake->body_length-1].pos.y;
    snake->tail.angle = snake->body[snake->body_length-1].angle;

    // update body position
    for(int i = snake->body_length; i > 0; i--) {
        snake->body[i].pos.x = snake->body[i-1].pos.x;
        snake->body[i].pos.y = snake->body[i-1].pos.y;
        snake->body[i].angle = snake->body[i-1].angle;

        snake->body[i].is_turn = snake->body[i-1].is_turn;
        snake->body[i].should_flip_vertical = snake->body[i-1].should_flip_vertical;
        snake->body[i].should_flip_horizontal = snake->body[i-1].should_flip_horizontal;
        snake->body[i].turn_rotation = snake->body[i-1].turn_rotation;

        snake->body[i-1].is_turn = false;
        snake->body[i-1].should_flip_vertical = false;
        snake->body[i-1].should_flip_horizontal = false;
        snake->body[i-1].turn_rotation = 0;
    }
    switch(snake->body[0].angle) {
        case 0: // old direction was up
            if(snake->dir == Left) {
                snake->body[0].is_turn = true;
                snake->body[0].should_flip_vertical = false;
                snake->body[0].should_flip_horizontal = false;
                snake->body[0].turn_rotation = 0;
            } else if (snake->dir == Right) {
                snake->body[0].is_turn = true;
                snake->body[0].should_flip_vertical = false;
                snake->body[0].should_flip_horizontal = true;
                snake->body[0].turn_rotation = 0;
            }
            break;
        case 90: // old direction was right
            break;
        case 180: // old direction was down
            break;
        case 270: // old direction was left
            break;
    }
    /* snake->body[0].is_turn = snake->head.has_turned; */
    snake->body[0].pos.x = snake->head.pos.x;
    snake->body[0].pos.y = snake->head.pos.y;
    snake->body[0].angle = snake->head.angle;
    // update head position
    snake->head.pos.x += snake->vel_x;
    snake->head.pos.y += snake->vel_y;
    snake->head.has_turned = false;
}

Player *new_player(int host, int port, int player_nr)
{
    Player *player = malloc(sizeof(Player));

    player->points = 0;
    player->alive = true;
    player->host = host;
    player->port = port;
    player->snake = new_snake(player_nr);

    return player;
}

void load_texture(App *app, SDL_Texture **texture, char *path)
{
    printf("Loading texture: %s\n", path);
    SDL_Surface *surface = IMG_Load(path);
    if(!surface) {
        fprintf(stderr, "error creating surface\n");
        exit(EXIT_FAILURE);
    }

    *texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    SDL_FreeSurface(surface);
    if(!*texture) {
        fprintf(stderr, "error creating texture\n");
        exit(EXIT_FAILURE);
    }
}
