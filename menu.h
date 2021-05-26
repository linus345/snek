#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"
#include "app.h"
#include "sound.h"

//Menu button creation struct consisting of SDL_Rect, SDL_Texture and SDL_Color.
typedef struct Screen_item {
    SDL_Rect rect;
    SDL_Texture *texture;
    SDL_Color color;
} Screen_item;

//Numreric values to determine menu outcome.
enum Menu_selection {
    MAIN_MENU = 0,
    SELECT_GAME = 10,
    HOST_MULTIPLAYER = 11,
    JOIN_MULTIPLAYER = 12,
    HIGH_SCORE = 20,
    SETTINGS = 30,
    START_GAME = 100,
    TYPE_NAME = 420
};

void menu_init(App* app, SDL_Texture* background, SDL_Texture* newGame, SDL_Texture* exit);
SDL_Color color_select (int selection);
Screen_item* menu_button_background (App* app, char resource[]);
//Screen_item* menu_button_text(App* app, char* text, TTF_Font* font, SDL_Color color);
Screen_item* input_text(App* app, Screen_item* item);
bool hover_state (Screen_item* button, int Mx, int My);

int main_menu(App* app);
int select_game_menu(App* app);
int join_multiplayer(App* app);
int host_multiplayer(App* app);
int high_score(App* app);
int settings(App* app);
int type_name(App* app);

#endif
