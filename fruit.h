#ifndef FRUIT_H
#define FRUIT_H

#include <SDL2/SDL_net.h>
#include "app.h"
#include "snake.h"

#define NR_OF_FRUIT_TYPES 4

enum Fruit_type {
    Cherry = 0,
    Apple = 1,
    Pear = 2,
    Mango = 3
};

typedef struct {
    Pos pos;
    enum Fruit_type type;
    int points;
} Fruit;

Fruit *new_fruit(Fruit *fruits[], int random_x, int random_y, int random_type, int nr_of_fruits, Snake *snake);
void get_fruit_pos_and_spawn(UDPpacket *pack_recv, Fruit *fruits[], int *nr_of_fruits, int nr_of_players, Snake *snake);
bool fruit_collision(Snake *snake, Fruit *fruits[], int nr_of_fruits);

#endif
