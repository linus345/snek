#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app.h"
#include "game.h"
#include "menu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

SDL_Color
    black
    = { 0, 0, 0, 255 },
    gray = { 127, 127, 127, 255 },
    white = { 255, 255, 255, 255 },
    green = { 45, 93, 9, 255 },
    dark_green = { 9, 34, 3, 255 };

void menu(App* app)
{
    int menu_state = MAIN_MENU;
    while (app->running) {
        switch (menu_state) {
        case MAIN_MENU:
            menu_state = main_menu(app);
            break;
        case SELECT_GAME:
            menu_state = select_game_menu(app);
            break;
        case JOIN_MULTIPLAYER:
            menu_state = join_multiplayer(app);
            break;
        case HOST_MULTIPLAYER:
            menu_state = host_multiplayer(app);
            break;
        case HIGH_SCORE:
            menu_state = high_score(app);
            break;
        case SETTINGS:
            menu_state = settings(app);
            break;
        case TYPE_NAME:
            menu_state = type_name(app, name);
            break;
        case START_GAME:
            menu_state = game(app, name);
            break;
        }
    }
    return;
}

// Button memory allocator and texture loader.
Screen_item* menu_button_background(App* app, char resource[])
{
    Screen_item* button = malloc(sizeof(Screen_item));
    load_texture(app, &button->texture, resource);
    button->rect.x = x;
    button->rect.y = y;
    button->rect.w = w;
    button->rect.h = h;
    return button;
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
    item->texture = texture;
    return item;
}

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

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Screen_item* background = menu_button_background(app, "./resources/background.png");
    Screen_item* button1 = menu_button_background(app, "./resources/menuButton.png");
    Screen_item* text1 = menu_button_text(app, "Start Game", font, green);
    Screen_item* button2 = menu_button_background(app, "./resources/menuButton.png");
    Screen_item* text2 = menu_button_text(app, "High Score", font, green);
    Screen_item* button3 = menu_button_background(app, "./resources/menuButton.png");
    Screen_item* text3 = menu_button_text(app, "Settings", font, green);
    Screen_item* exit_button = menu_button_text(app, "Exit Game", font, white);

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
                if (hover_state(text1, Mx, My)) {
                    // Makes space on the heap
                    free(background);
                    free(button1);
                    free(button2);
                    free(button3);
                    free(text1);
                    free(text2);
                    free(text3);
                    free(exit_button);
                    return SELECT_GAME;
                } /* else if (hover_state(text2, Mx, My)) {
                        // Makes space on the heap
                        free(background);
                        free(button1);
                        free(button2);
                        free(button3);
                        free(text1);
                        free(text2);
                        free(text3);
                        free(exit_button);
                        return HIGH_SCORE;
                    } else if (hover_state(text3, Mx, My)) {
                        // Makes space on the heap
                        free(background);
                        free(button1);
                        free(button2);
                        free(button3);
                        free(text1);
                        free(text2);
                        free(text3);
                        free(exit_button);
                        return SETTINGS;
                    }*/
                else if (hover_state(exit_button, Mx, My)) {
                    app->running = false;
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_F11) {
                    if (app->fullscreen) {
                        SDL_SetWindowFullscreen(app->window, 0);
                        app->fullscreen = false;
                    } else {
                        SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        app->fullscreen = true;
                    }
                    // Makes space on the heap
                    free(background);
                    free(button1);
                    free(button2);
                    free(button3);
                    free(text1);
                    free(text2);
                    free(text3);
                    free(exit_button);
                    return MAIN_MENU;
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    // exit main loop
                    app->running = false;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        if (app->fullscreen) {
            render_item(app, &background->rect, background->texture, 0, 0, app->display.w, app->display.h);
        } else {
            render_item(app, &background->rect, background->texture, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        }

        render_item(app, &button1->rect, button1->texture, BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H);
        render_item(app, &text1->rect, text1->texture, TEXT_X, TEXT_Y, TEXT_W, TEXT_H);
        render_item(app, &button2->rect, button2->texture, BUTTON_X, BUTTON_Y + (1 * 150), BUTTON_W, BUTTON_H);
        render_item(app, &text2->rect, text2->texture, TEXT_X, TEXT_Y + (1 * 150), TEXT_W, TEXT_H);
        render_item(app, &button3->rect, button3->texture, BUTTON_X, BUTTON_Y + (2 * 150), BUTTON_W, BUTTON_H);
        render_item(app, &text3->rect, text3->texture, TEXT_X, TEXT_Y + (2 * 150), TEXT_W, TEXT_H);
        render_item(app, &exit_button->rect, exit_button->texture, TEXT_X, TEXT_Y + (3 * 150), TEXT_W, TEXT_H);

        //If-state for wether the text should switch color on hover or not
        if (hover_state(text1, Mx, My)) {
            SDL_SetTextureColorMod(text1->texture, 9, 34, 3);

        } else if (hover_state(text2, Mx, My)) {
            SDL_SetTextureColorMod(text2->texture, 9, 34, 3);

        } else if (hover_state(text3, Mx, My)) {
            SDL_SetTextureColorMod(text3->texture, 9, 34, 3);

        } else if (hover_state(exit_button, Mx, My)) {
            SDL_SetTextureColorMod(exit_button->texture, 127, 127, 127);

        } else {
            SDL_SetTextureColorMod(text1->texture, 45, 93, 9);
            SDL_SetTextureColorMod(text2->texture, 45, 93, 9);
            SDL_SetTextureColorMod(text3->texture, 45, 93, 9);
            SDL_SetTextureColorMod(exit_button->texture, 255, 255, 255);
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

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Screen_item* background = menu_button_background(app, "./resources/background.png");
    Screen_item* button1 = menu_button_background(app, "./resources/menuButton.png");
    Screen_item* text1 = menu_button_text(app, "Single Player", font, green);
    Screen_item* button2 = menu_button_background(app, "./resources/menuButton.png");
    Screen_item* text2 = menu_button_text(app, "Host Multiplayer", font, green);
    Screen_item* button3 = menu_button_background(app, "./resources/menuButton.png");
    Screen_item* text3 = menu_button_text(app, "Join Multiplayer", font, green);
    Screen_item* exit_button = menu_button_text(app, "Back", font, white);

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
                if (hover_state(start_game_button, Mx, My)) {
                    // Makes space on the heap
                    free(button1);
                    free(text1);
                    free(button2);
                    free(text2);
                    free(button3);
                    free(text3);
                    free(exit_button);
                    return START_GAME;

                } /* else if (hover_state(text2, Mx, My)) {
                    // Makes space on the heap
                    free(button1);
                    free(text1);
                    free(button2);
                    free(text2);
                    free(button3);
                    free(text3);
                    free(exit_button);
                    return HOST_MULTIPLAYER;

                } */
                else if (hover_state(text3, Mx, My)) {
                    // Makes space on the heap
                    free(button1);
                    free(text1);
                    free(button2);
                    free(text2);
                    free(button3);
                    free(text3);
                    free(exit_button);
                    return JOIN_MULTIPLAYER;

                } else if (hover_state(exit_button, Mx, My)) {
                    // Makes space on the heap
                    free(button1);
                    free(text1);
                    free(button2);
                    free(text2);
                    free(button3);
                    free(text3);
                    free(exit_button);
                    return MAIN_MENU;
                }
                break;
            case SDL_KEYDOWN:
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
                    free(background);
                    free(button1);
                    free(button2);
                    free(button3);
                    free(text1);
                    free(text2);
                    free(text3);
                    free(exit_button);
                    return SELECT_GAME;
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

        if (app->fullscreen) {
            render_item(app, &background->rect, background->texture, 0, 0, app->display.w, app->display.h);
        } else {
            render_item(app, &background->rect, background->texture, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        }

        render_item(app, &button1->rect, button1->texture, BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H);
        render_item(app, &text1->rect, text1->texture, TEXT_X, TEXT_Y, TEXT_W, TEXT_H);
        render_item(app, &button2->rect, button2->texture, BUTTON_X, BUTTON_Y + (1 * 150), BUTTON_W, BUTTON_H);
        render_item(app, &text2->rect, text2->texture, TEXT_X, TEXT_Y + (1 * 150), TEXT_W, TEXT_H);
        render_item(app, &button3->rect, button3->texture, BUTTON_X, BUTTON_Y + (2 * 150), BUTTON_W, BUTTON_H);
        render_item(app, &text3->rect, text3->texture, TEXT_X, TEXT_Y + (2 * 150), TEXT_W, TEXT_H);
        render_item(app, &exit_button->rect, exit_button->texture, TEXT_X, TEXT_Y + (3 * 150), TEXT_W, TEXT_H);

        //If-state for wether the text should switch color on hover or not
        if (hover_state(start_game_button, Mx, My)) {
            SDL_SetTextureColorMod(start_game_button->texture, 9, 34, 3);

        } else if (hover_state(high_score_button, Mx, My)) {
            SDL_SetTextureColorMod(high_score_button->texture, 9, 34, 3);

        } else if (hover_state(settings_button, Mx, My)) {
            SDL_SetTextureColorMod(settings_button->texture, 9, 34, 3);

        } else if (hover_state(exit_button, Mx, My)) {
            SDL_SetTextureColorMod(exit_button->texture, 127, 127, 127);
        } else {
            SDL_SetTextureColorMod(start_game_button->texture, 45, 93, 9);
            SDL_SetTextureColorMod(high_score_button->texture, 45, 93, 9);
            SDL_SetTextureColorMod(settings_button->texture, 45, 93, 9);
            SDL_SetTextureColorMod(exit_button->texture, 255, 255, 255);
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
    int Mx, My;
    bool ip = false, port = false;
    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);
    SDL_Surface* tmp_surface = NULL;

    Screen_item* background = menu_button_background(app, "./resources/background.png");
    Screen_item* background1 = menu_button_background(app, "./resources/ip_field.png");
    Screen_item* background2 = menu_button_background(app, "./resources/port_field.png");
    Screen_item* button = menu_button_background(app, "./resources/menuButton.png");
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
                    port = true;
                    SDL_StartTextInput();
                } else if (hover_state(button, Mx, My)) {
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
                    free(text1);
                    free(text2);
                    free(text3);
                    free(exit_button);
                    return JOIN_MULTIPLAYER;
                case SDLK_ESCAPE:
                    return SELECT_GAME;
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_f:
                    SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    *fullscreen = true;
                    // Makes space on the heap
                    free(single_player_background);
                    free(single_player_button);
                    free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    free(join_multiplayer_background);
                    free(join_multiplayer_button);
                    free(return_button);

                    return SELECT_GAME;
                case SDLK_ESCAPE:
                    // return main_menu sätt bool till false
                    SDL_SetWindowFullscreen(app->window, 0);
                    *fullscreen = false;
                    free(single_player_background);
                    free(single_player_button);
                    free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    free(join_multiplayer_background);
                    free(join_multiplayer_button);
                    free(return_button);

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
        if (app->fullscreen) {
            render_item(app, &background->rect, background->texture, 0, 0, app->display.w, app->display.h);
        } else {
            render_item(app, &background->rect, background->texture, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
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

        render_item(app, &background1->rect, background1->texture, 230, 250, 500, 180);
        render_item(app, &background2->rect, background2->texture, 300, 390, 360, 150);
        render_item(app, &button->rect, button->texture, BUTTON_X, BUTTON_Y + 200, BUTTON_W, BUTTON_H);
        render_item(app, &text1->rect, text1->texture, 280, 290, 410, 110);
        render_item(app, &text2->rect, text2->texture, 350, 420, 250, 90);
        render_item(app, &text3->rect, text3->texture, TEXT_X, TEXT_Y + 200, TEXT_W, TEXT_H);
        render_item(app, &exit_button->rect, exit_button->texture, TEXT_X, TEXT_Y + 350, TEXT_W, TEXT_H);

        //If-state for wether the text should switch color on hover or not
        if (hover_state(text3, Mx, My)) {
            SDL_SetTextureColorMod(text3->texture, 9, 34, 3);

        } else if (hover_state(exit_button, Mx, My)) {
            SDL_SetTextureColorMod(exit_button->texture, 127, 127, 127);

        } else if (hover_state(text1, Mx, My)) {
            SDL_SetTextureColorMod(text1->texture, 127, 127, 127);

        } else if (hover_state(text2, Mx, My)) {
            SDL_SetTextureColorMod(text2->texture, 127, 127, 127);
        } else {
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
}
/*
int host_multiplayer(App* app, bool* fullscreen_bool)
{

    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* return_button = NULL;

    if (*fullscreen) {
        background_view.h = r->h;
        background_view.w = r->w;
        return_button = menu_button_text(app, r->w / 2 - 70, 865, 200, 75, "Back", font, color_select(WHITE));
    } else if (!*fullscreen) {
        background_view.h = WINDOW_HEIGHT;
        background_view.w = WINDOW_WIDTH;
        return_button = menu_button_text(app, 527, 865, 200, 75, "Back", font, color_select(WHITE));
    }

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
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_f:
                    SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    *fullscreen = true;
                    // Makes space on the heap

                    free(return_button);

                    return SETTINGS;
                case SDLK_ESCAPE:
                    // return main_menu sätt bool till false
                    SDL_SetWindowFullscreen(app->window, 0);
                    *fullscreen = false;
                    free(return_button);

                    return SETTINGS;
                }
                break;
            }
            break;
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, return_button);

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

int high_score(App* app, SDL_Rect* r, bool* fullscreen)
{
    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* return_button = NULL;

    if (*fullscreen) {
        background_view.h = r->h;
        background_view.w = r->w;
        return_button = menu_button_text(app, r->w / 2 - 70, 865, 200, 75, "Back", font, color_select(WHITE));
    } else if (!*fullscreen) {
        background_view.h = WINDOW_HEIGHT;
        background_view.w = WINDOW_WIDTH;
        return_button = menu_button_text(app, 527, 865, 200, 75, "Back", font, color_select(WHITE));
    }

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
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_f:
                    SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    *fullscreen = true;
                    // Makes space on the heap
                    free(return_button);

                    return HIGH_SCORE;
                case SDLK_ESCAPE:
                    // return main_menu sätt bool till false
                    SDL_SetWindowFullscreen(app->window, 0);
                    *fullscreen = false;
                    free(return_button);

                    return HIGH_SCORE;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, return_button);

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

int settings(App* app, SDL_Rect* r, bool* fullscreen)
{
    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* return_button = NULL;

    if (*fullscreen) {
        background_view.h = r->h;
        background_view.w = r->w;
        return_button = menu_button_text(app, r->w / 2 - 70, 865, 200, 75, "Back", font, color_select(WHITE));
    } else if (!*fullscreen) {
        background_view.h = WINDOW_HEIGHT;
        background_view.w = WINDOW_WIDTH;
        return_button = menu_button_text(app, 527, 865, 200, 75, "Back", font, color_select(WHITE));
    }

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
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_f:
                    SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    *fullscreen = true;
                    // Makes space on the heap

                    free(return_button);

                    return SETTINGS;
                case SDLK_ESCAPE:
                    // return main_menu sätt bool till false
                    SDL_SetWindowFullscreen(app->window, 0);
                    *fullscreen = false;
                    free(return_button);

                    return SETTINGS;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, return_button);

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

int type_name(App* app, char name[])
{

    bool done = false;
    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* type_name_background = menu_button_background(app, 450, 400, 360, 150, "./resources/menuButton.png");
    Button* text = NULL;
    Button* return_button = menu_button_text(app, 527, 865, 200, 75, "Back", font, color_select(WHITE));

    SDL_StartTextInput();
    SDL_Event event;
    while (!done) {

        printf("While loop\n");

        if (SDL_PollEvent(&event)) {
            printf("If-statement successful\n");
            switch (event.type) {
            case SDL_QUIT:
                app->running = false;
                done = true;
                break;
            case SDL_TEXTINPUT:
                /* Add new text onto the end of our text */
                strcat(name, event.text.text);
                text = menu_button_text(app, 485, 430, 290, 90, name, font, color_select(GREEN));
                break;
            case SDL_KEYDOWN:
                // enter key pressed?
                switch (event.key.keysym.sym) {
                case SDLK_RETURN:
                    done = true;
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (hover_state(return_button, Mx, My)) {
                    return SELECT_GAME;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);
        printf("Checkpoint\n");

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, type_name_background);
        if (text != NULL) {
            render_button(app, text);
        } // Renders the user input
        render_button(app, return_button);

        //If-state for wether the text should switch color on hover or not

        if (hover_state(return_button, Mx, My)) {
            SDL_SetTextureColorMod(return_button->texture, 127, 127, 127);

        } else {
            SDL_SetTextureColorMod(return_button->texture, 255, 255, 255);
        }
        printf("Checkpoint\n");
        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }

    SDL_StopTextInput();

    printf("\nthe text input is: %s\n\n", name);

    // Makes space on the heap
    free(type_name_background);
    free(text);
    free(return_button);

    return START_GAME;
}
