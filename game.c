#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL_image.h>
#include "app.h"
#include "game.h"

void load_texture(App *app, SDL_Texture **texture, char *path)
{
    printf("Loading texture: %s\n", path);
    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
    {
        fprintf(stderr, "error creating surface\n");
        exit(EXIT_FAILURE);
    }

    *texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    SDL_FreeSurface(surface);
    if (!*texture)
    {
        fprintf(stderr, "error creating texture\n");
        exit(EXIT_FAILURE);
    }
}

//Renders an item on the screen.
void render_item(App* app, SDL_Rect* rect, SDL_Texture* texture, int x, int y, int w, int h, bool* fullscreen_bool)
{
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    if (fullscreen_bool) {
        optimizeFullscreen(app, rect);
    }
    SDL_RenderCopy(app->renderer, texture, NULL, rect);
}

//Converts the size and coordinates of an SDL_Rect to fit fullscreen.
void optimizeFullscreen (App* app, SDL_Rect *rect)
{
    rect->x = (app->display.w / 2) - (rect->w / 2);
    rect->y = rect->y * (app->display.h / WINDOW_HEIGHT);
    return;
}