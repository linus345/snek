#ifndef FRUIT_H
#define FRUIT_H

#include "game.h"

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

Fruit *rand_fruit(int nr_of_players, int *nr_of_fruits, Snake *snake, Fruit *fruits[]); 

bool fruit_collision(Snake *snake, Fruit *fruits[]);

#endif