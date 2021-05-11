#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include "game.h"
#include "app.h"

enum Menu_selection {
    MAIN_MENU = 0,
    SELECT_GAME = 10,
    HOST_MULTIPLAYER = 11,
    JOIN_MULTIPLAYER = 12,
    HIGH_SCORE = 20,
    SETTINGS = 30,
    START_GAME = 100
};
/*
typedef struct {
    SDL_Color color;
} Color;
*/
typedef struct {
    SDL_Rect rect;
    SDL_Texture *texture, *background;
    SDL_Color color;
} Button;

void menu_init(App *app, SDL_Texture *background, SDL_Texture *newGame, SDL_Texture *exit);
SDL_Color color_select (int selection);
Button *menu_button_background (App *app, char resource[]);
Button *menu_button_text(App *app, char *text, TTF_Font *font, SDL_Color color);
void render_button (App* app, Button* button, bool *fullscreen_bool);
bool hover_state (Button *button, int Mx, int My);

<<<<<<< HEAD
void menu(App* app, char *ip_address, char *port_nr);
void port_ip_input (App *app, char input[], bool ip_not_port, bool *fullscreen_bool);
int main_menu (App* app, SDL_Rect* fullscreen, bool* fullscreen_bool);
int select_game_menu (App *app, bool *fullscreen_bool);
int join_multiplayer (App *app, char *ip_adress, char *port_nr, bool *fullscreen_bool);
int host_multiplayer (App *app, bool *fullscreen_bool);
int high_score (App *app, bool *fullscreen_bool);
int settings (App *app, bool *fullscreen_bool);
=======
void port_ip_input (App *app, char input[], int x, int y, int w, int h, bool ip_not_port);
int main_menu (App *app, SDL_Rect *r);
int select_game_menu (App *app);
int join_multiplayer (App *app, char *ip_adress, char *port_nr);
int host_multiplayer (App *app);
int high_score (App *app);
int settings (App *app);
>>>>>>> parent of 7b89b1b (Testing)

#endif