#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include "app.h"
#include "fruit.h"
#include "game.h"
#include "snake.h"

Fruit *rand_fruit(int nr_of_players, int *nr_of_fruits, Snake *snake, Fruit *fruits[])
{
    if (*nr_of_fruits >= nr_of_players)
    {
        return NULL;
    }

    //allocate memory on heap
    Fruit *fruit = malloc(sizeof(Fruit));

    bool check_pos = true;
    int fruit_type;
    if (*nr_of_fruits < nr_of_players)
    {
        fruit->pos.x = rand() % WINDOW_WIDTH;
        fruit->pos.y = rand() % WINDOW_HEIGHT;
        fruit_type = rand() % (NR_OF_FRUIT_TYPES-1);
        // place fruit at location not previously occupied by another fruit or snake
        // Check if the fruit spawns on a snake head
        if (fruit->pos.x == snake->head.pos.x && fruit->pos.y == snake->head.pos.y)
        {
            check_pos = false;
        }
        // Check if the fruit spawns on a snake body
        for (int j = 0; j < snake->body_length; j++)
        {
            if (fruit->pos.x == snake->body[j].pos.x && fruit->pos.y == snake->body[j].pos.y)
            {
                check_pos = false;
            }
        }
        // Check if the fruit spawns on a snake tail
        if (fruit->pos.x != snake->tail.pos.x && fruit->pos.y != snake->tail.pos.y)
        {
            check_pos = false;
        }
        // Check if the fruit spawns on another fruit
        for (int i = 0; i < *nr_of_fruits; i++)
        {
            if (fruit->pos.x == fruits[i]->pos.x && fruit->pos.y == fruits[i]->pos.y)
            {
                check_pos = false;
            }
        }

        if (check_pos) // No occurenses of snakes or fruits
        {
            fruits[*nr_of_fruits]->pos.x = fruit->pos.x;
            fruits[*nr_of_fruits]->pos.y = fruit->pos.y;
            fruits[*nr_of_fruits]->type = fruit_type;
            //place_fruit(fruits[i], int i);
            (*nr_of_fruits)++;
        }
    }
    return fruit;
}


bool fruit_collision(Snake *snake, Fruit *fruits[])
{
    if (fruits[0]->pos.x == snake->head.pos.x && fruits[0]->pos.y == snake->head.pos.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}