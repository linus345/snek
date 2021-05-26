#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "app.h"
#include "fruit.h"
#include "game.h"
#include "menu.h"
#include "player.h"

//Standard dimensions for menu button backgrounds and texts.
enum Button_dimensions {
    BUTTON_Y = 400,
    BUTTON_W = 360,
    BUTTON_X = 450,
    BUTTON_H = 150,
    TEXT_Y = 430,
    TEXT_W = 290,
    TEXT_X = 480,
    TEXT_H = 90,
};

enum Render_item_type {
    UNSPECIFIED = 0,
    BACKGROUND = 1,
    MENU_BUTTON = 2,
    SNAKE = 3,
    STRETCH = 4
};

typedef struct menu {
    Screen_item* background;
    Screen_item* button;
    Screen_item* text1;
    Screen_item* text2;
    Screen_item* text3;
    Screen_item* return_button;
} Menu;

typedef struct scoreboard {
    Screen_item* background;
    Screen_item* scoreboard;
    Screen_item* name1;
    Screen_item* name2;
    Screen_item* name3;
    Screen_item* name4;
    Screen_item* score1;
    Screen_item* score2;
    Screen_item* score3;
    Screen_item* score4;
    Screen_item* mute;
    Screen_item* return_button;
} Scoreboard;


Screen_item* menu_button_text(App* app, char* text, TTF_Font* font, SDL_Color color);
Screen_item* menu_button_background (App* app, char resource[]);

void render_item(App* app, SDL_Rect* rect, SDL_Texture* texture, int item_type, int x, int y, int w, int h);

Menu* init_menu_tex(App* app, char top_button[], char middle_button[], char lower_button[], char return_button[]);
void render_menu(App* app, Menu* menu);
void free_menu(Menu* menu);

Scoreboard* create_scoreboard(App* app, Player players[]);
void render_scoreboard(App* app, Scoreboard *scoreboard);
void free_scoreboard(Scoreboard* scoreboard);
#endif