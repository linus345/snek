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

void load_texture(App *app, SDL_Texture **texture, char *path);
int game(App* app, char* name);
void render_item(App* app, SDL_Rect* rect, SDL_Texture* texture, int x, int y, int w, int h);
void optimizeFullscreen(App* app, SDL_Rect *rect);

#endif
