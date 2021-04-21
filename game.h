#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "app.h"
#define CELL_SIZE 32

#define MAX_PLAYERS 4

typedef struct {
    int x;
    int y;
} Pos;

void load_texture(App *app, SDL_Texture **texture, char *path);

#endif