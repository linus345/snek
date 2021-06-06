#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"
#include "app.h"
#include "sound.h"

//Numreric values to determine menu outcome.
enum Menu_selection {
    MAIN_MENU = 0,
    SELECT_GAME = 1,
    HOST_MULTIPLAYER = 2,
    JOIN_MULTIPLAYER = 3,
    HIGH_SCORE = 4,
    SETTINGS = 5,
    START_GAME = 6,
    TYPE_NAME = 7,
    LOBBY = 8
};

void menu_init(App* app, SDL_Texture* background, SDL_Texture* newGame, SDL_Texture* exit);
SDL_Color color_select (int selection);
Screen_item* menu_button_background (App* app, char resource[]);
//Screen_item* menu_button_text(App* app, char* text, TTF_Font* font, SDL_Color color);
Screen_item* input_text(App* app, Screen_item* item);
bool hover_state (Screen_item* button, int Mx, int My);
bool is_hovering_over(SDL_Rect* rect, int mouse_x, int mouse_y);

int main_menu(App* app, TTF_Font* font);
int select_game_menu(App* app, TTF_Font* font);
int join_multiplayer(App* app, TTF_Font* font);
int host_multiplayer(App* app);
int high_score(App* app);
int settings(App* app);
int type_name(App* app, TTF_Font* font);

#endif
