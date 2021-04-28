#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include "game.h"
#include "app.h"


//TODO fix defined constants for array lengths
typedef struct {
    SDL_Rect rect;
    SDL_Texture *texture, *background;
} Button;

void menu_init(App *app, SDL_Texture *background, SDL_Texture *newGame, SDL_Texture *exit);
Button *init_menu_button (App *app, int x, int y, int w, int h, char *resource[]);
Button *create_button(App *app, int x, int y, int w, int h, char *text, TTF_Font *font, SDL_Color color);

void render_button (App *app, Button *button);
#endif