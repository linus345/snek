#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include "app.h"
#include "fruit.h"
#include "game.h"
#include "snake.h"
#include "network.h"

Fruit *new_fruit(Fruit *fruits[], int random_x, int random_y, int random_type, Snake *snake)
{
    Fruit* fruit = malloc(sizeof(Fruit));

    // generate random position
    fruit->pos.x = ((random_x % (GAME_WIDTH / CELL_SIZE)) * CELL_SIZE) + GAME_START_POS;
    fruit->pos.y = (random_y % (GAME_HEIGHT / CELL_SIZE)) * CELL_SIZE;
    // generate random fruit type
    fruit->type = random_type % 4;//rand() % 4;
    // different points based on fruit type
    switch (fruit->type) {
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
    if (fruit->pos.x == snake->head.pos.x && fruit->pos.y == snake->head.pos.y) {
        free(fruit);
        return NULL;
    }
    // body
    for (int i = 0; i < snake->body_length; i++) {
        if (fruit->pos.x == snake->body[i].pos.x && fruit->pos.y == snake->body[i].pos.y) {
            free(fruit);
            return NULL;
        }
    }
    // tail
    if (fruit->pos.x == snake->tail.pos.x && fruit->pos.y == snake->tail.pos.y) {
        free(fruit);
        return NULL;
    }
    // check if position overlaps with other fruit positions
    for(int i = 0; i < MAX_PLAYERS; i++) {
        // skip if NULL
        if(fruits[i] == NULL)
            continue;

        if(fruit->pos.x == fruits[i]->pos.x && fruit->pos.y == fruits[i]->pos.y) {
            free(fruit);
            return NULL;
        }
    }

    return fruit;
}

void get_fruit_pos_and_spawn(Uint8 *data, Fruit *fruits[], int *nr_of_fruits, int nr_of_players, Snake *snake)
{
    // get data from packet
    int type, random_x, random_y, random_type, fruit_index;
    // format: type rand_x rand_y fruit_type fruit_index
    sscanf((char *) data, "%d %d %d %d %d", &type, &random_x, &random_y, &random_type, &fruit_index);
    printf("received new fruit: \n");
    printf("x: %d, y: %d, type: %d, index: %d\n", random_x, random_y, random_type, fruit_index);

    if (*nr_of_fruits >= nr_of_players) {
        printf("nr_of_fruits >= nr_of_players");
        return;
    }

    // create new fruit TODO
    fruits[fruit_index] = new_fruit(fruits, random_x, random_y, random_type, snake);
    printf("created new fruit\n");
    printf("x: %d, y: %d, type: %d\n", fruits[fruit_index]->pos.x, fruits[fruit_index]->pos.y, fruits[fruit_index]->type);

    // increment nr_of_fruits
    (*nr_of_fruits)++;
}

void fruit_collision(App *app, UDPsocket socket, IPaddress server_addr, UDPpacket *pack_send, Snake *snake, Fruit *fruits[], int *nr_of_fruits, int client_id)
{
    //check if snake head is at fruit position
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(fruits[i] != NULL && fruits[i]->pos.x == snake->head.pos.x && fruits[i]->pos.y == snake->head.pos.y) {
            // send event to other clients
            send_ate_fruit(socket, server_addr, pack_send, client_id, i);
            // update fruits by removing the one that was hit and add body part to snake
            update_state_if_fruit_collision(app, snake, fruits, nr_of_fruits, i);
        }
    }
}

void update_state_if_fruit_collision(App *app, Snake *snake, Fruit *fruits[], int *nr_of_fruits, int index)
{
    // deallocate fruit from heap
    printf("removing fruit\n");
    printf("fruit_index: %d\n", index);
    printf("fruit.x: %d, fruit.y: %d, fruit.type: %d\n", fruits[index]->pos.x, fruits[index]->pos.y, fruits[index]->type);
    free(fruits[index]);
    fruits[index] = NULL;
    printf("removed fruit\n");
    // indicate that there is one less fruit
    (*nr_of_fruits)--;
    // change head texture
    snake->head.mouth_eating = true;
    // add body part to snake
    snake->body[snake->body_length] = new_snake_body_part(&snake->body[snake->body_length-1].pos, 
            snake->body[snake->body_length-1].angle,
            &snake->body_length);
    // change snake speed
    if (snake->speed > 100) {
        snake->speed = (SPEED - (pow(snake->body_length, 2) * 2) / 3);
    } else if (snake->speed < 100) {
        snake->speed -= 2;
    } else if (snake->speed >= 0) {
        snake->speed = 0;
    }
    // update player score TODO
    //player1->points += fruits[0]->type;
    //score = player1->points;
    //sprintf(buffer, "%d", player1->points);
    //player1_score = menu_button_text(app, buffer, font, white_txt);
    
    // play eating sound effect
    if (!app->sound->muted) {
        play_sound(app->sound->eat); // plays eat sound effect
    }
}
