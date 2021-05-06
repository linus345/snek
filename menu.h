#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
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

enum Color_pallet {
    BLACK = 0,
    GRAY = 127,
    WHITE = 255,
    GREEN = 1,
    DARK_GREEN = 2
    
};

typedef struct {
    SDL_Rect rect;
    SDL_Texture *texture, *background;
    SDL_Color color;
} Button;

void menu_init(App *app, SDL_Texture *background, SDL_Texture *newGame, SDL_Texture *exit);
SDL_Color color_select (int selection);
Button *menu_button_background (App *app, int x, int y, int w, int h, char *resource[]);
Button *menu_button_text(App *app, int x, int y, int w, int h, char *text, TTF_Font *font, SDL_Color color);
void render_button (App *app, Button *button);
bool hover_state (Button *button, int Mx, int My);

void port_ip_input (App *app, char input[], int x, int y, int w, int h, bool ip_not_port);
int main_menu (App *app, SDL_Rect *r, bool *fullscreen);
int select_game_menu (App *app);
int join_multiplayer (App *app, char *ip_adress, char *port_nr);
int host_multiplayer (App *app);
int high_score (App *app);
int settings (App *app);

#endif