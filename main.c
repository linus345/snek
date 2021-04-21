#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include "app.h"
#include "game.h"
#include "player.h"
#include "snake.h"
#include "fruit.h"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error: SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    printf("successfully initialized SDL\n");
    if (SDLNet_Init() != 0)
    {
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

    // intilaze fruits array
    Fruit *fruits[MAX_PLAYERS];
    int nr_of_fruits = 0;
    //bool start_up_fruit = true;

    Pos fruit_texture[4];
    //cherry
    fruit_texture[0].x = 0;
    fruit_texture[0].y = 0;
    //apple
    fruit_texture[1].x = 32;
    fruit_texture[1].y = 0;
    //pear
    fruit_texture[2].x = 0;
    fruit_texture[2].y = 32;
    //mango
    fruit_texture[3].x = 32;
    fruit_texture[3].y = 32;

    SDL_Texture *fruit_sprite_tex;
    load_texture(app, &fruit_sprite_tex, "./resources/fruit-sprite.png");

    // background texture
    SDL_Texture *background_tex;
    load_texture(app, &background_tex, "./resources/background.png");

    // timer
    unsigned last_time = 0, current_time;

    while (app->running)
    {
        SDL_Event event;
        // check for event
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                // exit main loop
                app->running = false;
                break;
            case SDL_KEYDOWN:
                // key pressed?
                switch (event.key.keysym.sym)
                { /* event.key.keysym.scancode */
                case SDLK_UP:
                case SDLK_w: /* SDL_SCANCODE_W */
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

        current_time = SDL_GetTicks();
        if (current_time > last_time + player1->snake->speed)
        {
            change_snake_velocity(player1->snake);

            // update snake velocity based on direction state
            // test singleplayer position update
            new_snake_pos(player1->snake);

            last_time = current_time;
        }

        // snake head rendering
        SDL_Rect head_src = {snake_texture[0].x, snake_texture[0].y, CELL_SIZE, CELL_SIZE};
        SDL_Rect head_dst = {player1->snake->head.pos.x, player1->snake->head.pos.y, CELL_SIZE, CELL_SIZE};

        // snake body rendering
        SDL_Rect body_src[MAX_SNAKE_LENGTH];
        SDL_Rect body_dst[MAX_SNAKE_LENGTH];
        for (int i = 0; i < player1->snake->body_length; i++)
        {
            if (player1->snake->body[i].is_turn)
            {
                body_src[i].x = snake_texture[3].x;
                body_src[i].y = snake_texture[3].y;
            }
            else
            {
                body_src[i].x = snake_texture[1].x;
                body_src[i].y = snake_texture[1].y;
            }
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

        // render head
        SDL_RenderCopyEx(app->renderer, snake_sprite_tex, &head_src, &head_dst, player1->snake->head.angle, NULL, SDL_FLIP_NONE);
        // render body
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        int rotation;
        for (int i = 0; i < player1->snake->body_length; i++)
        {
            rotation = player1->snake->body[i].angle;
            flip = SDL_FLIP_NONE;
            if (player1->snake->body[i].is_turn)
            {
                rotation = player1->snake->body[i].turn_rotation;
                if (player1->snake->body[i].should_flip_vertical)
                {
                    flip = SDL_FLIP_VERTICAL;
                }
                else if (player1->snake->body[i].should_flip_horizontal)
                {
                    flip = SDL_FLIP_HORIZONTAL;
                }
            }
            SDL_RenderCopyEx(app->renderer, snake_sprite_tex, &body_src[i], &body_dst[i], rotation, NULL, flip);
        }
        // render tail
        SDL_RenderCopyEx(app->renderer, snake_sprite_tex, &tail_src, &tail_dst, player1->snake->tail.angle, NULL, SDL_FLIP_NONE);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        SDL_Rect fruit_src[nr_of_fruits];
        SDL_Rect fruit_dst[nr_of_fruits];


        //nr_of_fruits = 0;

        //for (int i = 0; i < nr_of_fruits+1; i++){
         //   fruits[nr_of_fruits] = rand_fruit(4, &nr_of_fruits, player1->snake, fruits);
        //}
if (nr_of_fruits < 4){
    fruits[nr_of_fruits] = rand_fruit(4, &nr_of_fruits, player1->snake, fruits);
}
        for (int i = 0; i < nr_of_fruits; i++)
        {
            fruit_src[i].x = fruit_texture[fruits[i]->type].x;
            fruit_src[i].y = fruit_texture[fruits[i]->type].y;
            fruit_src[i].w = CELL_SIZE;
            fruit_src[i].h = CELL_SIZE;

            fruit_dst[i].x = fruits[i]->pos.x;
            fruit_dst[i].y = fruits[i]->pos.y;
            fruit_dst[i].w = CELL_SIZE;
            fruit_dst[i].h = CELL_SIZE;
        }
        for (int i = 0; i < nr_of_fruits; i++){
            SDL_RenderCopyEx(app->renderer, fruit_sprite_tex, &fruit_src[i], &fruit_dst[i], 0, NULL, SDL_FLIP_NONE);
        }

        /*
        if (start_up_fruit)
        {
            SDL_Rect fruit_src = {fruit_texture[0].x, fruit_texture[0].y, CELL_SIZE, CELL_SIZE};
            SDL_Rect fruit_dst = {fruits[0]->pos.x, fruits[0]->pos.y, CELL_SIZE, CELL_SIZE};
            SDL_RenderCopyEx(app->renderer, fruit_sprite_tex, &fruit_src, &fruit_dst, 0, NULL, SDL_FLIP_NONE);
            start_up_fruit = false;
        }
    

        if (fruit_collision(player1->snake, fruits))
        {
            
        }
        */
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
    }

    printf("%d", nr_of_fruits);
    quit_app(app);
    
    return 0;
}
