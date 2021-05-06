#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "app.h"

App *init_app()
{
    // allocate enough memory on the heap
    App *app = malloc(sizeof(App));

    // initialize window
    app->window = SDL_CreateWindow("Snek", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    // handle for errors
    if(app->window == NULL) {
        // print error
        fprintf(stderr, "Error while initializing window: %s\n", SDL_GetError());
        // exit with failure
        exit(EXIT_FAILURE);
    }

    // initialize renderer
    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);
    // handle errors
    if(app->renderer == NULL) {
        // print error
        fprintf(stderr, "Error while initializing renderer: %s\n", SDL_GetError());
        // exit with failure
        exit(EXIT_FAILURE);
    }

    // indicate that the app is running, used for main loop
    app->running = true;

    // return pointer
    return app;
}

void optimizeFullscreen (SDL_Rect *rect) {

    SDL_DisplayMode display;
    SDL_GetCurrentDisplayMode(0, &display);
    rect->x = rect->x * (display.w / WINDOW_WIDTH);
    rect->y = rect->y * (display.h / WINDOW_HEIGHT);
}

void quit_app(App *app)
{
    printf("Exiting...\n");
    // destory window and renderer to free memory
    SDL_DestroyWindow(app->window);
    SDL_DestroyRenderer(app->renderer);
    
    // cleanup subsystems before exiting
    SDLNet_Quit();
    SDL_Quit();

    // successfully exit
    exit(EXIT_SUCCESS);
}
