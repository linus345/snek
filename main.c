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

    Player *player1 = new_player(1, 1, 1);

    Pos snake_texture[4];
    // head
    snake_texture[0].x = 0;
    snake_texture[0].y = 0;
    // body
    snake_texture[1].x = 32;
    snake_texture[1].y = 0;
    // tail
    snake_texture[2].x = 32;
    snake_texture[2].y = 32;
    // turning bodypart
    snake_texture[3].x = 0;
    snake_texture[3].y = 32;
    
    SDL_Texture *snake_sprite_tex;
    load_texture(app, &snake_sprite_tex, "./resources/snake-sprite.png");

    // background texture
    SDL_Texture *background_tex;
    load_texture(app, &background_tex, "./resources/background.png");

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
                            if (player1->snake->dir != Down)
                                player1->snake->dir = Up;
                            break;
                        case SDLK_DOWN:
                        case SDLK_s:
                            if (player1->snake->dir != Up)
                                player1->snake->dir = Down;
                            break;
                        case SDLK_RIGHT:
                        case SDLK_d:
                            if (player1->snake->dir != Left)
                                player1->snake->dir = Right;
                            break;
                        case SDLK_LEFT:
                        case SDLK_a:
                            if (player1->snake->dir != Right)
                                player1->snake->dir = Left;
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }

        // update snake velocity based on direction state
        change_snake_velocity(player1->snake);
        // test singleplayer position update
        new_snake_pos(player1->snake);

        // snake head rendering
        SDL_Rect head_src = {snake_texture[0].x, snake_texture[0].y, CELL_SIZE, CELL_SIZE};
        SDL_Rect head_dst = {player1->snake->head.pos.x, player1->snake->head.pos.y, CELL_SIZE, CELL_SIZE};

        // snake body rendering
        SDL_Rect body_src[MAX_SNAKE_LENGTH];
        SDL_Rect body_dst[MAX_SNAKE_LENGTH];
        for(int i = 0; i < player1->snake->body_length; i++) {
            body_src[i].x = snake_texture[1].x;
            body_src[i].y = snake_texture[1].y;
            body_src[i].w = CELL_SIZE;
            body_src[i].h = CELL_SIZE;

            body_dst[i].x = player1->snake->body[i].pos.x;
            body_dst[i].y = player1->snake->body[i].pos.y;
            body_dst[i].w = CELL_SIZE;
            body_dst[i].h = CELL_SIZE;
        }
        SDL_Rect tail_src = {snake_texture[2].x, snake_texture[2].y, CELL_SIZE, CELL_SIZE};
        SDL_Rect tail_dst = {player1->snake->tail.pos.x, player1->snake->tail.pos.y, CELL_SIZE, CELL_SIZE};

        // clear screen before next render
        SDL_RenderClear(app->renderer);

        SDL_Rect background_dst = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderCopy(app->renderer, background_tex, NULL, &background_dst);

        SDL_RenderCopyEx(app->renderer, snake_sprite_tex, &head_src, &head_dst, player1->snake->head.angle, NULL, SDL_FLIP_NONE);
        for(int i = 0; i < player1->snake->body_length; i++) {
            SDL_RenderCopyEx(app->renderer, snake_sprite_tex, &body_src[i], &body_dst[i], player1->snake->body[i].angle, NULL, SDL_FLIP_NONE);
        }
        SDL_RenderCopyEx(app->renderer, snake_sprite_tex, &tail_src, &tail_dst, player1->snake->tail.angle, NULL, SDL_FLIP_NONE);

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000/60);
    }

    quit_app(app);
    return 0;
}
