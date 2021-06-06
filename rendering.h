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

enum Scoreboard_dimensions {
    SD_BUTTON_X = 0,
    SD_BUTTON_Y = 50,
    SD_BUTTON_W = 250,
    SD_BUTTON_H = 100,

    NAME_X = 29,
    NAME_Y = 65,
    NAME_W = 100,
    NAME_H = 70,

    SCORE_X = 150,
    SCORE_Y = 65,
    SCORE_W = 70,
    SCORE_H = 70,

    Y_OFFSET = 100
};

enum Finsh_scoreboard_dimensions {
    FSD_BUTTON_X = WINDOW_WIDTH / 2 - 180,
    FSD_BUTTON_Y = 200,
    FSD_BUTTON_W = 400,
    FSD_BUTTON_H = 150,

    F_NAME_X = WINDOW_WIDTH / 2 - 130,
    F_NAME_Y = 234,
    F_NAME_W = 130,
    F_NAME_H = 90,

    F_SCORE_X = WINDOW_WIDTH / 2 + 40,
    F_SCORE_Y = 234,
    F_SCORE_W = 90,
    F_SCORE_H = 90,

    FY_OFFSET = 150
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

// Genral functions
Screen_item* menu_button_text(App* app, char* text, TTF_Font* font, SDL_Color color);
Screen_item* menu_button_background(App* app, char resource[]);
void render_item(App* app, SDL_Rect* rect, SDL_Texture* texture, int item_type, int x, int y, int w, int h);
// Menu functions
Menu* init_menu_tex(App* app, TTF_Font* font, char top_button[], char middle_button[], char lower_button[], char return_button[]);
void render_menu(App* app, Menu* menu);
void free_menu(Menu* menu);
// Scoreboard functions
void render_scoreboard(App* app, Scoreboard* scoreboard);
void render_end_of_round(App* app, Scoreboard* scoreboard);
void render_fruits(App* app, Fruit* fruits[], SDL_Texture* fruit_sprite_tex, Pos fruit_texture[]);
void render_snakes(App* app, Player* players[], int nr_of_players, SDL_Texture* snake_sprite_textures[], Pos snake_texture[]);
#endif
