#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include <SDL2/SDL.h>
//960
#define WINDOW_WIDTH 1210
#define WINDOW_HEIGHT 960

#define GAME_WIDTH 960
#define GAME_HEIGHT 960

#define GAME_START 250

typedef struct {
    bool running;
    SDL_Window *window;
    SDL_Renderer *renderer;
} App;

App *init_app();
void quit_app(App *app);

#endif
