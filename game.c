#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
            snake->head.x = 100;
            snake->head.y = CELL_SIZE;
            snake->dir = Right;
            snake->body[0].x = snake->head.x - CELL_SIZE;
            snake->body[0].y = snake->head.y;
            snake->tail.x = snake->head.x - CELL_SIZE * 2;
            snake->tail.y = snake->head.y;
            break;
        // position snake top right, moving to downwards
        case 2:
            snake->vel_x = 0;
            snake->vel_y = snake->speed;
            snake->head.x = WINDOW_WIDTH - 100;
            snake->head.y = CELL_SIZE * 3;
            snake->dir = Down;
            snake->body[0].x = snake->head.x;
            snake->body[0].y = snake->head.y - CELL_SIZE;
            snake->tail.x = snake->head.x;
            snake->tail.y = snake->head.y - CELL_SIZE * 2;
            break;
        // position snake bottom right, moving to the left
        case 3:
            snake->vel_x = -snake->speed;
            snake->vel_y = 0;
            snake->head.x = WINDOW_WIDTH - CELL_SIZE * 3;
            snake->head.y = WINDOW_HEIGHT - 100;
            snake->dir = Left;
            snake->body[0].x = snake->head.x + CELL_SIZE;
            snake->body[0].y = snake->head.y;
            snake->tail.x = snake->head.x + CELL_SIZE * 2;
            snake->tail.y = snake->head.y;
            break;
        // position snake bottom left, moving upwards
        case 4:
            snake->vel_x = 0;
            snake->vel_y = -snake->speed;
            snake->head.x = 100;
            snake->head.y = WINDOW_HEIGHT - CELL_SIZE * 3;
            snake->dir = Up;
            snake->body[0].x = snake->head.x;
            snake->body[0].y = snake->head.y + CELL_SIZE;
            snake->tail.x = snake->head.x;
            snake->tail.y = snake->head.y + CELL_SIZE * 2;
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

Fruit *rand_fruit(int nr_of_players, Snake *snakes[], Fruit *previous_fruit)
{
    //allocate memory on heap
    Fruit *fruit = malloc(sizeof(Fruit));

    while(nr_of_players > 0)
    {
        if(nr_of_fruits <= nr_of_players)
        {
            fruit->pos.x = rand() % WINDOW_WIDTH;
            fruit->pos.y = rand() % WINDOW_HEIGHT;
            place_fruit(fruit->pos.x, fruit->pos.y1);  
            nr_of_fruits++;
            previous_fruit.x1 = cherry.x1;
            previous_fruit.y1 = cherry.y1;
        }
    }
}

