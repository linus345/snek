#include "app.h"
#include "menu.h"
#include "sound.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

App* init_app()
{

    // Allocate memory for the core program features
    App* app = malloc(sizeof(App));

    // initialize window
    app->window = SDL_CreateWindow("Snek", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    // handle for errors
    if (app->window == NULL) {
        // print error
        fprintf(stderr, "Error while initializing window: %s\n", SDL_GetError());
        // exit with failure
        exit(EXIT_FAILURE);
    }

    // initialize renderer
    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);
    // handle errors
    if (app->renderer == NULL) {
        // print error
        fprintf(stderr, "Error while initializing renderer: %s\n", SDL_GetError());
        // exit with failure
        exit(EXIT_FAILURE);
    }

    // Collects information about current monitor.
    if (SDL_GetCurrentDisplayMode(0, &app->display) != 0) {
        // print error
        fprintf(stderr, "Error while calling SDL_DisplayMode: %s\n", SDL_GetError());
        // exit with failure
        exit(EXIT_FAILURE);
    }
    char tmp[1] = "";
    strcpy(app->ip, tmp);
    strcpy(app->port, tmp);

    // indicate that the app is running, used for main loop
    app->running = true;

    // Indicates wether the application is in fullscreen or not
    //app->fullscreen = false;

    // return pointer
    return app;
}

void quit_app(App* app)
{

    printf("Exiting...\n");
    // destory window and renderer to free memory
    SDL_DestroyWindow(app->window);
    SDL_DestroyRenderer(app->renderer);

    // cleanup subsystems before exiting'
    Mix_CloseAudio();
    SDLNet_Quit();
    TTF_Quit();
    SDL_Quit();

    // successfully exit
    exit(EXIT_SUCCESS);
}
