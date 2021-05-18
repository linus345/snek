#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "app.h"
#include "fruit.h"
#include "game.h"
#include "menu.h"
#include "player.h"
#include "snake.h"

int main(int argc, char* argv[])
{
    // int game
    // start game
    // end game

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error: SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    printf("successfully initialized SDL\n");

    if (SDLNet_Init() != 0) {
        fprintf(stderr, "Error: SDLNet_Init: %s\n", SDLNet_GetError());
        return 2;
    }
    printf("successfully initialized SDL_net\n");
    if (TTF_Init() != 0) {
        SDL_Log("TTF_Init failed: %s", SDL_GetError());
        return 3;
    }
    printf("successfully initialized TTF\n");
    srand(time(NULL));

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Error: SDLNet_Init: %s\n", SDLNet_GetError());
        return 4;
    }
    
    App* app = init_app();

    SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    TTF_Init();
    if (TTF_Init == NULL || SDL_SetWindowFullscreen == NULL) {
        fprintf(stderr, "error: font not found\n%s\n", TTF_GetError());
        return 0;
    }
    menu(app);

    TTF_Quit();
    quit_app(app);

    return 0;
}
