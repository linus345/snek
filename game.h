#ifndef GAME_H
#define GAME_H

#include "app.h"
#include "sound.h"
#include <SDL2/SDL.h>

#define CELL_SIZE 32

#define MAX_PLAYERS 4

typedef struct {
    int x;
    int y;
} Pos;

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

enum Finsh_scoreboard_dimensions {
    FSD_BUTTON_X = GAME_WIDTH/2-180,
    FSD_BUTTON_Y = 200,
    FSD_BUTTON_W = 400,
    FSD_BUTTON_H = 150,

    F_NAME_X = GAME_WIDTH/2-130,
    F_NAME_Y = 234,
    F_NAME_W = 130,
    F_NAME_H = 90,

    F_SCORE_X = GAME_WIDTH/2+40,
    F_SCORE_Y = 234,
    F_SCORE_W = 90,
    F_SCORE_H = 90,

    FY_OFFSET = 150
};

void
load_texture(App* app, SDL_Texture** texture, char* path);
void optimizeFullscreen(App* app, SDL_Rect* rect);
int game(App* app, Sound_effects* sound);
int scoreboard(App* app, Sound_effects* sound, int score);

#endif
