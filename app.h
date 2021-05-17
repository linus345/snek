#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#define WINDOW_WIDTH 1250
#define WINDOW_HEIGHT 960

#define GAME_WIDTH 960
#define GAME_HEIGHT 960
//Core window renderer
typedef struct App {
    bool running, fullscreen;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_DisplayMode display;
    char ip[16], port[5], player_name[16];
} App;

App *init_app();
void quit_app(App *app);

#endif
