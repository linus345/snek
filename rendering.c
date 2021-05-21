#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "rendering.h"
#include "app.h"
#include "fruit.h"
#include "game.h"
#include "menu.h"
#include "player.h"

//Renders an item on the screen.
void render_item(App* app, SDL_Rect* rect, SDL_Texture* texture, int item_type, int x, int y, int w, int h)
{
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    /*
    if (app->fullscreen) {
        switch (item_type) {
            case BACKGROUND:
                rect->w = app->display.w;
                rect->h = app->display.h;
                break;
            case STRETCH:
                rect->x = rect->x * (app->display.w / (WINDOW_WIDTH - 300));
                rect->y = rect->y * (app->display.h / WINDOW_HEIGHT);
                rect->w = rect->w * (app->display.w / (WINDOW_WIDTH - 300));
                rect->h = rect->h * (app->display.h / WINDOW_HEIGHT);
                break;
            case MENU_BUTTON:
                    rect->x = (app->display.w / 2) - (rect->w / 2);
                    rect->y = rect->y * (app->display.h / WINDOW_HEIGHT);
                break;
        }
    }
    */
    SDL_RenderCopy(app->renderer, texture, NULL, rect);
}
