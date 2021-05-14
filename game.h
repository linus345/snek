#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "app.h"
#include "menu.h"
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

void load_texture(App *app, SDL_Texture **texture, char *path);
void render_item(App* app, SDL_Rect* rect, SDL_Texture* texture, int x, int y, int w, int h);
void optimizeFullscreen(App* app, SDL_Rect *rect);
int game(App* app);

#endif
