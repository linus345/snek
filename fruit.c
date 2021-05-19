#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include "app.h"
#include "fruit.h"
#include "game.h"
#include "snake.h"

Fruit *new_fruit(Fruit *fruits[], int random_x, int random_y, int random_type, int nr_of_fruits, Snake *snake)
{
    Fruit *fruit = malloc(sizeof(Fruit));

    // generate random position
    fruit->pos.x = (random_x % (WINDOW_WIDTH / CELL_SIZE)) * CELL_SIZE;
    fruit->pos.y = (random_y % (WINDOW_HEIGHT / CELL_SIZE)) * CELL_SIZE;
    // generate random fruit type
    fruit->type = random_type % 4;//rand() % 4;
    // different points based on fruit type
    switch(fruit->type) {
        case Cherry:
            fruit->points = 10;
            break;
        case Apple:
            fruit->points = 20;
            break;
        case Pear:
            fruit->points = 30;
            break;
        case Mango:
            fruit->points = 40;
            break;
    }
    // check if position overlaps with snake position
    // head
    if(fruit->pos.x == snake->head.pos.x && fruit->pos.y == snake->head.pos.y) {
        free(fruit);
        return NULL;
    }
    // body
    for(int i = 0; i < snake->body_length; i++) {
        if(fruit->pos.x == snake->body[i].pos.x && fruit->pos.y == snake->body[i].pos.y) {
            free(fruit);
            return NULL;
        }
    }
    // tail
    if(fruit->pos.x == snake->tail.pos.x && fruit->pos.y == snake->tail.pos.y) {
        free(fruit);
        return NULL;
    }
    // check if position overlaps with other fruit positions
    for(int i = 0; i < nr_of_fruits; i++) {
        if(fruit->pos.x == fruits[i]->pos.x && fruit->pos.y == fruits[i]->pos.y) {
            free(fruit);
            return NULL;
        }
    }
    
    return fruit;
}

void get_fruit_pos_and_spawn(UDPpacket *pack_recv, Fruit *fruits[], int *nr_of_fruits, Snake *snake)
{
    // get data from packet
    int type, random_x, random_y, random_type;
    sscanf(pack_recv->data, "%d %d %d %d", &type, &random_x, &random_y, &random_type);

    // create new fruit
    do {
        fruits[*nr_of_fruits] = new_fruit(fruits, random_x, random_y, random_type, *nr_of_fruits, snake);
    } while(fruits[*nr_of_fruits] == NULL);
    // increment nr_of_fruits
    (*nr_of_fruits)++;
}

bool fruit_collision(Snake *snake, Fruit *fruits[], int nr_of_fruits)
{
    //check if snake head is at fruit position
    for(int i = 0; i < nr_of_fruits; i++) {
        if(fruits[i]->pos.x == snake->head.pos.x && fruits[i]->pos.y == snake->head.pos.y) {
            snake->head.mouth_eating = true;
            return true;
        }
    }
    return false;
}

