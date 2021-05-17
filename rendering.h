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
    BUTTON_X = (WINDOW_WIDTH / 2) - (BUTTON_W / 2),
    BUTTON_H = 150,
    TEXT_X = 337,
    TEXT_Y = 430,
    TEXT_W = 290,
    TEXT_H = 90,
};

void render_item(App* app, SDL_Rect* rect, SDL_Texture* texture, int item_type, int x, int y, int w, int h);

#endif