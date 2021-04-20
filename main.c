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

    while (app->running) {
        SDL_Event event;
        // check for event
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    // exit main loop
                    app->running = false;
                    break;
            }
        }
        if (snake->head.x <= 0 || snake->head.y <= 0 || snake->head.x >= WINDOW_WIDTH - CELL_SIZE || pos.y >= WINDOW_HEIGHT - CELL_SIZE.h)
        {
            game.state = QUIT_STATE; // Kills the program
        }
    }

    quit_app(app);
    return 0;
}
