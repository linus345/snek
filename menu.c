#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app.h"
#include "game.h"
#include "menu.h"
#include "rendering.h"
#include "sound.h"

//Checks if the mouse is hovering on an SDL_Rect.
bool hover_state(Screen_item* button, int Mx, int My)
{
    if (Mx >= button->rect.x && Mx <= button->rect.x + button->rect.w && My >= button->rect.y && My <= button->rect.y + button->rect.h) {
        return true;
    }
    return false;
}

// User input for ip address och port number
int main_menu(App* app)
{
    int Mx, My;

    bool playsound = true;

    Menu* menu = init_menu_tex(app, "Start Game", "High Score", "Settings", "Exit Game");

    while (app->running) {
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
                    // Makes space on the heap
                    free_menu(menu);
                    return SELECT_GAME;
                } /* else if (hover_state(text2, Mx, My)) {
                        // Plays button press effect
                        play_sound(app->sound->press);
                        // Makes space on the heap
                        free_menu(menu);
                        return HIGH_SCORE;
                    } else if (hover_state(text3, Mx, My)) {
                        // Plays button press effect
                        play_sound(app->sound->press);
                        // Makes space on the heap
                        free_menu(menu);
                        return SETTINGS;
                    }*/
                else if (hover_state(menu->return_button, Mx, My)) {
                    app->running = false;
                }
                break;
            case SDL_KEYDOWN:
                /*
                if (event.key.keysym.sym == SDLK_F11) {
                    if (app->fullscreen) {
                        SDL_SetWindowFullscreen(app->window, 0);
                        app->fullscreen = false;
                    } else {
                        SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        app->fullscreen = true;
                    }
                    // Makes space on the heap
                    free_menu(menu);
                    return MAIN_MENU;
                } else */
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
            if (playsound) { // Makes sure the sound effect only plays once
                play_sound(app->sound->hover); // Plays hover effect
                playsound = false;
            }
            SDL_SetTextureColorMod(menu->text1->texture, 9, 34, 3);

        } else if (hover_state(menu->text2, Mx, My)) {
            if (playsound) { // Makes sure the sound effect only plays once
                play_sound(app->sound->hover); // Plays hover effect
                playsound = false;
            }
            SDL_SetTextureColorMod(menu->text2->texture, 9, 34, 3);

        } else if (hover_state(menu->text3, Mx, My)) {
            if (playsound) { // Makes sure the sound effect only plays once
                play_sound(app->sound->hover); // Plays hover effect
                playsound = false;
            }
            SDL_SetTextureColorMod(menu->text3->texture, 9, 34, 3);

        } else if (hover_state(menu->return_button, Mx, My)) {
            if (playsound) { // Makes sure the sound effect only plays once
                play_sound(app->sound->hover); // Plays hover effect
                playsound = false;
            }
            SDL_SetTextureColorMod(menu->return_button->texture, 127, 127, 127);

        } else {
            if (!playsound) // Makes sure the sound effect only plays once
                playsound = true;
            SDL_SetTextureColorMod(menu->text1->texture, 45, 93, 9);
            SDL_SetTextureColorMod(menu->text2->texture, 45, 93, 9);
            SDL_SetTextureColorMod(menu->text3->texture, 45, 93, 9);
            SDL_SetTextureColorMod(menu->return_button->texture, 255, 255, 255);
        }

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }
    return 0;
}

int select_game_menu(App* app)
{

    int Mx, My;

    bool playsound = true;

    Menu* menu = init_menu_tex(app, "Single Player", "Host Multiplayer", "Join Multiplayer", "Back");

    while (app->running) {

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
                    // Makes space on the heap
                    free_menu(menu);
                    return TYPE_NAME;

                } /* else if (hover_state(text2, Mx, My)) {
                    // Plays button press effect
                    play_sound(app->sound->press);
                    // Makes space on the heap
                    free_menu(menu);
                    return HOST_MULTIPLAYER;

                } */
                else if (hover_state(menu->text3, Mx, My)) {
                    // Plays button press effect
                    play_sound(app->sound->press);
                    // Makes space on the heap
                    free_menu(menu);
                    return JOIN_MULTIPLAYER;

                } else if (hover_state(menu->return_button, Mx, My)) {
                    // Plays button press effect
                    play_sound(app->sound->back);
                    // Makes space on the heap
                    free_menu(menu);
                    return MAIN_MENU;
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                /*
                case SDLK_F11:
                    if (app->fullscreen) {
                        SDL_SetWindowFullscreen(app->window, 0);
                        app->fullscreen = false;
                    } else {
                        SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        app->fullscreen = true;
                    }
                    // Makes space on the heap
                    free_menu(menu);
                    return SELECT_GAME;
                */
                case SDLK_ESCAPE:
                    // exit main loop
                    return MAIN_MENU;
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
            if (playsound) { // Makes sure the sound effect only plays once
                play_sound(app->sound->hover); // Plays hover effect
                playsound = false;
            }
            SDL_SetTextureColorMod(menu->text1->texture, 9, 34, 3);

        } else if (hover_state(menu->text2, Mx, My)) {
            if (playsound) { // Makes sure the sound effect only plays once
                play_sound(app->sound->hover); // Plays hover effect
                playsound = false;
            }
            SDL_SetTextureColorMod(menu->text2->texture, 9, 34, 3);

        } else if (hover_state(menu->text3, Mx, My)) {
            if (playsound) { // Makes sure the sound effect only plays once
                play_sound(app->sound->hover); // Plays hover effect
                playsound = false;
            }
            SDL_SetTextureColorMod(menu->text3->texture, 9, 34, 3);

        } else if (hover_state(menu->return_button, Mx, My)) {
            if (playsound) { // Makes sure the sound effect only plays once
                play_sound(app->sound->hover); // Plays hover effect
                playsound = false;
            }
            SDL_SetTextureColorMod(menu->return_button->texture, 127, 127, 127);
        } else {
            if (!playsound) { // Makes sure the sound effect only plays once
                playsound = true;
            }
            SDL_SetTextureColorMod(menu->text1->texture, 45, 93, 9);
            SDL_SetTextureColorMod(menu->text2->texture, 45, 93, 9);
            SDL_SetTextureColorMod(menu->text3->texture, 45, 93, 9);
            SDL_SetTextureColorMod(menu->return_button->texture, 255, 255, 255);
        }

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }
    SDL_StopTextInput();
    return 0;
}

