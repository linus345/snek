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

SDL_Color
    black
    = { 0, 0, 0, 255 },
    gray = { 127, 127, 127, 255 },
    white = { 255, 255, 255, 255 },
    green = { 45, 93, 9, 255 },
    dark_green = { 9, 34, 3, 255 };

//Renders an item on the screen.
void render_item(App* app, SDL_Rect* rect, SDL_Texture* texture, int item_type, int x, int y, int w, int h)
{
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    /*
    if (app->fullscreen) {
        switch (item_type) {
            case BACKGROUND:
                rect->w = app->display.w;
                rect->h = app->display.h;
                break;
            case STRETCH:
                rect->x = rect->x * (app->display.w / (WINDOW_WIDTH - 300));
                rect->y = rect->y * (app->display.h / WINDOW_HEIGHT);
                rect->w = rect->w * (app->display.w / (WINDOW_WIDTH - 300));
                rect->h = rect->h * (app->display.h / WINDOW_HEIGHT);
                break;
            case MENU_BUTTON:
                    rect->x = (app->display.w / 2) - (rect->w / 2);
                    rect->y = rect->y * (app->display.h / WINDOW_HEIGHT);
                break;
        }
    }
    */
    SDL_RenderCopy(app->renderer, texture, NULL, rect);
}

