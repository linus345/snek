#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "app.h"
#include "sound.h"
#include "fruit.h"

#define CELL_SIZE 32
#define MAX_PLAYERS 4

/*
typedef struct {
    int x;
    int y;
} Pos;
*/

typedef struct {
    int nr_of_players;
    int client_id;
    unsigned current_time;
    unsigned last_time;
    bool connected;
    int nr_of_fruits;
    Fruit *fruits[MAX_PLAYERS];
} Game_State;

//void main_loop(App* app);
Game_State *init_game_state();
void load_texture(App* app, SDL_Texture** texture, char* path);
void main_loop(App* app, Game_State *game_state);
void optimizeFullscreen(App* app, SDL_Rect* rect);
int game(App* app, Game_State *game_state);
int scoreboard(App* app, int score);

#endif
