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

SDL_Color*

    black = { 0, 0, 0 },
    gray = { 127, 127, 127 },
    white = { 255, 255, 255 },
    green = { 45, 93, 9 },
    dark_green = { 9, 34, 3 };

// Universal button renderer for menus
Button* menu_button_background(App* app, int x, int y, int w, int h, char resource[], bool *fullscreen_bool)
{
    Button* button = malloc(sizeof(Button));
    load_texture(app, &button->texture, resource);
    button->rect.x = x;
    button->rect.y = y;
    button->rect.w = w;
    button->rect.h = h;
    return button;
}

// Universal text renderer for buttons
Button* menu_button_text(App* app, int x, int y, int w, int h, char* text, TTF_Font* font, SDL_Color color, bool *fullscreen_bool)
{
    Button* button = malloc(sizeof(Button));
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;button->rect = rect;
    button->texture = texture;
    return button;
}

void render_button(App* app, Button* button, bool *fullscreen)
{
    SDL_RenderCopy(app->renderer, button->texture, NULL, &button->rect);
}

bool hover_state(Button* button, int Mx, int My)
{
    if (Mx >= button->rect.x && Mx <= button->rect.x + button->rect.w && My >= button->rect.y && My <= button->rect.y + button->rect.h) {
        return true;
    }
    return false;
}

// User input for ip address och port number
void port_ip_input(App* app, char input[], int x, int y, int w, int h, bool ip_not_port, bool *fullscreen_bool)
{

    bool done = false;
    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* enter_ip_background = menu_button_background(app, 230, 250, 500, 180, "./resources/ip_field.png", fullscreen_bool);
    Button* enter_port_background = menu_button_background(app, 300, 390, 360, 150, "./resources/port_field.png", fullscreen_bool);
    Button* join_background = menu_button_background(app, 300, 600, 360, 150, "./resources/menuButton.png", fullscreen_bool);

    Button* enter_ip = menu_button_text(app, 280, 290, 410, 110, "Enter IP", font, white, fullscreen_bool);
    Button* enter_port = menu_button_text(app, 350, 420, 250, 90, "Enter Port", font, white, fullscreen_bool);

    Button* join_button = menu_button_text(app, 337, 630, 290, 90, "Join", font, green, fullscreen_bool);
    Button* return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, white, fullscreen_bool);

    Button* text = NULL;

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
                strcat(input, event.text.text);
                text = menu_button_text(app, x, y, w, h, input, font, white, fullscreen_bool);
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
                    done = true;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);
        printf("Checkpoint\n");

        if (fullscreen_bool) {

        }

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, enter_ip_background, fullscreen_bool);
        render_button(app, enter_port_background, fullscreen_bool);
        render_button(app, join_background, fullscreen_bool);

        // Makes sure the button pressed is not renderd
        if (ip_not_port) {
            render_button(app, enter_ip, fullscreen_bool);
        } else if (!ip_not_port) {
            render_button(app, enter_port, fullscreen_bool);
        }

        render_button(app, text, fullscreen_bool); // Renders the user input
        render_button(app, join_button, fullscreen_bool);
        render_button(app, return_button, fullscreen_bool);

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

    printf("\nthe text input is: %s\n\n", input);

    // Makes space on the heap
    free(enter_ip_background);
    free(enter_port_background);
    free(join_background);
    free(enter_ip);
    free(enter_port);
    free(join_button);
    free(return_button);
}

