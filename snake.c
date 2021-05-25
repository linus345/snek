#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "app.h"
#include "game.h"
#include "snake.h"
#include "fruit.h"

// TODO: BANDAID FIX, MUST FIX!!! FIX MUCH IMPORTANT!!!!!!!!!
void head_adjecent_with_fruit(Head_Part *head, Fruit *fruits[], int nr_of_fruits);

Snake *new_snake(int id)
{
    // allocate memory on heap
    Snake *snake = malloc(sizeof(Snake));

    printf("new_snake id: %d\n", id);
    snake->body_length = 0;
    // initialize start speed
    snake->speed = SPEED;
    snake->next_dir = None;
    snake->head.has_turned = false;
    snake->head.next_pos.x = -1;
    snake->head.next_pos.y = -1;

    // position snake differently depending on player
    switch(id) {
        // position snake top left, moving to the right
        case 0:
            snake->vel_x = CELL_SIZE;
            snake->vel_y = 0;
            snake->head.angle = 90;
            snake->tail.angle = 90;
            snake->head.pos.x = (CELL_SIZE * 3) + GAME_START_POS;
            snake->head.pos.y = CELL_SIZE;
            snake->dir = Right;
            snake->body[snake->body_length] = new_snake_body_part(&snake->head.pos, snake->head.angle, &snake->body_length);
            snake->tail.pos.x = snake->head.pos.x - CELL_SIZE * 2;
            snake->tail.pos.y = snake->head.pos.y;
            break;
        // position snake top right, moving to downwards
        case 1:
            snake->vel_x = 0;
            snake->vel_y = CELL_SIZE;
            snake->head.angle = 180;
            snake->body[0].angle = 180;
            snake->tail.angle = 180;
            snake->head.pos.x = WINDOW_WIDTH - CELL_SIZE * 3;
            snake->head.pos.y = CELL_SIZE * 3;
            snake->dir = Down;
            snake->body[snake->body_length] = new_snake_body_part(&snake->head.pos, snake->head.angle, &snake->body_length);
            snake->tail.pos.x = snake->head.pos.x;
            snake->tail.pos.y = snake->head.pos.y - CELL_SIZE * 2;
            break;
        // position snake bottom right, moving to the left
        case 2:
            snake->vel_x = CELL_SIZE;
            snake->vel_y = 0;
            snake->head.angle = 270;
            snake->body[0].angle = 270;
            snake->tail.angle = 270;
            snake->head.pos.x = WINDOW_WIDTH - CELL_SIZE * 3;
            snake->head.pos.y = WINDOW_HEIGHT - CELL_SIZE * 3;
            snake->dir = Left;
            snake->body[snake->body_length] = new_snake_body_part(&snake->head.pos, snake->head.angle, &snake->body_length);
            snake->tail.pos.x = snake->head.pos.x + CELL_SIZE * 2;
            snake->tail.pos.y = snake->head.pos.y;
            break;
        // position snake bottom left, moving upwards
        case 3:
            snake->vel_x = 0;
            snake->vel_y = CELL_SIZE;
            snake->head.angle = 0;
            snake->body[0].angle = 0;
            snake->tail.angle = 0;
            snake->head.pos.x = (CELL_SIZE * 3) + GAME_START_POS;
            snake->head.pos.y = WINDOW_HEIGHT - CELL_SIZE * 3;
            snake->dir = Up;
            snake->body[snake->body_length] = new_snake_body_part(&snake->head.pos, snake->head.angle, &snake->body_length);
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

void new_snake_pos(Snake *snake, bool should_update_head)
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
            if(snake->dir == Up) {
                snake->body[0].is_turn = true;
                snake->body[0].should_flip_vertical = false;
                snake->body[0].should_flip_horizontal = false;
                snake->body[0].turn_rotation = 90;
            } else if (snake->dir == Down) {
                snake->body[0].is_turn = true;
                snake->body[0].should_flip_vertical = false;
                snake->body[0].should_flip_horizontal = true;
                snake->body[0].turn_rotation = 90;
            }
            break;
        case 180: // old direction was down
            if(snake->dir == Left) {
                snake->body[0].is_turn = true;
                snake->body[0].should_flip_vertical = true;
                snake->body[0].should_flip_horizontal = false;
                snake->body[0].turn_rotation = 0;
            } else if (snake->dir == Right) {
                snake->body[0].is_turn = true;
                snake->body[0].should_flip_vertical = false;
                snake->body[0].should_flip_horizontal = false;
                snake->body[0].turn_rotation = 180;
            }
            break;
        case 270: // old direction was left
            if(snake->dir == Up) {
                snake->body[0].is_turn = true;
                snake->body[0].should_flip_vertical = true;
                snake->body[0].should_flip_horizontal = false;
                snake->body[0].turn_rotation = 90;
            } else if (snake->dir == Down) {
                snake->body[0].is_turn = true;
                snake->body[0].should_flip_vertical = false;
                snake->body[0].should_flip_horizontal = false;
                snake->body[0].turn_rotation = 270;
            }
            break;
    }
    /* snake->body[0].is_turn = snake->head.has_turned; */
    snake->body[0].pos.x = snake->head.pos.x;
    snake->body[0].pos.y = snake->head.pos.y;
    snake->body[0].angle = snake->head.angle;

    // update head position
    if(should_update_head) {
        snake->head.pos.x += snake->vel_x;
        snake->head.pos.y += snake->vel_y;
    }
    snake->head.has_turned = false;
    snake->head.mouth_open = false;
    snake->head.mouth_eating = false;
}

