#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "../includes/app.h"
#include "../includes/fruit.h"
#include "../includes/game.h"
#include "../includes/menu.h"
#include "../includes/player.h"
#include "../includes/snake.h"
#include "../includes/network.h"
#include "../includes/circular_buffer.h"

// used to indicate if receive thread should exit
bool thread_done;

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
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
        return 3;
    }
    printf("successfully initialized TTF\n");
    srand(time(NULL));

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Error: SDLNet_Init: %s\n", SDLNet_GetError());
        return 4;
    }

    App* app = init_app();

    Game_State *game_state = init_game_state();

    main_loop(app, game_state);

    quit_app(app);

    return 0;
}