int main_menu(App* app, SDL_Rect* fullscreen, bool* fullscreen_bool)
{

    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, fullscreen->w, fullscreen->h };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* start_game_background = NULL;
    Button* start_game_button = NULL;
    Button* high_score_background = NULL;
    Button* high_score_button = NULL;
    Button* settings_background = NULL;
    Button* settings_button = NULL;
    Button* exit_button = NULL;


    while (app->running) {

        start_game_background = menu_button_background(app, 300, 400, 360, 150, "./resources/menuButton.png", fullscreen_bool);
        start_game_button = menu_button_text(app, 337, 430, 290, 90, "Start Game", font, green, fullscreen_bool);
        high_score_background = menu_button_background(app, 300, 550, 360, 150, "./resources/menuButton.png", fullscreen_bool);
        high_score_button = menu_button_text(app, 337, 580, 290, 90, "High Score", font, green, fullscreen_bool);
        settings_background = menu_button_background(app, 300, 700, 360, 150, "./resources/menuButton.png", fullscreen_bool);
        settings_button = menu_button_text(app, 337, 730, 290, 90, "Settings", font, green, fullscreen_bool);
        exit_button = menu_button_text(app, 380, 865, 200, 75, "Exit Game", font, white, fullscreen_bool);

        if (fullscreen_bool) {
            optimizeFullscreen(&start_game_background->rect);
            optimizeFullscreen(&start_game_button->rect);
            optimizeFullscreen(&high_score_background->rect);
            optimizeFullscreen(&high_score_button->rect);
            optimizeFullscreen(&settings_background->rect);
            optimizeFullscreen(&settings_button->rect);
            optimizeFullscreen(&exit_button->rect);
        }        

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
                    free(start_game_background);
                    free(start_game_button);
                    free(high_score_background);
                    free(high_score_button);
                    free(settings_background);
                    free(settings_button);
                    free(exit_button);
                    return SELECT_GAME;
                } else if (hover_state(high_score_button, Mx, My)) {
                    // Makes space on the heap
                    free(start_game_background);
                    free(start_game_button);
                    free(high_score_background);
                    free(high_score_button);
                    free(settings_background);
                    free(settings_button);
                    free(exit_button);
                    return HIGH_SCORE;
                } else if (hover_state(settings_button, Mx, My)) {
                    // Makes space on the heap
                    free(start_game_background);
                    free(start_game_button);
                    free(high_score_background);
                    free(high_score_button);
                    free(settings_background);
                    free(settings_button);
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
                    *fullscreen_bool=true;
                    // Makes space on the heap
                    free(start_game_background);
                    free(start_game_button);
                    free(high_score_background);
                    free(high_score_button);
                    free(settings_background);
                    free(settings_button);
                    free(exit_button);
                    
                    return MAIN_MENU;
                case SDLK_ESCAPE:
                    // return main_menu sätt bool till false
                    SDL_SetWindowFullscreen(app->window, NULL);
                    *fullscreen_bool=false;
                    free(start_game_background);
                    free(start_game_button);
                    free(high_score_background);
                    free(high_score_button);
                    free(settings_background);
                    free(settings_button);
                    free(exit_button);
                    
                    return MAIN_MENU;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);


        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, start_game_background, fullscreen_bool);
        render_button(app, start_game_button, fullscreen_bool);
        render_button(app, high_score_background, fullscreen_bool);
        render_button(app, high_score_button, fullscreen_bool);
        render_button(app, settings_background, fullscreen_bool);
        render_button(app, settings_button, fullscreen_bool);
        render_button(app, exit_button, fullscreen_bool);

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
}

int select_game_menu(App* app, bool *fullscreen_bool)
{

    int Mx, My;
    char input_ip[13], input_port[5];

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* single_player_background = menu_button_background(app, 300, 400, 360, 150, "./resources/menuButton.png", fullscreen_bool);
    Button* single_player_button = menu_button_text(app, 337, 430, 290, 90, "Single Player", font, green, fullscreen_bool);
    Button* host_multiplayer_background = menu_button_background(app, 300, 550, 360, 150, "./resources/menuButton.png", fullscreen_bool);
    Button* host_multiplayer_button = menu_button_text(app, 337, 580, 290, 90, "Host Multiplayer", font, green, fullscreen_bool);
    Button* join_multiplayer_background = menu_button_background(app, 300, 700, 360, 150, "./resources/menuButton.png", fullscreen_bool);
    Button* join_multiplayer_button = menu_button_text(app, 337, 730, 290, 90, "Join Multiplayer", font, green, fullscreen_bool);
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
                if (hover_state(single_player_button, Mx, My)) {
                    // Makes space on the heap
                    free(single_player_background);
                    free(single_player_button);
                    free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    free(join_multiplayer_background);
                    free(join_multiplayer_button);
                    return START_GAME;

                } else if (hover_state(join_multiplayer_button, Mx, My)) {
                    // Makes space on the heap
                    free(single_player_background);
                    free(single_player_button);
                    free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    free(join_multiplayer_background);
                    free(join_multiplayer_button);
                    return JOIN_MULTIPLAYER;

                } else if (hover_state(host_multiplayer_button, Mx, My)) {
                    // Makes space on the heap
                    free(single_player_background);
                    free(single_player_button);
                    free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    free(join_multiplayer_background);
                    free(join_multiplayer_button);
                    return HOST_MULTIPLAYER;

                } else if (hover_state(return_button, Mx, My)) {
                    // Makes space on the heap
                    free(single_player_background);
                    free(single_player_button);
                    free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    free(join_multiplayer_background);
                    free(join_multiplayer_button);
                    return MAIN_MENU;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, single_player_background, fullscreen_bool);
        render_button(app, single_player_button, fullscreen_bool);
        render_button(app, host_multiplayer_background, fullscreen_bool);
        render_button(app, host_multiplayer_button, fullscreen_bool);
        render_button(app, join_multiplayer_background, fullscreen_bool);
        render_button(app, join_multiplayer_button, fullscreen_bool);
        render_button(app, return_button, fullscreen_bool);

        //If-state for wether the text should switch color on hover or not
        if (hover_state(single_player_button, Mx, My)) {
            SDL_SetTextureColorMod(single_player_button->texture, 9, 34, 3);

        } else if (hover_state(host_multiplayer_button, Mx, My)) {
            SDL_SetTextureColorMod(host_multiplayer_button->texture, 9, 34, 3);

        } else if (hover_state(join_multiplayer_button, Mx, My)) {
            SDL_SetTextureColorMod(join_multiplayer_button->texture, 9, 34, 3);

        } else if (hover_state(return_button, Mx, My)) {
            SDL_SetTextureColorMod(return_button->texture, 127, 127, 127);
        } else {
            SDL_SetTextureColorMod(single_player_button->texture, 45, 93, 9);
            SDL_SetTextureColorMod(host_multiplayer_button->texture, 45, 93, 9);
            SDL_SetTextureColorMod(join_multiplayer_button->texture, 45, 93, 9);
            SDL_SetTextureColorMod(return_button->texture, 255, 255, 255);
        }

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }
    
}