Body_Part new_snake_body_part(Pos *last_body_part_pos, int angle, int *body_length)
{
    Body_Part body;

    body.is_turn = false;
    body.should_flip_vertical = false;
    body.should_flip_horizontal = false;
    body.turn_rotation = 0;

    body.angle = angle;
    switch(body.angle) {
        case 0: // going up
            body.pos.x = last_body_part_pos->x;
            body.pos.y = last_body_part_pos->y + CELL_SIZE;
            break;
        case 90: // going right
            body.pos.x = last_body_part_pos->x - CELL_SIZE;
            body.pos.y = last_body_part_pos->y;
            break;
        case 180: // going down
            body.pos.x = last_body_part_pos->x;
            body.pos.y = last_body_part_pos->y - CELL_SIZE;
            break;
        case 270: // going left
            body.pos.x = last_body_part_pos->x + CELL_SIZE;
            body.pos.y = last_body_part_pos->y;
            break;
    }

    (*body_length)++;
    return body;
}

bool collison_with_wall(Snake *snake)
{
        // Checks collison with walls
    if (snake->head.pos.x < GAME_START_POS || snake->head.pos.y < 0 || snake->head.pos.x > WINDOW_WIDTH - CELL_SIZE || snake->head.pos.y > WINDOW_HEIGHT - CELL_SIZE) {
        // Collison detected
        return true; 
    }
    else
    return false;
}

bool collison_with_snake(Snake *snake)
{
    // Checks collison with snake body
    for (int i = 0; i < snake->body_length; i++) {
        if (snake->head.pos.x == snake->body[i].pos.x && snake->head.pos.y == snake->body[i].pos.y) {
        // Collison detected
        return true; 
        }
    }
    // Checks collison with snake tail
    if (snake->head.pos.x == snake->tail.pos.x && snake->head.pos.y == snake->tail.pos.y) {
        // Collison detected
        return true; 
    }
    // No collison detected
    return false;
}

void head_adjecent_with_fruit(Head_Part *head, Fruit *fruits[], int nr_of_fruits)
{
    for(int i = 0; i < nr_of_fruits; i++) {
        if (fruits[i] != NULL && fruits[i]->pos.x == head->pos.x && (fruits[i]->pos.y-CELL_SIZE == head->pos.y || fruits[i]->pos.y+CELL_SIZE == head->pos.y)) {
            head->mouth_open = true;
        } else if (fruits[i] != NULL && fruits[i]->pos.y == head->pos.y && (fruits[i]->pos.x-CELL_SIZE == head->pos.x || fruits[i]->pos.x+CELL_SIZE == head->pos.x)) {
            head->mouth_open = true;
        }
    }
}
