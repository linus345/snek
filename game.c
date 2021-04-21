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

void change_snake_velocity(Snake *snake)
{
    switch(snake->dir) {
        case Up:
            // check if head is aligned with the 32x32 grid, if it is, change velocity
            // otherwise, indicate that there is a velocity change that should take place
            // when it is aligned with the 32x32 grid.
            if(snake->head.pos.x % CELL_SIZE == 0 && snake->head.pos.y % CELL_SIZE == 0) {
                snake->vel_x = 0;
                snake->vel_y = -snake->speed;
                snake->next_dir = None;
                snake->head.angle = 0;
            } else {
                snake->next_dir = Up;
            }
            break;
        case Down:
            if(snake->head.pos.x % CELL_SIZE == 0 && snake->head.pos.y % CELL_SIZE == 0) {
                snake->vel_x = 0;
                snake->vel_y = snake->speed;
                snake->next_dir = None;
                snake->head.angle = 180;
            } else {
                snake->next_dir = Down;
            }
            break;
        case Left:
            if(snake->head.pos.x % CELL_SIZE == 0 && snake->head.pos.y % CELL_SIZE == 0) {
                snake->vel_x = -snake->speed;
                snake->vel_y = 0;
                snake->next_dir = None;
                snake->head.angle = 270;
            } else {
                snake->next_dir = Left;
            }
            break;
        case Right:
            if(snake->head.pos.x % CELL_SIZE == 0 && snake->head.pos.y % CELL_SIZE == 0) {
                snake->vel_x = snake->speed;
                snake->vel_y = 0;
                snake->next_dir = None;
                snake->head.angle = 90;
            } else {
                snake->next_dir = Right;
            }
            break;
        default:
            break;
    }
}

void new_snake_pos(Snake *snake)
{
    /*
     // body update
    snake->tail.pos.x += snake->head.pos.x + (snake->body_length+2)*CELL_SIZE;
    snake->tail.pos.y += snake->head.pos.y + (snake->body_length+2)*CELL_SIZE;  
    for(int i=snake->body_length; i>-1; i--){
        snake->body[i].pos.x = snake->body[i-1].pos.x + CELL_SIZE;
        snake->body[i].pos.y = snake->body[i-1].pos.y + CELL_SIZE;
    }
    */
    // head part update
    snake->head.pos.x += snake->vel_x;
    snake->head.pos.y += snake->vel_y;

    return;
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





/*
Fruit *rand_fruit(int nr_of_players, int nr_of_fruits, Snake *snakes[], Fruit *fruits[])
{
    //allocate memory on heap
    Fruit *fruit = malloc(sizeof(Fruit));

    if(nr_of_fruits < nr_of_players)
    {
        fruit->pos.x = rand() % WINDOW_WIDTH;
        fruit->pos.y = rand() % WINDOW_HEIGHT;
        //place fruit at location not previously occupied by another fruit or snake
        for (int i = 0; i < nr_of_fruits; i++) 
        {
            if(fruit->pos.x != fruits[i]->pos.x && fruit->pos.y != fruits[i]->pos.y)
            {
                if (fruit->pos.x != player1->snake->head->pos.x && fruit->pos.y != player1->snake->head->pos.y)
                { 
                    for(int j = 0; j < player1->snake->body_length; j++)
                    {
                        if(fruit->pos.x != player1->snake->body[j]->pos.x && fruit->pos.y != player1->snake->body[j]->pos.y)
                        {
                            if(fruit->pos.x != player1->snake->tail->pos.x && fruit->pos.y != player1->snake->tail->pos.y)
                            {
                                fruits[i]->pos.x = fruit->pos.x;                   
                                fruits[i]->pos.y = fruit->pos.y;
                                place_fruit(fruits[i]);
                                nr_of_fruits++; 
                            }
                        }
                    } 
                }
            }
        }
    }
    return fruit;
}

void place_fruit(Fruit *fruits[], Pos fruit_texture[], SDL_Texture fruit_sprite_tex)
{
    SDL_Rect fruit_src = {fruit_texture[0].x, fruit_texture[0].y, CELL_SIZE, CELL_SIZE};
    SDL_Rect fruit_dst = {fruit->pos.x, fruit->pos.y, CELL_SIZE, CELL_SIZE};
    SDL_RenderCopy(app->renderer, fruit_sprite_tex, &fruit_src, &fruit_dst, NULL, NULL);
}*/

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

