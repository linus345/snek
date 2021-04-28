#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include "game.h"
#include "app.h"

#define MAIN_MENU 0
#define SELECT_GAME 1
#define HIGH_SCORE 2
#define SETTINGS 3
#define START_GAME 4
//TODO fix defined constants for array lengths
typedef struct {
    SDL_Rect rect;
    SDL_Texture *texture, *background;
} Button;

void menu_init(App *app, SDL_Texture *background, SDL_Texture *newGame, SDL_Texture *exit);
Button *menu_button_background (App *app, int x, int y, int w, int h, char *resource[]);
Button *menu_button_text(App *app, int x, int y, int w, int h, char *text, TTF_Font *font, SDL_Color color);

void render_button (App *app, Button *button);
int main_menu (App *app);
int select_game_menu (App *app);
#endif