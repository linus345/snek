#include <stdio.h>
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

    SDL_Surface *surface = IMG_Load("../snake-test/textures/snakehead.png");
    if(!surface) {
        fprintf(stderr, "error creating surface\n");
        return 3;
    }
    printf("SDL_image works\n");

    SDLNet_Quit();
    SDL_Quit();
    return 0;
}
