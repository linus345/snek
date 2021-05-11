#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"
#include "app.h"

typedef struct {
    SDL_Rect rect;
    SDL_Texture *texture, *background;
    SDL_Color color;
} Button;

enum Menu_selection {
    MAIN_MENU = 0,
    SELECT_GAME = 10,
    HOST_MULTIPLAYER = 11,
    JOIN_MULTIPLAYER = 12,
    HIGH_SCORE = 20,
    SETTINGS = 30,
    START_GAME = 100
};

enum Button_dimensions {
    BUTTON_X = 300,
    BUTTON_Y = 400,
    BUTTON_W = 360,
    BUTTON_H = 150,
    TEXT_X = 337,
    TEXT_Y = 430,
    TEXT_W = 290,
    TEXT_H = 90,
    EXIT_X = 380,
    EXIT_Y = 865,
    EXIT_W = 290,
    EXIT_H = 90
};

void menu_init(App *app, SDL_Texture *background, SDL_Texture *newGame, SDL_Texture *exit);
SDL_Color color_select (int selection);
Button *menu_button_background (App *app, char resource[]);
Button *menu_button_text(App *app, char *text, TTF_Font *font, SDL_Color color);
bool hover_state (Button *button, int Mx, int My);

void menu(App* app, char *ip_address, char *port_nr, bool *fullscreen_bool);
void port_ip_input (App *app, char input[], bool ip_not_port, bool *fullscreen_bool);
int main_menu (App* app, SDL_Rect* fullscreen, bool* fullscreen_bool);
int select_game_menu (App *app, bool *fullscreen_bool);
int join_multiplayer (App *app, char *ip_adress, char *port_nr, bool *fullscreen_bool);
int host_multiplayer (App *app, bool *fullscreen_bool);
int high_score (App *app, bool *fullscreen_bool);
int settings (App *app, bool *fullscreen_bool);

#endif