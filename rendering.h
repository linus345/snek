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

void render_item(App* app, SDL_Rect* rect, SDL_Texture* texture, int item_type, int x, int y, int w, int h);

#endif