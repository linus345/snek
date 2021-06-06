#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app.h"
#include "menu.h"
#include "rendering.h"
#include "sound.h"
#include "player.h"

//Checks if the mouse is hovering on an SDL_Rect.
bool hover_state(Screen_item* button, int Mx, int My)
{
    if (Mx >= button->rect.x && Mx <= button->rect.x + button->rect.w && My >= button->rect.y && My <= button->rect.y + button->rect.h) {
        return true;
    }
    return false;
}

bool is_hovering_over(SDL_Rect* rect, int mouse_x, int mouse_y)
{
    if(mouse_x >= rect->x && mouse_x <= rect->x + rect->w && mouse_y >= rect->y && mouse_y <= rect->y + rect->h) {
        return true;
    }
    return false;
}

// User input for ip address och port number
int main_menu(App* app, TTF_Font* font)
{
    int Mx, My;
    enum Menu_selection next_menu_state = MAIN_MENU;
    bool playsound = true;

    Menu* menu = init_menu_tex(app, font, "Start Game", "Exit Game");

    while (app->running && next_menu_state == MAIN_MENU) {
        //SDL_MouseButtonEvent mouse_event;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                // exit main loop
                app->running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (hover_state(menu->text1, Mx, My)) {
                    // Plays button press effect
                    play_sound(app->sound->press);
                    next_menu_state = SELECT_GAME;
                } else if (hover_state(menu->return_button, Mx, My)) {
                    app->running = false;
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    // exit main loop
                    app->running = false;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        render_menu(app, menu);

        //If-state for wether the text should switch color on hover or not
        if (hover_state(menu->text1, Mx, My)) {
            play_hover_sound(app->sound, &playsound);
            SDL_SetTextureColorMod(menu->text1->texture, 9, 34, 3);
        } else if (hover_state(menu->return_button, Mx, My)) {
            play_hover_sound(app->sound, &playsound);
            SDL_SetTextureColorMod(menu->return_button->texture, 127, 127, 127);
        } else {
            if (!playsound) // Makes sure the sound effect only plays once
                playsound = true;
            SDL_SetTextureColorMod(menu->text1->texture, 45, 93, 9);
            SDL_SetTextureColorMod(menu->return_button->texture, 255, 255, 255);
        }

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }
    free_menu(menu);
    return next_menu_state;
}

int select_game_menu(App* app, TTF_Font* font)
{
    int Mx, My;
    enum Menu_selection next_menu_state = SELECT_GAME;
    bool playsound = true;

    Menu* menu = init_menu_tex(app, font, "Join Multiplayer", "Back");

    while (app->running && next_menu_state == SELECT_GAME) {
        //SDL_MouseButtonEvent mouse_event;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                // exit main loop
                app->running = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (hover_state(menu->text1, Mx, My)) {
                    // Plays button press effect
                    play_sound(app->sound->press);
                    next_menu_state = JOIN_MULTIPLAYER;
                } else if (hover_state(menu->return_button, Mx, My)) {
                    // Plays button press effect
                    play_sound(app->sound->back);
                    next_menu_state = MAIN_MENU;
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    // exit main loop
                    /* return MAIN_MENU; */
                    next_menu_state = MAIN_MENU;
                    break;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        render_menu(app, menu);

        //If-state for wether the text should switch color on hover or not
        if (hover_state(menu->text1, Mx, My)) {
            play_hover_sound(app->sound, &playsound);
            SDL_SetTextureColorMod(menu->text1->texture, 9, 34, 3);
        } else if (hover_state(menu->return_button, Mx, My)) {
            play_hover_sound(app->sound, &playsound);
            SDL_SetTextureColorMod(menu->return_button->texture, 127, 127, 127);
        } else {
            if (!playsound) { // Makes sure the sound effect only plays once
                playsound = true;
            }
            SDL_SetTextureColorMod(menu->text1->texture, 45, 93, 9);
            SDL_SetTextureColorMod(menu->return_button->texture, 255, 255, 255);
        }

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }
    SDL_StopTextInput();
    free_menu(menu);
    return next_menu_state;
}

int join_multiplayer(App* app, TTF_Font* font)
{
    int Mx, My;
    enum Menu_selection next_menu_state = JOIN_MULTIPLAYER;
    bool ip = false, port = false, playsound = true;

    SDL_Color white = { 255, 255, 255, 255 };

    SDL_Surface* tmp_surface = NULL;

    Screen_item* background = menu_button_background(app, "./resources/Textures/background.png");
    Screen_item* background1 = menu_button_background(app, "./resources/Textures/ip_field.png");
    Screen_item* background2 = menu_button_background(app, "./resources/Textures/port_field.png");
    Screen_item* button = menu_button_background(app, "./resources/Textures/menuButton.png");
    Screen_item* text1 = menu_button_text(app, "Enter IP", font, white);
    Screen_item* text2 = menu_button_text(app, "Enter Port", font, white);
    Screen_item* text3 = menu_button_text(app, "Join", font, white);
    Screen_item* exit_button = menu_button_text(app, "Back", font, white);

    while (app->running && next_menu_state == JOIN_MULTIPLAYER) {
        //SDL_MouseButtonEvent mouse_event;
        SDL_Event event;
        if (SDL_PollEvent(&event)) {

            switch (event.type) {
            case SDL_QUIT:
                // exit main loop
                app->running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (hover_state(background1, Mx, My)) {
                    ip = true;
                    SDL_StartTextInput();
                } else if (hover_state(background2, Mx, My)) {
                    // Plays button press effect
                    play_sound(app->sound->press);
                    port = true;
                    SDL_StartTextInput();
                } else if (hover_state(text3, Mx, My)) {
                    // Plays button press effect
                    play_sound(app->sound->press);
                    next_menu_state = LOBBY;
                } else if (hover_state(exit_button, Mx, My)) {
                    // Plays button press effect
                    play_sound(app->sound->back);
                    next_menu_state = SELECT_GAME;
                }
                break;
            case SDL_TEXTINPUT:
                if (ip && strlen(app->ip) < 15) {
                    strcat(app->ip, event.text.text);
                } else if (port && strlen(app->port) < 4) {
                    strcat(app->port, event.text.text);
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_BACKSPACE:
                    if (ip && strlen(app->ip) > 0) {
                        app->ip[strlen(app->ip) - 1] = '\0';
                    } else if (port && strlen(app->port) > 0) {
                        app->port[strlen(app->port) - 1] = '\0';
                    }
                    break;
                case SDLK_RETURN:
                    SDL_StopTextInput();
                    ip = false;
                    port = false;
                    tmp_surface = NULL;
                    printf("IP: %s\n", app->ip);
                    printf("Port: %s\n", app->port);
                    break;
                case SDLK_ESCAPE:
                    next_menu_state = SELECT_GAME;
                    break;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        if (ip && port) {
            SDL_StopTextInput();
            ip = false;
            port = false;
            tmp_surface = NULL;
        }

        if (app->ip[0] == '\0' && ip) {
            tmp_surface = TTF_RenderText_Blended(font, "Enter IP", white);
        } else if (app->ip[0] != '\0' && ip) {
            tmp_surface = TTF_RenderText_Blended(font, app->ip, white);
        } else if (app->port[0] == '\0' && port) {
            tmp_surface = TTF_RenderText_Blended(font, "Enter Port", white);
        } else if (app->port[0] != '\0' && port) {
            tmp_surface = TTF_RenderText_Blended(font, app->port, white);
        }
        if (ip && tmp_surface != NULL) {
            text1->texture = SDL_CreateTextureFromSurface(app->renderer, tmp_surface);
        } else if (port && tmp_surface != NULL) {
            text2->texture = SDL_CreateTextureFromSurface(app->renderer, tmp_surface);
        }

        render_item(app, &background->rect, background->texture, BACKGROUND, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        render_item(app, &background1->rect, background1->texture, MENU_BUTTON, 380, 250, 500, 180);
        render_item(app, &background2->rect, background2->texture, MENU_BUTTON, 450, 390, 360, 150);
        render_item(app, &button->rect, button->texture, MENU_BUTTON, BUTTON_X, BUTTON_Y + 200, BUTTON_W, BUTTON_H);
        render_item(app, &text1->rect, text1->texture, MENU_BUTTON, 430, 290, 410, 110);
        render_item(app, &text2->rect, text2->texture, MENU_BUTTON, 500, 420, 250, 90);
        render_item(app, &text3->rect, text3->texture, MENU_BUTTON, TEXT_X, TEXT_Y + 200, TEXT_W, TEXT_H);
        render_item(app, &exit_button->rect, exit_button->texture, MENU_BUTTON, TEXT_X, TEXT_Y + 350, TEXT_W, TEXT_H);

        //If-state for wether the text should switch color on hover or not
        if (hover_state(text3, Mx, My)) {
            play_hover_sound(app->sound, &playsound);
            SDL_SetTextureColorMod(text3->texture, 9, 34, 3);

        } else if (hover_state(exit_button, Mx, My)) {
            play_hover_sound(app->sound, &playsound);
            SDL_SetTextureColorMod(exit_button->texture, 127, 127, 127);

        } else if (hover_state(text1, Mx, My)) {
            play_hover_sound(app->sound, &playsound);
            SDL_SetTextureColorMod(text1->texture, 127, 127, 127);

        } else if (hover_state(text2, Mx, My)) {
            play_hover_sound(app->sound, &playsound);
            SDL_SetTextureColorMod(text2->texture, 127, 127, 127);
        } else {
            if (!playsound) { // Makes sure the sound effect only plays once
                playsound = true;
            }
            SDL_SetTextureColorMod(text3->texture, 45, 93, 9);
            SDL_SetTextureColorMod(exit_button->texture, 255, 255, 255);
            SDL_SetTextureColorMod(text1->texture, 255, 255, 255);
            SDL_SetTextureColorMod(text2->texture, 255, 255, 255);
        }

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }
    // free memory on heap
    SDL_DestroyTexture(background->texture);
    free(background);
    SDL_DestroyTexture(background1->texture);
    free(background1);
    SDL_DestroyTexture(background2->texture);
    free(background2);
    SDL_DestroyTexture(button->texture);
    free(button);
    SDL_DestroyTexture(text1->texture);
    free(text1);
    SDL_DestroyTexture(text2->texture);
    free(text2);
    SDL_DestroyTexture(text3->texture);
    free(text3);
    SDL_DestroyTexture(exit_button->texture);
    free(exit_button);
    SDL_StopTextInput();
    return next_menu_state;
}
