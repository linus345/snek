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

    // position snake differently depending on player
    switch(player_nr) {
        // position snake top left, moving to the right
        case 1:
            snake->vel_x = snake->speed;
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
            snake->vel_y = snake->speed;
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
            snake->vel_x = -snake->speed;
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
            snake->vel_y = -snake->speed;
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