int join_multiplayer(App* app)
{
    // TODO: move type_name to lobby
    type_name(app);
    int Mx, My;
    bool ip = false, port = false, playsound = true;

    TTF_Font* font = TTF_OpenFont("./resources/Fonts/adventure.otf", 250);

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

    while (app->running) {

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
                    // Makes space on the heap
                    free(background);
                    free(background1);
                    free(background2);
                    free(button);
                    free(text1);
                    free(text2);
                    free(text3);
                    free(exit_button);
                    SDL_StopTextInput();
                    return START_GAME;

                } else if (hover_state(exit_button, Mx, My)) {
                    // Plays button press effect
                    play_sound(app->sound->back);
                    // Makes space on the heap
                    free(background);
                    free(background1);
                    free(background2);
                    free(button);
                    free(text1);
                    free(text2);
                    free(text3);
                    free(exit_button);
                    SDL_StopTextInput();
                    return SELECT_GAME;
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
                /*
                case SDLK_F11:
                    if (app->fullscreen) {
                        SDL_SetWindowFullscreen(app->window, 0);
                        app->fullscreen = false;
                    } else {
                        SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        app->fullscreen = true;
                    }
                    // Makes space on the heap
                    free(background);
                    free(background1);
                    free(background2);
                    free(button);
                    free(text1);
                    free(text2);
                    free(text3);
                    free(exit_button);
                    return JOIN_MULTIPLAYER;
                */
                case SDLK_ESCAPE:
                    return SELECT_GAME;
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
            if (playsound) { // Makes sure the sound effect only plays once
                play_sound(app->sound->hover); // Plays hover effect
                playsound = false;
            }
            SDL_SetTextureColorMod(text3->texture, 9, 34, 3);

        } else if (hover_state(exit_button, Mx, My)) {
            if (playsound) { // Makes sure the sound effect only plays once
                play_sound(app->sound->hover); // Plays hover effect
                playsound = false;
            }
            SDL_SetTextureColorMod(exit_button->texture, 127, 127, 127);

        } else if (hover_state(text1, Mx, My)) {
            if (playsound) { // Makes sure the sound effect only plays once
                play_sound(app->sound->hover); // Plays hover effect
                playsound = false;
            }
            SDL_SetTextureColorMod(text1->texture, 127, 127, 127);

        } else if (hover_state(text2, Mx, My)) {
            if (playsound) { // Makes sure the sound effect only plays once
                play_sound(app->sound->hover); // Plays hover effect
                playsound = false;
            }
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
    return 0;
}
/*
int host_multiplayer(App* app, bool* fullscreen_bool)
{

    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, white, fullscreen_bool);

    while (app->running) {

        //SDL_MouseButtonEvent mouse_event;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                // exit main loop
                app->running = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (hover_state(return_button, Mx, My)) {
                    // Makes space on the heap
                    free(return_button);
                    return SELECT_GAME;
                }
            }
            break;
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, return_button, fullscreen_bool);

        //If-state for wether the text should switch color on hover or not
        if (hover_state(return_button, Mx, My)) {
            SDL_SetTextureColorMod(return_button->texture, 127, 127, 127);

        } else {
            SDL_SetTextureColorMod(return_button->texture, 255, 255, 255);
        }

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }
}

int high_score(App* app, bool* fullscreen_bool)
{

    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, white, fullscreen_bool);

    while (app->running) {

        //SDL_MouseButtonEvent mouse_event;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                // exit main loop
                app->running = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (hover_state(return_button, Mx, My)) {
                    // Makes space on the heap
                    free(return_button);
                    return MAIN_MENU;
                }
            }
            break;
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, return_button, fullscreen_bool);

        //If-state for wether the text should switch color on hover or not
        if (hover_state(return_button, Mx, My)) {
            SDL_SetTextureColorMod(return_button->texture, 127, 127, 127);

        } else {
            SDL_SetTextureColorMod(return_button->texture, 255, 255, 255);
        }

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }
}

int settings(App* app, bool* fullscreen_bool)
{

    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, white, fullscreen_bool);

    while (app->running) {

        //SDL_MouseButtonEvent mouse_event;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                // exit main loop
                app->running = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (hover_state(return_button, Mx, My)) {
                    // Makes space on the heap
                    free(return_button);
                    return MAIN_MENU;
                }
            }
            break;
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, return_button, fullscreen_bool);

        //If-state for wether the text should switch color on hover or not
        if (hover_state(return_button, Mx, My)) {
            SDL_SetTextureColorMod(return_button->texture, 127, 127, 127);

        } else {
            SDL_SetTextureColorMod(return_button->texture, 255, 255, 255);
        }

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }
}
*/

int type_name(App* app)
{
    int Mx, My;

    bool playsound = true;

    SDL_Color white = { 255, 255, 255, 255 };

    TTF_Font* font = TTF_OpenFont("./resources/Fonts/adventure.otf", 250);
    SDL_Surface* tmp_surface = NULL;

    Screen_item* background = menu_button_background(app, "./resources/Textures/background.png");
    Screen_item* button = menu_button_background(app, "./resources/Textures/menuButton.png");
    Screen_item* text = menu_button_text(app, "Enter name", font, white);
    Screen_item* exit_button = menu_button_text(app, "Back", font, white);

    while (app->running) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                // exit main loop
                app->running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (hover_state(button, Mx, My)) {
                    // Plays button press effect
                    play_sound(app->sound->press);
                    printf("Pressed\n");
                    SDL_StartTextInput();
                } else if (hover_state(exit_button, Mx, My)) {
                    // Plays button press effect
                    play_sound(app->sound->back);
                    free(background);
                    free(button);
                    free(text);
                    free(exit_button);
                    return SELECT_GAME;
                }
                break;

            case SDL_TEXTINPUT:
                // TODO: insert name into player->name instead
                if (strlen(app->player_name) < 15) {
                    strcat(app->player_name, event.text.text);
                }
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {

                case SDLK_BACKSPACE:
                    if (strlen(app->player_name) > 0) {
                        app->player_name[strlen(app->player_name) - 1] = '\0';
                    }
                    break;
                case SDLK_RETURN:
                    // Plays button press effect
                    play_sound(app->sound->back);
                    SDL_StopTextInput();
                    tmp_surface = NULL;
                    printf("Name: %s\n", app->player_name);
                    return START_GAME;
                    break;
                case SDLK_F11:
                    if (app->fullscreen) {
                        SDL_SetWindowFullscreen(app->window, 0);
                        app->fullscreen = false;
                    } else {
                        SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        app->fullscreen = true;
                    }
                    // Makes space on the heap
                    free(background);
                    free(button);
                    free(text);
                    free(exit_button);
                    return TYPE_NAME;
                case SDLK_ESCAPE:
                    free(background);
                    free(button);
                    free(text);
                    free(exit_button);
                    return TYPE_NAME;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        if (app->player_name[0] == '\0') {
            tmp_surface = TTF_RenderText_Blended(font, "Enter name", white);
        } else if (app->player_name[0] != '\0') {
            tmp_surface = TTF_RenderText_Blended(font, app->player_name, white);
        }

        if (tmp_surface != 0) {
            text->texture = SDL_CreateTextureFromSurface(app->renderer, tmp_surface);
        } else if (tmp_surface == NULL) {
            printf("%s\n", SDL_GetError());
        }

        render_item(app, &background->rect, background->texture, BACKGROUND, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        render_item(app, &button->rect, button->texture, MENU_BUTTON, BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H);
        render_item(app, &text->rect, text->texture, MENU_BUTTON, TEXT_X, TEXT_Y, TEXT_W, TEXT_H);
        render_item(app, &exit_button->rect, exit_button->texture, MENU_BUTTON, TEXT_X, TEXT_Y + 350, TEXT_W, TEXT_H);

        if (hover_state(exit_button, Mx, My)) {
            if (playsound) { // Makes sure the sound effect only plays once
                play_sound(app->sound->hover); // Plays hover effect
                playsound = false;
            }
            SDL_SetTextureColorMod(exit_button->texture, 127, 127, 127);

        } else {
            if (!playsound) { // Makes sure the sound effect only plays once
                playsound = true;
            }
            SDL_SetTextureColorMod(exit_button->texture, 255, 255, 255);
        }

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }
}
