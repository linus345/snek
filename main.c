#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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

    srand(time(NULL));

    App *app = init_app();

    int nr_of_players = 0;
    Player *player1 = new_player(1, 1, 1);
    nr_of_players++;

    Pos snake_texture[6];
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
    // mouth_open
    snake_texture[4].x = 64;
    snake_texture[4].y = 0;
    // mouth eating
    snake_texture[5].x = 64;
    snake_texture[5].y = 32;

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

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SDL_Texture *background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    SDL_Texture *newGame;
    load_texture(app, &newGame, "./resources/menuButton.png");
    SDL_Rect newGame_view = {300, 400, 360, 150};

    SDL_Texture *exit;
    load_texture(app, &exit, "./resources/menuButton.png");
    SDL_Rect exit_view = {300, 600, 360, 150};
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    TTF_Init();

    TTF_Font * font = TTF_OpenFont("arial.ttf", 25);
    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }

    SDL_Color color = { 255, 255, 255 };
    SDL_Surface * surface = TTF_RenderText_Solid(font,
    "Welcome to Gigi Labs", color);

    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    
    

    //menu_init(app, background, newGame, exit);

    while (app->running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                // exit main loop
                app->running = false;
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        SDL_RenderCopy(app->renderer, newGame, NULL, &newGame_view);
        SDL_RenderCopy(app->renderer, exit, NULL, &exit_view);

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
    }

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
                {
                case SDLK_UP:
                case SDLK_w:
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

        // Checks if any collisons has occured with the walls
        if (collison_with_wall(player1->snake))
        {
            app->running = false;
        }
        // Checks if any collisons has occured with a snake
        if (collison_with_snake(player1->snake))
        {
            app->running = false;
        }

        current_time = SDL_GetTicks();
        if (current_time > last_time + player1->snake->speed)
        {
            change_snake_velocity(player1->snake);

            // update snake velocity based on direction state
            // test singleplayer position update
            new_snake_pos(player1->snake);
            head_adjecent_with_fruit(&player1->snake->head, fruits, nr_of_fruits);

            last_time = current_time;
        }
        if (fruit_collision(player1->snake, fruits, nr_of_fruits))
        {
            free(fruits[nr_of_fruits - 1]);
            nr_of_fruits--;
            new_snake_body_part(&player1->snake->body[player1->snake->body_length - 1].pos,
                                player1->snake->body[player1->snake->body_length - 1].angle,
                                &player1->snake->body_length);
            player1->snake->speed -= 100;
        }

        // fruit rendering
        SDL_Rect fruit_src[MAX_PLAYERS];
        SDL_Rect fruit_dst[MAX_PLAYERS];

        Fruit *temp_fruit = NULL;
        if (nr_of_fruits < nr_of_players)
        {
            // spawn new fruit
            while (temp_fruit == NULL)
            {
                temp_fruit = new_fruit(fruits, nr_of_fruits, player1->snake);
            }
            fruits[nr_of_fruits++] = temp_fruit;
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

        // snake head rendering
        SDL_Rect head_src;
        SDL_Rect head_dst = {player1->snake->head.pos.x, player1->snake->head.pos.y, CELL_SIZE, CELL_SIZE};

        if (player1->snake->head.mouth_open)
        {
            // Snake open mouth next to fruit
            head_src.x = snake_texture[4].x;
            head_src.y = snake_texture[4].y;
        }
        else if (player1->snake->head.mouth_eating)
        {
            // Snake eating after open mouth
            head_src.x = snake_texture[5].x;
            head_src.y = snake_texture[5].y;
        }
        else
        {
            // Snake closed mouth, default
            head_src.x = snake_texture[0].x;
            head_src.y = snake_texture[0].y;
        }
        head_src.w = CELL_SIZE;
        head_src.h = CELL_SIZE;

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

        // render fruits
        for (int i = 0; i < nr_of_fruits; i++)
        {
            SDL_RenderCopyEx(app->renderer, fruit_sprite_tex, &fruit_src[i], &fruit_dst[i], 0, NULL, SDL_FLIP_NONE);
        }

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

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
    }

    quit_app(app);
    

    return 0;
}
