#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "app.h"
#include "sound.h"

#define CELL_SIZE 32

#define MAX_PLAYERS 4

typedef struct {
    int x;
    int y;
} Pos;

enum Render_item_type {
    UNSPECIFIED = 0,
    BACKGROUND = 1,
    MENU_BUTTON = 2
};

enum Scoreboard_dimensions {
    SD_BUTTON_X = 0,
    SD_BUTTON_Y = 50,
    SD_BUTTON_W = 250,
    SD_BUTTON_H = 100,

    NAME_X = 15,
    NAME_Y = 65,
    NAME_W = 100,
    NAME_H = 70,

    SCORE_X = 150,
    SCORE_Y = 65,
    SCORE_W = 70,
    SCORE_H = 70,
    
    Y_OFFSET = 100
};

void load_texture(App *app, SDL_Texture **texture, char *path);
void optimizeFullscreen(App* app, SDL_Rect *rect);
int game(App* app, Sound_effects* sound);

#endif
