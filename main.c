#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include "app.h"
#include "game.h"

int main(int argc, char *argv[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error: SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    printf("successfully initialized SDL\n");
    if(SDLNet_Init() != 0) {
        fprintf(stderr, "Error: SDLNet_Init: %s\n", SDLNet_GetError());
        return 2;
    }
    printf("successfully initialized SDL_net\n");

    App *app = init_app();

    // test singleplayer
    Player *singleplayer = new_player(0, 0, 1);

    while (app->running) {
        SDL_Event event;
        // check for event
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    // exit main loop
                    app->running = false;
                    break;
                case SDL_KEYDOWN:
                    // key pressed?
                    switch (event.key.keysym.sym){                  /* event.key.keysym.scancode */
                        case SDLK_UP:
                        case SDLK_w:                                /* SDL_SCANCODE_W */
                            if (singleplayer->snake->dir != Down)
                                singleplayer->snake->dir = Up;
                            break;
                        case SDLK_DOWN:
                        case SDLK_s:
                            if (singleplayer->snake->dir != Up)
                                singleplayer->snake->dir = Down;
                            break;
                        case SDLK_RIGHT:
                        case SDLK_d:
                            if (singleplayer->snake->dir != Left)
                                singleplayer->snake->dir = Right;
                            break;
                        case SDLK_LEFT:
                        case SDLK_a:
                            if (singleplayer->snake->dir != Right)
                                singleplayer->snake->dir = Left;
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }
    }

    quit_app(app);
    return 0;
}
