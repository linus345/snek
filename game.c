#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "app.h"
#include "fruit.h"
#include "game.h"
#include "menu.h"
#include "player.h"
#include "rendering.h"

void load_texture(App* app, SDL_Texture** texture, char* path)
{
    printf("Loading texture: %s\n", path);
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        fprintf(stderr, "error creating surface\n");
        exit(EXIT_FAILURE);
    }

    *texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    SDL_FreeSurface(surface);
    if (!*texture) {
        fprintf(stderr, "error creating texture\n");
        exit(EXIT_FAILURE);
    }
}

int game(App* app, Sound_effects* sound)
{
    bool show_scoreboard = false;
    /*
    if (app->fullscreen) {
        SDL_SetWindowFullscreen(app->window, 0);
        app->fullscreen = false;
    } else {
        SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        app->fullscreen = true;
    }
    */

    int nr_of_players = 0;
    Player* player1 = new_player(1, 1, 1);
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

    SDL_Texture* snake_sprite_tex;
    load_texture(app, &snake_sprite_tex, "./resources/snake-sprite.png");

    // intilaze fruits array
    Fruit* fruits[MAX_PLAYERS];
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

    SDL_Texture* fruit_sprite_tex;
    load_texture(app, &fruit_sprite_tex, "./resources/fruit-sprite.png");

    // Scoreboard texture
    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    SDL_Color white_txt = { 255, 255, 255, 255 };

    Screen_item* background = menu_button_background(app, "./resources/background.png");
    Screen_item* scorescreen_background = menu_button_background(app, "./resources/Forest_green.jpg");
    Screen_item* goal_text = menu_button_text(app, "Goal", font, white_txt);
    Screen_item* goal_nr = menu_button_text(app, "250", font, white_txt);

    Screen_item* scoreboard1 = menu_button_background(app, "./resources/menuButton.png");
    Screen_item* scoreboard2 = menu_button_background(app, "./resources/menuButton.png");
    Screen_item* scoreboard3 = menu_button_background(app, "./resources/menuButton.png");
    Screen_item* scoreboard4 = menu_button_background(app, "./resources/menuButton.png");

    Screen_item* player1_name = menu_button_text(app, app->player_name, font, white_txt);
    Screen_item* player2_name = menu_button_text(app, "Stoffe", font, white_txt);
    Screen_item* player3_name = menu_button_text(app, "Victor", font, white_txt);
    Screen_item* player4_name = menu_button_text(app, "Alma", font, white_txt);

    Screen_item* player1_score = menu_button_text(app, " ", font, white_txt);
    Screen_item* player2_score = menu_button_text(app, " ", font, white_txt);
    Screen_item* player3_score = menu_button_text(app, " ", font, white_txt);
    Screen_item* player4_score = menu_button_text(app, " ", font, white_txt);

    int score = 0;
    char buffer[50];

    Screen_item* return_button = menu_button_text(app, "Leave game", font, white_txt);

    // timer
    unsigned last_time = 0, current_time;
    int Mx, My;
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
                switch (event.key.keysym.sym) {
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
                case SDLK_F11:
                    if (app->fullscreen) {
                        SDL_SetWindowFullscreen(app->window, 0);
                        app->fullscreen = false;
                    } else {
                        SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        app->fullscreen = true;
                    }
                    break;
                case SDLK_ESCAPE:
                    break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (hover_state(return_button, Mx, My)) {
                    // Makes space on the heap
                    free(goal_text);
                    free(goal_nr);
                    free(scoreboard1);
                    free(scoreboard2);
                    free(scoreboard3);
                    free(scoreboard4);
                    free(player1_name);
                    free(player2_name);
                    free(player3_name);
                    free(player3_name);
                    free(player4_name);
                    free(player1_score);
                    free(player2_score);
                    free(player3_score);
                    free(player4_score);
                    free(return_button);
                    return MAIN_MENU;
                }
                break;
            }
        }

        // Checks if any collisons has occured with the walls
        if (collison_with_wall(player1->snake)) {
            play_sound(sound->wall_collison); // plays wall collison sound effect
            app->running = false;
            show_scoreboard = true; // Remove once multiplayer has been implimented
        }
        // Checks if any collisons has occured with a snake
        if (collison_with_snake(player1->snake)) {
            app->running = false;
            show_scoreboard = true; // Remove once multiplayer has been implimented
        }

        current_time = SDL_GetTicks();
        if (current_time > last_time + player1->snake->speed) {
            change_snake_velocity(player1->snake);

            // update snake velocity based on direction state
            // test singleplayer position update
            new_snake_pos(player1->snake);
            for (int i = 0; nr_of_fruits > i; i++) {
                if (fruits[i]->pos.x == player1->snake->head.pos.x && (fruits[i]->pos.y - CELL_SIZE == player1->snake->head.pos.y || fruits[i]->pos.y + CELL_SIZE == player1->snake->head.pos.y)) {
                    player1->snake->head.mouth_open = true;
                } else if (fruits[i]->pos.y == player1->snake->head.pos.y && (fruits[i]->pos.x - CELL_SIZE == player1->snake->head.pos.x || fruits[i]->pos.x + CELL_SIZE == player1->snake->head.pos.x)) {
                    player1->snake->head.mouth_open = true;
                }
            }

            last_time = current_time;
        }
        if (fruit_collision(player1->snake, fruits, nr_of_fruits)) {
            free(fruits[nr_of_fruits - 1]);
            nr_of_fruits--;
            new_snake_body_part(&player1->snake->body[player1->snake->body_length - 1].pos,
                player1->snake->body[player1->snake->body_length - 1].angle,
                &player1->snake->body_length);

            if (player1->snake->speed > 100) {
                player1->snake->speed = (SPEED - (pow(player1->snake->body_length, 2) * 2) / 3);
            } else if (player1->snake->speed < 100) {
                player1->snake->speed -= 2;
            } else if (player1->snake->speed >= 0) {
                player1->snake->speed = 0;
            }

            player1->points += fruits[0]->type;
            score = player1->points;

            sprintf(buffer, "%d", player1->points);

            player1_score = menu_button_text(app, buffer, font, white_txt);

            play_sound(sound->eat); // plays eat sound effect

            printf("speed: %d\n", player1->snake->speed);
        }

        // fruit rendering
        SDL_Rect fruit_src[MAX_PLAYERS];
        SDL_Rect fruit_dst[MAX_PLAYERS];

        Fruit* temp_fruit = NULL;
        if (nr_of_fruits < nr_of_players) {
            // spawn new fruit
            while (temp_fruit == NULL) {
                temp_fruit = new_fruit(fruits, nr_of_fruits, player1->snake);
            }
            fruits[nr_of_fruits++] = temp_fruit;
        }
        for (int i = 0; i < nr_of_fruits; i++) {
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
        SDL_Rect head_dst = { player1->snake->head.pos.x, player1->snake->head.pos.y, CELL_SIZE, CELL_SIZE };

        if (player1->snake->head.mouth_open) {
            // Snake open mouth next to fruit
            head_src.x = snake_texture[4].x;
            head_src.y = snake_texture[4].y;
        } else if (player1->snake->head.mouth_eating) {
            // Snake eating after open mouth
            head_src.x = snake_texture[5].x;
            head_src.y = snake_texture[5].y;
        } else {
            // Snake closed mouth, default
            head_src.x = snake_texture[0].x;
            head_src.y = snake_texture[0].y;
        }
        head_src.w = CELL_SIZE;
        head_src.h = CELL_SIZE;

        // snake body rendering
        SDL_Rect body_src[MAX_SNAKE_LENGTH];
        SDL_Rect body_dst[MAX_SNAKE_LENGTH];
        for (int i = 0; i < player1->snake->body_length; i++) {
            if (player1->snake->body[i].is_turn) {
                body_src[i].x = snake_texture[3].x;
                body_src[i].y = snake_texture[3].y;
            } else {
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

        SDL_Rect tail_src = { snake_texture[2].x, snake_texture[2].y, CELL_SIZE, CELL_SIZE };
        SDL_Rect tail_dst = { player1->snake->tail.pos.x, player1->snake->tail.pos.y, CELL_SIZE, CELL_SIZE};

        // clear screen before next render
        SDL_RenderClear(app->renderer);

        render_item(app, &scorescreen_background->rect, scorescreen_background->texture, STRETCH, 0, 0, 250, WINDOW_HEIGHT);
        render_item(app, &background->rect, background->texture, BACKGROUND, GAME_START_POS, 0, GAME_WIDTH, WINDOW_HEIGHT);

        render_item(app, &goal_text->rect, goal_text->texture, UNSPECIFIED, 0, 0, 50, 50);
        render_item(app, &goal_nr->rect, goal_nr->texture, UNSPECIFIED, 194, 0, 50, 50);

        render_item(app, &scoreboard1->rect, scoreboard1->texture, UNSPECIFIED, SD_BUTTON_X, SD_BUTTON_Y, SD_BUTTON_W, SD_BUTTON_H);
        render_item(app, &scoreboard2->rect, scoreboard2->texture, UNSPECIFIED, SD_BUTTON_X, SD_BUTTON_Y + Y_OFFSET, SD_BUTTON_W, SD_BUTTON_H);
        render_item(app, &scoreboard3->rect, scoreboard3->texture, UNSPECIFIED, SD_BUTTON_X, SD_BUTTON_Y + (2 * Y_OFFSET), SD_BUTTON_W, SD_BUTTON_H);
        render_item(app, &scoreboard4->rect, scoreboard4->texture, UNSPECIFIED, SD_BUTTON_X, SD_BUTTON_Y + (3 * Y_OFFSET), SD_BUTTON_W, SD_BUTTON_H);

        render_item(app, &player1_name->rect, player1_name->texture, UNSPECIFIED, NAME_X, NAME_Y, NAME_W, NAME_H);
        render_item(app, &player1_score->rect, player1_score->texture, UNSPECIFIED, SCORE_X, SCORE_Y, SCORE_W, SCORE_H);

        render_item(app, &player2_name->rect, player2_name->texture, UNSPECIFIED, NAME_X, NAME_Y + Y_OFFSET, NAME_W, NAME_H);
        render_item(app, &player3_name->rect, player3_name->texture, UNSPECIFIED, NAME_X, NAME_Y + (2 * Y_OFFSET), NAME_W, NAME_H);
        render_item(app, &player4_name->rect, player4_name->texture, UNSPECIFIED, NAME_X, NAME_Y + (3 * Y_OFFSET), NAME_W, NAME_H);

        render_item(app, &return_button->rect, return_button->texture, UNSPECIFIED, 15, GAME_HEIGHT-50, 220, 50);

        // render fruits
        for (int i = 0; i < nr_of_fruits; i++) {
            SDL_RenderCopyEx(app->renderer, fruit_sprite_tex, &fruit_src[i], &fruit_dst[i], 0, NULL, SDL_FLIP_NONE);
        }

        // render head
        SDL_RenderCopyEx(app->renderer, snake_sprite_tex, &head_src, &head_dst, player1->snake->head.angle, NULL, SDL_FLIP_NONE);
        // render body
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        int rotation;
        for (int i = 0; i < player1->snake->body_length; i++) {
            rotation = player1->snake->body[i].angle;
            flip = SDL_FLIP_NONE;
            if (player1->snake->body[i].is_turn) {
                rotation = player1->snake->body[i].turn_rotation;
                if (player1->snake->body[i].should_flip_vertical) {
                    flip = SDL_FLIP_VERTICAL;
                } else if (player1->snake->body[i].should_flip_horizontal) {
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
        SDL_GetMouseState(&Mx, &My);
    }

    if (show_scoreboard) {
        //play_sound(sound->scoreboard);
        app->running = true;
        return scoreboard(app, sound, score);
    } else {
        return MAIN_MENU;
    }
}

int scoreboard(App* app, Sound_effects* sound, int score)
{
    //bool playsound = true;

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);
    SDL_Color white_txt = { 255, 255, 255, 255 };

    Screen_item* scorescreen_background = menu_button_background(app, "./resources/Forest_green.jpg");

    Screen_item* goal_text = menu_button_text(app, "Goal:", font, white_txt);
    Screen_item* goal_nr = menu_button_text(app, "250", font, white_txt);

    Screen_item* scoreboard1 = menu_button_background(app, "./resources/menuButton.png");
    Screen_item* scoreboard2 = menu_button_background(app, "./resources/menuButton.png");
    Screen_item* scoreboard3 = menu_button_background(app, "./resources/menuButton.png");
    Screen_item* scoreboard4 = menu_button_background(app, "./resources/menuButton.png");

    Screen_item* player1_name = menu_button_text(app, app->player_name, font, white_txt);
    Screen_item* player2_name = menu_button_text(app, "Stoffe", font, white_txt);
    Screen_item* player3_name = menu_button_text(app, "Victor", font, white_txt);
    Screen_item* player4_name = menu_button_text(app, "Alma", font, white_txt);

    char buffer[10];
    sprintf(buffer, "%d", score);

    Screen_item* player1_score = menu_button_text(app, buffer, font, white_txt);
    Screen_item* player2_score = menu_button_text(app, " ", font, white_txt);
    Screen_item* player3_score = menu_button_text(app, " ", font, white_txt);
    Screen_item* player4_score = menu_button_text(app, " ", font, white_txt);

    Screen_item* continue_button = menu_button_text(app, "Press to continue", font, white_txt);

    // timer
    unsigned last_time = 0, current_time;
    int Mx, My;
    int speed = 100000;
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
                switch (event.key.keysym.sym) {
                case SDLK_F11:
                    if (app->fullscreen) {
                        SDL_SetWindowFullscreen(app->window, 0);
                        app->fullscreen = false;
                    } else {
                        SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        app->fullscreen = true;
                    }
                    // Makes space on the heap
                    break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (hover_state(continue_button, Mx, My)) {
                    play_sound(sound->press);
                    // Makes space on the heap
                    //free(goal_text);
                    //free(goal_nr);
                    free(scoreboard1);
                    free(scoreboard2);
                    free(scoreboard3);
                    free(scoreboard4);
                    free(player1_name);
                    free(player2_name);
                    free(player3_name);
                    free(player3_name);
                    free(player4_name);
                    free(player1_score);
                    free(player2_score);
                    free(player3_score);
                    free(player4_score);
                    free(continue_button);
                    return MAIN_MENU;
                }
                break;
            }
        }

        // clear screen before next render
        SDL_RenderClear(app->renderer);

        render_item(app, &scorescreen_background->rect, scorescreen_background->texture, STRETCH, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        render_item(app, &goal_text->rect, goal_text->texture, UNSPECIFIED, WINDOW_WIDTH/2-129, 100, 300, 120);

        render_item(app, &scoreboard1->rect, scoreboard1->texture, UNSPECIFIED, FSD_BUTTON_X, FSD_BUTTON_Y, FSD_BUTTON_W, FSD_BUTTON_H);
        render_item(app, &scoreboard2->rect, scoreboard2->texture, UNSPECIFIED, FSD_BUTTON_X, FSD_BUTTON_Y + FY_OFFSET, FSD_BUTTON_W, FSD_BUTTON_H);
        render_item(app, &scoreboard3->rect, scoreboard3->texture, UNSPECIFIED, FSD_BUTTON_X, FSD_BUTTON_Y + (2 * FY_OFFSET), FSD_BUTTON_W, FSD_BUTTON_H);
        render_item(app, &scoreboard4->rect, scoreboard4->texture, UNSPECIFIED, FSD_BUTTON_X, FSD_BUTTON_Y + (3 * FY_OFFSET), FSD_BUTTON_W, FSD_BUTTON_H);

        render_item(app, &player1_name->rect, player1_name->texture, UNSPECIFIED, F_NAME_X, F_NAME_Y, F_NAME_W, F_NAME_H);
        render_item(app, &player1_score->rect, player1_score->texture, UNSPECIFIED, F_SCORE_X, F_SCORE_Y, F_SCORE_W, F_SCORE_H);

        render_item(app, &player2_name->rect, player2_name->texture, UNSPECIFIED, F_NAME_X, F_NAME_Y + FY_OFFSET, F_NAME_W, F_NAME_H);
        render_item(app, &player3_name->rect, player3_name->texture, UNSPECIFIED, F_NAME_X, F_NAME_Y + (2 * FY_OFFSET), F_NAME_W, F_NAME_H);
        render_item(app, &player4_name->rect, player4_name->texture, UNSPECIFIED, F_NAME_X, F_NAME_Y + (3 * FY_OFFSET), F_NAME_W, F_NAME_H);

        render_item(app, &continue_button->rect, continue_button->texture, UNSPECIFIED, WINDOW_WIDTH/2-129, WINDOW_HEIGHT-145, 300, 120);

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }
}