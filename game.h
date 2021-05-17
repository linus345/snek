#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "app.h"

#define CELL_SIZE 32
#define MAX_PLAYERS 4

typedef struct {
    int x;
    int y;
} Pos;

typedef struct {
    int nr_of_players;
    int client_id;
    unsigned current_time;
    unsigned last_time;
    bool connected;
} Game_State;

Game_State *init_game_state();
void load_texture(App *app, SDL_Texture **texture, char *path);

#endif
