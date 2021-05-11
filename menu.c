#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "app.h"
#include "game.h"
#include "menu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

SDL_Color

    black = { 0, 0, 0, 255 },
    gray = { 127, 127, 127, 255 },
    white = { 255, 255, 255, 255 },
    green = { 45, 93, 9, 255 },
    dark_green = { 9, 34, 3, 255 };

void menu(App* app, char* ip_address, char* port_nr)
{
    int menu_state = 0;

    while (app->running) {

        switch (menu_state) {
        case MAIN_MENU:
            menu_state = main_menu(app);
            break;
        case SELECT_GAME:
            menu_state = select_game_menu(app);
            break;
        case JOIN_MULTIPLAYER:
            menu_state = join_multiplayer(app, ip_address, port_nr);
            break; /*
        case HOST_MULTIPLAYER:
            menu_state = host_multiplayer(app, &fullscreen_bool);
            break;
        case HIGH_SCORE:
            menu_state = high_score(app, &fullscreen_bool);
            break;
        case SETTINGS:
            menu_state = settings(app, &fullscreen_bool);
            break;
        case START_GAME:
            return;*/
        }
    }
    return;
}

// Button memory allocator and texture loader.
Screen_item* menu_button_background(App* app, char resource[])
{
    Screen_item* button = malloc(sizeof(Screen_item));
    load_texture(app, &button->texture, resource);
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
                    free(button1);
                    free(button2);
                    free(button3);
                    free(text1);
                    free(text2);
                    free(text3);
                    free(exit_button);
                    return SELECT_GAME;
                } else if (hover_state(text2, Mx, My)) {
                    // Makes space on the heap
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
                    free(button1);
                    free(button2);
                    free(button3);
                    free(text1);
                    free(text2);
                    free(text3);
                    free(exit_button);
                    return SETTINGS;
                } else if (hover_state(exit_button, Mx, My)) {
                    app->running = false;
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_f:
                    SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    app->fullscreen = true;
                    // Makes space on the heap
                    free(button1);
                    free(button2);
                    free(button3);
                    free(text1);
                    free(text2);
                    free(text3);
                    free(exit_button);

                    return MAIN_MENU;
                case SDLK_ESCAPE:
                    // return main_menu sätt bool till false
                    SDL_SetWindowFullscreen(app->window, 0);
                    app->fullscreen = false;
                    free(button1);
                    free(button2);
                    free(button3);
                    free(text1);
                    free(text2);
                    free(text3);
                    free(exit_button);

                    return MAIN_MENU;
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
                if (hover_state(text1, Mx, My)) {
                    // Makes space on the heap
                    free(button1);
                    free(text1);
                    free(button2);
                    free(text2);
                    free(button3);
                    free(text3);
                    free(exit_button);
                    return START_GAME;

                } else if (hover_state(text2, Mx, My)) {
                    // Makes space on the heap
                    free(button1);
                    free(text1);
                    free(button2);
                    free(text2);
                    free(button3);
                    free(text3);
                    free(exit_button);
                    return JOIN_MULTIPLAYER;

                } else if (hover_state(text3, Mx, My)) {
                    // Makes space on the heap
                    free(button1);
                    free(text1);
                    free(button2);
                    free(text2);
                    free(button3);
                    free(text3);
                    free(exit_button);
                    return HOST_MULTIPLAYER;

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
                    case SDLK_f:
                        SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        app->fullscreen = true;
                        // Makes space on the heap
                        free(button1);
                        free(button2);
                        free(button3);
                        free(text1);
                        free(text2);
                        free(text3);
                        free(exit_button);

                        return MAIN_MENU;
                    case SDLK_ESCAPE:
                        // return main_menu sätt bool till false
                        SDL_SetWindowFullscreen(app->window, 0);
                        app->fullscreen = false;
                        free(button1);
                        free(button2);
                        free(button3);
                        free(text1);
                        free(text2);
                        free(text3);
                        free(exit_button);

                        return MAIN_MENU;
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

int join_multiplayer(App* app, char* ip_address, char* port_nr)
{

    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

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
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                // exit main loop
                app->running = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (hover_state(background1, Mx, My)) {
                    // Gets input from user
                    port_ip_input(app, ip_address, false);
                    strcpy(ip_address, ""); // !!!!!!!!!!!!!!!!!!!WARNING REMOVE LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                } else if (hover_state(background2, Mx, My)) {
                    // Gets input from user
                    port_ip_input(app, port_nr, true);
                    strcpy(port_nr, ""); // !!!!!!!!!!!!!!!!!!!WARNING REMOVE LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                } else if (hover_state(exit_button, Mx, My)) {
                    // Makes space on the heap
                    free(background1);
                    free(background2);
                    free(button);
                    free(text1);
                    free(text2);
                    free(text3);
                    free(exit_button);
                    return SELECT_GAME;
                } else if (hover_state(background2, Mx, My)) {
                    // Makes space on the heap
                    free(background1);
                    free(background2);
                    free(button);
                    free(text1);
                    free(text2);
                    free(text3);
                    free(exit_button);
                    return START_GAME;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
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
    return 0;
}
void port_ip_input(App* app, char input[], bool ip_not_port)
{

    bool done = false;
    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Screen_item* enter_ip_background = menu_button_background(app, "./resources/ip_field.png");
    Screen_item* enter_port_background = menu_button_background(app, "./resources/port_field.png");
    Screen_item* join_background = menu_button_background(app, "./resources/menuButton.png");

    Screen_item* enter_ip = menu_button_text(app, "Enter IP", font, white);
    Screen_item* enter_port = menu_button_text(app, "Enter Port", font, white);

    Screen_item* join_button = menu_button_text(app, "Join", font, green);
    Screen_item* exit_button = menu_button_text(app, "Back", font, white);

    Screen_item* text = NULL;

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
                // Add new text onto the end of our text
                strcat(input, event.text.text);
                text = menu_button_text(app, input, font, white);
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
                if (hover_state(exit_button, Mx, My)) {
                    done = true;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);
        //printf("Checkpoint 1\n");

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_item(app, &enter_ip_background->rect, enter_ip_background->texture, 230, 250, 500, 180);
        render_item(app, &enter_port_background->rect, enter_port_background->texture, 300, 390, 360, 150);
        render_item(app, &join_background->rect, join_background->texture, BUTTON_X, BUTTON_Y + 200, BUTTON_W, BUTTON_H);

        // Makes sure the button pressed is not renderd
        if (ip_not_port) {
            render_item(app, &enter_ip->rect, enter_ip->texture, 280, 290, 410, 110);
        } else if (!ip_not_port) {
            render_item(app, &enter_port->rect, enter_port->texture, 350, 420, 250, 90);
        }
        //printf("Checkpoint 2\n");

        //render_button(app, text); // Renders the user input
        render_item(app, &join_button->rect, join_button->texture, TEXT_X, TEXT_Y + 200, TEXT_W, TEXT_H);
        render_item(app, &exit_button->rect, exit_button->texture, TEXT_X, TEXT_Y + 350, TEXT_W, TEXT_H);

        //If-state for wether the text should switch color on hover or not
        //printf("Checkpoint 3\n");
        if (hover_state(exit_button, Mx, My)) {
            SDL_SetTextureColorMod(exit_button->texture, 127, 127, 127);

        } else {
            SDL_SetTextureColorMod(exit_button->texture, 255, 255, 255);
        }
        //printf("Checkpoint 4\n");
        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }

    SDL_StopTextInput();

    printf("\nthe text input is: %s\n\n", input);

    // Makes space on the heap
    free(enter_ip_background);
    free(enter_port_background);
    free(join_background);
    free(enter_ip);
    free(enter_port);
    free(join_button);
    free(exit_button);
    return;
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
} */