// Universal text renderer for buttons.
Screen_item* menu_button_text(App* app, char* text, TTF_Font* font, SDL_Color color)
{
    Screen_item* item = malloc(sizeof(Screen_item));
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (surface == NULL) {
        SDL_Log("TTF_RenderText_Blended failed: %s", SDL_GetError());
        app->running = false;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    if (texture == NULL) {
        SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        app->running = false;
    }

    // free surface
    SDL_FreeSurface(surface);

    item->texture = texture;
    return item;
}
// Button memory allocator and texture loader.
Screen_item* menu_button_background(App* app, char resource[])
{
    Screen_item* button = malloc(sizeof(Screen_item));
    //Screen_item* button;
    load_texture(app, &button->texture, resource);
    return button;
}

Menu* init_menu_tex(App* app, TTF_Font* font, char top_button[], char middle_button[], char lower_button[], char return_button[])
{
    Menu* menu = malloc(sizeof(Menu));

    // Creates background and menu button texture
    menu->background = menu_button_background(app, "./resources/Textures/background.png");
    menu->button = menu_button_background(app, "./resources/Textures/menuButton.png");

    // Creates text textures
    menu->text1 = menu_button_text(app, top_button, font, green);
    menu->text2 = menu_button_text(app, middle_button, font, green);
    menu->text3 = menu_button_text(app, lower_button, font, green);
    menu->return_button = menu_button_text(app, return_button, font, white);

    return menu;
}

void render_menu(App* app, Menu* menu)
{
    // Renders background and button textures
    render_item(app, &menu->background->rect, menu->background->texture, BACKGROUND, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    render_item(app, &menu->button->rect, menu->button->texture, MENU_BUTTON, BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H);
    render_item(app, &menu->button->rect, menu->button->texture, MENU_BUTTON, BUTTON_X, BUTTON_Y + (1 * 150), BUTTON_W, BUTTON_H);
    render_item(app, &menu->button->rect, menu->button->texture, MENU_BUTTON, BUTTON_X, BUTTON_Y + (2 * 150), BUTTON_W, BUTTON_H);
    // Renders text textures
    render_item(app, &menu->text1->rect, menu->text1->texture, MENU_BUTTON, TEXT_X, TEXT_Y, TEXT_W, TEXT_H);
    render_item(app, &menu->text2->rect, menu->text2->texture, MENU_BUTTON, TEXT_X, TEXT_Y + (1 * 150), TEXT_W, TEXT_H);
    render_item(app, &menu->text3->rect, menu->text3->texture, MENU_BUTTON, TEXT_X, TEXT_Y + (2 * 150), TEXT_W, TEXT_H);
    render_item(app, &menu->return_button->rect, menu->return_button->texture, MENU_BUTTON, TEXT_X, TEXT_Y + (3 * 150), TEXT_W, TEXT_H);
}

void free_menu(Menu* menu)
{
    free(menu->background->texture);
    free(menu->background);
    free(menu->button->texture);
    free(menu->button);
    free(menu->text1->texture);
    free(menu->text1);
    free(menu->text2->texture);
    free(menu->text2);
    free(menu->text3->texture);
    free(menu->text3);
    free(menu->return_button->texture);
    free(menu->return_button);

    free(menu);
}

void render_scoreboard(App* app, Scoreboard* scoreboard)
{
    // Renders background and scoreboard textures
    render_item(app, &scoreboard->background->rect, scoreboard->background->texture, STRETCH, 0, 0, 250, WINDOW_HEIGHT);
    render_item(app, &scoreboard->scoreboard->rect, scoreboard->scoreboard->texture, UNSPECIFIED, SD_BUTTON_X, SD_BUTTON_Y, SD_BUTTON_W, SD_BUTTON_H);
    render_item(app, &scoreboard->scoreboard->rect, scoreboard->scoreboard->texture, UNSPECIFIED, SD_BUTTON_X, SD_BUTTON_Y + Y_OFFSET, SD_BUTTON_W, SD_BUTTON_H);
    render_item(app, &scoreboard->scoreboard->rect, scoreboard->scoreboard->texture, UNSPECIFIED, SD_BUTTON_X, SD_BUTTON_Y + (2 * Y_OFFSET), SD_BUTTON_W, SD_BUTTON_H);
    render_item(app, &scoreboard->scoreboard->rect, scoreboard->scoreboard->texture, UNSPECIFIED, SD_BUTTON_X, SD_BUTTON_Y + (3 * Y_OFFSET), SD_BUTTON_W, SD_BUTTON_H);
    // Renders name textures
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(scoreboard->name[i] == NULL || scoreboard->score[i] == NULL)
            continue;

        render_item(app, &scoreboard->name[i]->rect, scoreboard->name[i]->texture, MENU_BUTTON, NAME_X, NAME_Y + (i * Y_OFFSET), NAME_W, NAME_H);
        render_item(app, &scoreboard->score[i]->rect, scoreboard->score[i]->texture, UNSPECIFIED, SCORE_X, SCORE_Y + (i * Y_OFFSET), SCORE_W, SCORE_H);
    }
    // Renders mute and exit button
    render_item(app, &scoreboard->return_button->rect, scoreboard->return_button->texture, UNSPECIFIED, 55, GAME_HEIGHT - 50, 50, 50);
    render_item(app, &scoreboard->mute->rect, scoreboard->mute->texture, UNSPECIFIED, 160, GAME_HEIGHT - 50, 50, 50);
}

void render_end_of_round(App* app, Scoreboard* scoreboard)
{
    // Renders background and scoreboard textures
    render_item(app, &scoreboard->background->rect, scoreboard->background->texture, STRETCH, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    render_item(app, &scoreboard->scoreboard->rect, scoreboard->scoreboard->texture, UNSPECIFIED, FSD_BUTTON_X, FSD_BUTTON_Y, FSD_BUTTON_W, FSD_BUTTON_H);
    render_item(app, &scoreboard->scoreboard->rect, scoreboard->scoreboard->texture, UNSPECIFIED, FSD_BUTTON_X, FSD_BUTTON_Y + FY_OFFSET, FSD_BUTTON_W, FSD_BUTTON_H);
    render_item(app, &scoreboard->scoreboard->rect, scoreboard->scoreboard->texture, UNSPECIFIED, FSD_BUTTON_X, FSD_BUTTON_Y + (2 * FY_OFFSET), FSD_BUTTON_W, FSD_BUTTON_H);
    render_item(app, &scoreboard->scoreboard->rect, scoreboard->scoreboard->texture, UNSPECIFIED, FSD_BUTTON_X, FSD_BUTTON_Y + (3 * FY_OFFSET), FSD_BUTTON_W, FSD_BUTTON_H);
    // Renders name textures
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(scoreboard->name[i] == NULL || scoreboard->score[i] == NULL)
            continue;

        render_item(app, &scoreboard->name[i]->rect, scoreboard->name[i]->texture, UNSPECIFIED, F_NAME_X, F_NAME_Y + (i * FY_OFFSET), F_NAME_W, F_NAME_H);
        render_item(app, &scoreboard->score[i]->rect, scoreboard->score[i]->texture, UNSPECIFIED, F_SCORE_X, F_SCORE_Y + (i * FY_OFFSET), F_SCORE_W, F_SCORE_H);
    }
    // Renders return button
    render_item(app, &scoreboard->continue_button->rect, scoreboard->continue_button->texture, UNSPECIFIED, WINDOW_WIDTH / 2 - 129, WINDOW_HEIGHT - 145, 300, 120);
}

void render_fruits(App* app, Fruit* fruits[], SDL_Texture* fruit_sprite_tex, Pos fruit_texture[])
{
    SDL_Rect fruit_src[MAX_PLAYERS] = { 0 };
    SDL_Rect fruit_dst[MAX_PLAYERS] = { 0 };

    // create source and destination rectangles to render
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (fruits[i] == NULL) {
            // don't try to render a fruit that doesn't exist
            continue;
        }
        fruit_src[i].x = fruit_texture[fruits[i]->type].x;
        fruit_src[i].y = fruit_texture[fruits[i]->type].y;
        fruit_src[i].w = CELL_SIZE;
        fruit_src[i].h = CELL_SIZE;

        fruit_dst[i].x = fruits[i]->pos.x;
        fruit_dst[i].y = fruits[i]->pos.y;
        fruit_dst[i].w = CELL_SIZE;
        fruit_dst[i].h = CELL_SIZE;
    }
    // render fruits
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (fruit_src[i].w != CELL_SIZE || fruit_dst[i].w != CELL_SIZE) {
            // skip rendering if there is no fruit
            continue;
        }
        SDL_RenderCopyEx(app->renderer, fruit_sprite_tex, &fruit_src[i], &fruit_dst[i], 0, NULL, SDL_FLIP_NONE);
    }
}

