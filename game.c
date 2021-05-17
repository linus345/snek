#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include "app.h"
#include "game.h"

Game_State *init_game_state()
{
    Game_State *game_state = malloc(sizeof(Game_State));

    // client_id and current_time doesn't need to be initialized
    game_state->nr_of_players = 0;
    game_state->connected = false;
    game_state->last_time = 0;

    return game_state;
}

void load_texture(App *app, SDL_Texture **texture, char *path)
{
    printf("Loading texture: %s\n", path);
    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
    {
        fprintf(stderr, "error creating surface\n");
        exit(EXIT_FAILURE);
    }

    *texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    SDL_FreeSurface(surface);
    if (!*texture)
    {
        fprintf(stderr, "error creating texture\n");
        exit(EXIT_FAILURE);
    }
}
