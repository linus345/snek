#ifndef FRUIT_H
#define FRUIT_H

#include "game.h"
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

Fruit *new_fruit(Fruit *fruits[], int nr_of_fruits, Snake *snake);
bool fruit_collision(Snake *snake, Fruit *fruits[], int nr_of_fruits);

#endif
