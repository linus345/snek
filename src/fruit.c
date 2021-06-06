#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include "../includes/app.h"
#include "../includes/fruit.h"
#include "../includes/game.h"
#include "../includes/snake.h"
#include "../includes/network.h"
#include "../includes/player.h"

Fruit *new_fruit(Fruit *fruits[], int random_x, int random_y, int random_type, Player *players[])
{
    Fruit* fruit = malloc(sizeof(Fruit));

    // generate random position
    fruit->pos.x = ((random_x % (GAME_WIDTH / CELL_SIZE)) * CELL_SIZE) + GAME_START_POS;
    fruit->pos.y = (random_y % (GAME_HEIGHT / CELL_SIZE)) * CELL_SIZE;
    // generate random fruit type
    fruit->type = random_type % 4;
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
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(players[i] == NULL)
            continue;

        // head
        if (fruit->pos.x == players[i]->snake->head.pos.x && fruit->pos.y == players[i]->snake->head.pos.y) {
            free(fruit);
            return NULL;
        }
        // body
        for (int j = 0; j < players[i]->snake->body_length; j++) {
            if (fruit->pos.x == players[i]->snake->body[j].pos.x && fruit->pos.y == players[i]->snake->body[j].pos.y) {
                free(fruit);
                return NULL;
            }
        }
        // tail
        if (fruit->pos.x == players[i]->snake->tail.pos.x && fruit->pos.y == players[i]->snake->tail.pos.y) {
            free(fruit);
            return NULL;
        }
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

void get_fruit_pos_and_spawn(Uint8 *data, Fruit *fruits[], int *nr_of_fruits, int nr_of_players, Player *players[])
{
    // get data from packet
    int type, random_x, random_y, random_type, fruit_index;
    // format: type rand_x rand_y fruit_type fruit_index
    sscanf((char *) data, "%d %d %d %d %d", &type, &random_x, &random_y, &random_type, &fruit_index);
    printf("received new fruit: \n");
    printf("x: %d, y: %d, type: %d, index: %d\n", random_x, random_y, random_type, fruit_index);

    /* if (*nr_of_fruits >= nr_of_players) { */
    /*     printf("nr_of_fruits >= nr_of_players"); */
    /*     return; */
    /* } */

    // create new fruit
    fruits[fruit_index] = new_fruit(fruits, random_x, random_y, random_type, players);
    if(fruits[fruit_index] != NULL) {
        printf("created new fruit\n");
        printf("x: %d, y: %d, type: %d\n", fruits[fruit_index]->pos.x, fruits[fruit_index]->pos.y, fruits[fruit_index]->type);
        // increment nr_of_fruits
        (*nr_of_fruits)++;
    }
}

void fruit_collision(App *app, UDPsocket socket, IPaddress server_addr, UDPpacket *pack_send, Player *players[], Fruit *fruits[], int *nr_of_fruits, int client_id)
{
    //check if snake head is at fruit position
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(fruits[i] != NULL && fruits[i]->pos.x == players[client_id]->snake->head.pos.x && fruits[i]->pos.y == players[client_id]->snake->head.pos.y) {
            // send event to other clients
            send_ate_fruit(socket, server_addr, pack_send, client_id, i);
            // update fruits by removing the one that was hit and add body part to snake
            update_state_if_fruit_collision(app, players[client_id], fruits, nr_of_fruits, i);
            // update scoreboard with new points
            /* update_scoreboard(app, players, game_state->scoreboard); */
        }
    }
}

void update_state_if_fruit_collision(App *app, Player *player, Fruit *fruits[], int *nr_of_fruits, int index)
{
    if(fruits[index] == NULL)
        return;

    // save points before removing fruit
    int points = fruits[index]->points;
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
    player->snake->head.mouth_eating = true;
    // add body part to snake
    player->snake->body[player->snake->body_length] = new_snake_body_part(&player->snake->body[player->snake->body_length-1].pos, 
            player->snake->body[player->snake->body_length-1].angle,
            &player->snake->body_length);
    // change snake speed
    if (player->snake->speed > 100) {
        player->snake->speed = (SPEED - (pow(player->snake->body_length, 2) * 2) / 4);
    } else if (player->snake->speed < 100) {
        player->snake->speed -= 2;
    } else if (player->snake->speed <= 0) {
        player->snake->speed = 0;
    }
    // update player score TODO
    player->points += points;
    
    // play eating sound effect
    if (!app->sound->muted) {
        play_sound(app->sound->eat); // plays eat sound effect
    }
}