void render_snakes(App* app, Player* players[], int nr_of_players, SDL_Texture* snake_sprite_textures[], Pos snake_texture[])
{
    SDL_Rect head_src;
    SDL_Rect head_dst;
    SDL_Rect body_src[MAX_SNAKE_LENGTH];
    SDL_Rect body_dst[MAX_SNAKE_LENGTH];
    SDL_Rect tail_src;
    SDL_Rect tail_dst;
    SDL_RendererFlip flip;
    int rotation;
    for (int i = 0; i < nr_of_players; i++) {
        if (players[i] == NULL) {
            // skip current player if player doesn't exist
            continue;
        } else if (!players[i]->alive) {
            // skip current player if player is dead
            continue;
        }
        // snake head rendering
        head_dst.x = players[i]->snake->head.pos.x;
        head_dst.y = players[i]->snake->head.pos.y;
        head_dst.w = CELL_SIZE;
        head_dst.h = CELL_SIZE;

        if (players[i]->snake->head.mouth_open) {
            // Snake open mouth next to fruit
            head_src.x = snake_texture[4].x;
            head_src.y = snake_texture[4].y;
        } else if (players[i]->snake->head.mouth_eating) {
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
        for (int j = 0; j < players[i]->snake->body_length; j++) {
            if (players[i]->snake->body[j].is_turn) {
                body_src[j].x = snake_texture[3].x;
                body_src[j].y = snake_texture[3].y;
            } else {
                body_src[j].x = snake_texture[1].x;
                body_src[j].y = snake_texture[1].y;
            }
            body_src[j].w = CELL_SIZE;
            body_src[j].h = CELL_SIZE;

            body_dst[j].x = players[i]->snake->body[j].pos.x;
            body_dst[j].y = players[i]->snake->body[j].pos.y;
            body_dst[j].w = CELL_SIZE;
            body_dst[j].h = CELL_SIZE;
        }

        // snake tail rendering
        tail_src.x = snake_texture[2].x;
        tail_src.y = snake_texture[2].y;
        tail_src.w = CELL_SIZE;
        tail_src.h = CELL_SIZE;
        tail_dst.x = players[i]->snake->tail.pos.x;
        tail_dst.y = players[i]->snake->tail.pos.y;
        tail_dst.w = CELL_SIZE;
        tail_dst.h = CELL_SIZE;

        // render head
        SDL_RenderCopyEx(app->renderer, snake_sprite_textures[players[i]->color], &head_src, &head_dst, players[i]->snake->head.angle, NULL, SDL_FLIP_NONE);
        // render body
        flip = SDL_FLIP_NONE;
        for (int j = 0; j < players[i]->snake->body_length; j++) {
            rotation = players[i]->snake->body[j].angle;
            flip = SDL_FLIP_NONE;
            if (players[i]->snake->body[j].is_turn) {
                rotation = players[i]->snake->body[j].turn_rotation;
                if (players[i]->snake->body[j].should_flip_vertical) {
                    flip = SDL_FLIP_VERTICAL;
                } else if (players[i]->snake->body[j].should_flip_horizontal) {
                    flip = SDL_FLIP_HORIZONTAL;
                }
            }
            SDL_RenderCopyEx(app->renderer, snake_sprite_textures[players[i]->color], &body_src[j], &body_dst[j], rotation, NULL, flip);
        }
        // render tail
        SDL_RenderCopyEx(app->renderer, snake_sprite_textures[players[i]->color], &tail_src, &tail_dst, players[i]->snake->tail.angle, NULL, SDL_FLIP_NONE);
    }
}