int join_multiplayer(App* app, char* ip_address, char* port_nr, bool *fullscreen_bool)
{

    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, &"./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* enter_ip_background = menu_button_background(app, 230, 250, 500, 180, "./resources/ip_field.png", fullscreen_bool);
    Button* enter_port_background = menu_button_background(app, 300, 390, 360, 150, "./resources/port_field.png", fullscreen_bool);
    Button* join_background = menu_button_background(app, 300, 600, 360, 150, "./resources/menuButton.png", fullscreen_bool);
    Button* enter_ip = menu_button_text(app, 280, 290, 410, 110, "Enter IP", font, white, fullscreen_bool);
    Button* enter_port = menu_button_text(app, 350, 420, 250, 90, "Enter Port", font, white, fullscreen_bool);
    Button* join_button = menu_button_text(app, 337, 630, 290, 90, "Join", font, white, fullscreen_bool);
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
                if (hover_state(enter_ip_background, Mx, My)) {
                    // Gets input from user
                    port_ip_input(app, ip_address, 280, 290, 410, 110, false, fullscreen_bool);
                    strcpy(ip_address, ""); // !!!!!!!!!!!!!!!!!!!WARNING REMOVE LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                } else if (hover_state(enter_port_background, Mx, My)) {
                    // Gets input from user
                    port_ip_input(app, port_nr, 350, 420, 250, 90, true, fullscreen_bool);
                    strcpy(port_nr, ""); // !!!!!!!!!!!!!!!!!!!WARNING REMOVE LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                } else if (hover_state(return_button, Mx, My)) {
                    // Makes space on the heap
                    free(enter_ip_background);
                    free(enter_port_background);
                    free(join_background);
                    free(enter_ip);
                    free(enter_port);
                    free(join_button);
                    free(return_button);
                    return SELECT_GAME;
                } else if (hover_state(join_background, Mx, My)) {
                    // Makes space on the heap
                    free(enter_ip_background);
                    free(enter_port_background);
                    free(join_background);
                    free(enter_ip);
                    free(enter_port);
                    free(join_button);
                    free(return_button);
                    return START_GAME;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        if (fullscreen_bool) {
            optimizeFullscreen(&enter_ip_background->rect);
            optimizeFullscreen(&enter_port_background->rect);
            optimizeFullscreen(&join_background->rect);
            optimizeFullscreen(&enter_ip->rect);
            optimizeFullscreen(&enter_port->rect);
            optimizeFullscreen(&join_button->rect);
            optimizeFullscreen(&return_button->rect);
        } else if (!fullscreen_bool){
            enter_ip_background = menu_button_background(app, 230, 250, 500, 180, "./resources/ip_field.png", fullscreen_bool);
            enter_port_background = menu_button_background(app, 300, 390, 360, 150, "./resources/port_field.png", fullscreen_bool);
            join_background = menu_button_background(app, 300, 600, 360, 150, "./resources/menuButton.png", fullscreen_bool);
            enter_ip = menu_button_text(app, 280, 290, 410, 110, "Enter IP", font, white, fullscreen_bool);
            enter_port = menu_button_text(app, 350, 420, 250, 90, "Enter Port", font, white, fullscreen_bool);
            join_button = menu_button_text(app, 337, 630, 290, 90, "Join", font, green, fullscreen_bool);
            return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, white, fullscreen_bool);
        }
        render_button(app, enter_ip_background, fullscreen_bool);
        render_button(app, enter_port_background, fullscreen_bool);
        render_button(app, join_background, fullscreen_bool);

        render_button(app, enter_ip, fullscreen_bool);
        render_button(app, enter_port, fullscreen_bool);

        render_button(app, join_button, fullscreen_bool);
        render_button(app, return_button, fullscreen_bool);

        //If-state for wether the text should switch color on hover or not
        if (hover_state(join_button, Mx, My)) {
            SDL_SetTextureColorMod(join_button->texture, 9, 34, 3);

        } else if (hover_state(return_button, Mx, My)) {
            SDL_SetTextureColorMod(return_button->texture, 127, 127, 127);

        } else if (hover_state(enter_ip, Mx, My)) {
            SDL_SetTextureColorMod(enter_ip->texture, 127, 127, 127);

        } else if (hover_state(enter_port, Mx, My)) {
            SDL_SetTextureColorMod(enter_port->texture, 127, 127, 127);
        } else {
            SDL_SetTextureColorMod(join_button->texture, 45, 93, 9);
            SDL_SetTextureColorMod(return_button->texture, 255, 255, 255);
            SDL_SetTextureColorMod(enter_port->texture, 255, 255, 255);
            SDL_SetTextureColorMod(enter_ip->texture, 255, 255, 255);
        }

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }
    
}

int host_multiplayer(App* app, bool *fullscreen_bool)
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

int high_score(App* app, bool *fullscreen_bool)
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

int settings(App* app, bool *fullscreen_bool)
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