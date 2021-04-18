#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

typedef struct {
    bool running;
    SDL_Window *window;
    SDL_Renderer *renderer;
} App;

int init_app();
void quit_app();

#endif
