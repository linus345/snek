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
    //Screen_item* item;
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

Menu* init_menu_tex(App* app, char top_button[], char middle_button[], char lower_button[], char return_button[])
{
    Menu* menu = malloc(sizeof(Menu));

    // Creates background and menu button texture
    menu->background = menu_button_background(app, "./resources/Textures/background.png");
    menu->button = menu_button_background(app, "./resources/Textures/menuButton.png");

    // Creates text textures
    TTF_Font* font = TTF_OpenFont("./resources/Fonts/adventure.otf", 250);

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
    free(menu->background);
    free(menu->button);
    free(menu->text1);
    free(menu->text2);
    free(menu->text3);
    free(menu->return_button);

    free(menu);
}

Scoreboard* create_scoreboard(App* app, Player players[])
{
    Scoreboard* scoreboard = malloc(sizeof(Scoreboard));

    scoreboard->background = menu_button_background(app, "./resources/Textures/Forest_green.jpg");
    scoreboard->scoreboard = menu_button_background(app, "./resources/Textures/menuButton.png");
    /*
    Screen_item* goal_text = menu_button_text(app, "Goal", font, white_txt);
    Screen_item* goal_nr = menu_button_text(app, "250", font, white_txt);
    */

    TTF_Font* font = TTF_OpenFont("./resources/Fonts/adventure.otf", 250);

    scoreboard->name1 = menu_button_text(app, players[0].name, font, green);
    scoreboard->name2 = menu_button_text(app, players[1].name, font, green);
    scoreboard->name3 = menu_button_text(app, players[2].name, font, green);
    scoreboard->name4 = menu_button_text(app, players[3].name, font, green);
    char buffer[50];

    sprintf(buffer, "%d", player[0]->points);
    scoreboard->score1 = menu_button_text(app, buffer, font, green);
    sprintf(buffer, "%d", player[1]->points);
    scoreboard->score2 = menu_button_text(app, buffer, font, green);
    sprintf(buffer, "%d", player[2]->points);
    scoreboard->score3 = menu_button_text(app, buffer, font, green);
    sprintf(buffer, "%d", player[3]->points);
    scoreboard->score4 = menu_button_text(app, buffer, font, green);

    scoreboard->mute = menu_button_background(app, "./resources/Textures/speaker_icon.png");
    scoreboard->return_button = menu_button_background(app, "./resources/Textures/exit_button.png");

    return scoreboard;
}

void render_scoreboard(App* app, Scoreboard* scoreboard)
{
    // Renders background and button textures
    render_item(app, &scoreboard->background->rect, scoreboard->background->texture, STRETCH, 0, 0, 250, WINDOW_HEIGHT);
    render_item(app, &scoreboard->scoreboard->rect, scoreboard->scoreboard->texture, UNSPECIFIED, SD_BUTTON_X, SD_BUTTON_Y, SD_BUTTON_W, SD_BUTTON_H);
    render_item(app, &scoreboard->scoreboard->rect, scoreboard->scoreboard->texture, UNSPECIFIED, SD_BUTTON_X, SD_BUTTON_Y + Y_OFFSET, SD_BUTTON_W, SD_BUTTON_H);
    render_item(app, &scoreboard->scoreboard->rect, scoreboard->scoreboard->texture, UNSPECIFIED, SD_BUTTON_X, SD_BUTTON_Y + (2 * Y_OFFSET), SD_BUTTON_W, SD_BUTTON_H);
    render_item(app, &scoreboard->scoreboard->rect, scoreboard->scoreboard->texture, UNSPECIFIED, SD_BUTTON_X, SD_BUTTON_Y + (3 * Y_OFFSET), SD_BUTTON_W, SD_BUTTON_H);
    // Renders name textures
    render_item(app, &scoreboard->name1->rect, scoreboard->name1->texture, MENU_BUTTON, TEXT_X, TEXT_Y, TEXT_W, TEXT_H);
    render_item(app, &scoreboard->name2->rect, scoreboard->name2->texture, MENU_BUTTON, TEXT_X, TEXT_Y + (1 * 150), TEXT_W, TEXT_H);
    render_item(app, &scoreboard->name3->rect, scoreboard->name3->texture, MENU_BUTTON, TEXT_X, TEXT_Y + (2 * 150), TEXT_W, TEXT_H);
    render_item(app, &scoreboard->name4->rect, scoreboard->name4->texture, MENU_BUTTON, TEXT_X, TEXT_Y + (3 * 150), TEXT_W, TEXT_H);
    // Renders score textures
    render_item(app, &scoreboard->score1->rect, scoreboard->score1->texture, UNSPECIFIED, SCORE_X, SCORE_Y, SCORE_W, SCORE_H);
    render_item(app, &scoreboard->score2->rect, scoreboard->score2->texture, UNSPECIFIED, SCORE_X, SCORE_Y + (1 * 150), SCORE_W, SCORE_H);
    render_item(app, &scoreboard->score3->rect, scoreboard->score3->texture, UNSPECIFIED, SCORE_X, SCORE_Y + (2 * 150), SCORE_W, SCORE_H);
    render_item(app, &scoreboard->score4->rect, scoreboard->score4->texture, UNSPECIFIED, SCORE_X, SCORE_Y + (3 * 150), SCORE_W, SCORE_H);
    // Renders mute and exit button
    render_item(app, &scoreboard->return_button->rect, scoreboard->return_button->texture, UNSPECIFIED, 55, GAME_HEIGHT - 50, 50, 50);
    render_item(app, &scoreboard->mute->rect, scoreboard->mute->texture, UNSPECIFIED, 160, GAME_HEIGHT - 50, 50, 50);
}

void free_scoreboard(Scoreboard* scoreboard)
{
    free(scoreboard->background);
    free(scoreboard->scoreboard);
    free(scoreboard->name1);
    free(scoreboard->name2);
    free(scoreboard->name3);
    free(scoreboard->name4);
    free(scoreboard->score1);
    free(scoreboard->score2),
        free(scoreboard->score3);
    free(scoreboard->score4);
    free(scoreboard->mute);
    free(scoreboard->return_button);
    free(scoreboard);
}