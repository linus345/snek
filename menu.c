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

void menu(App* app, char* ip_address, char* port_nr)
{
    bool fullscreen_bool = true;
    int menu_state = 0;
    SDL_Rect fullscreen;

    if (SDL_GetDisplayBounds(0, &fullscreen) != 0) {
        SDL_Log("SDL_GetDisplayBounds failed: %s", SDL_GetError());
        return;
    }

    while (app->running) {

        switch (menu_state) {
        case MAIN_MENU:
            menu_state = main_menu(app, &fullscreen, &fullscreen_bool);
            break;
        case SELECT_GAME:
            menu_state = select_game_menu(app, &fullscreen, &fullscreen_bool);
            break;
        case JOIN_MULTIPLAYER:
            menu_state = join_multiplayer(app, ip_address, port_nr, &fullscreen, &fullscreen_bool);
            break; 
        case HOST_MULTIPLAYER:
            menu_state = host_multiplayer(app, &fullscreen, &fullscreen_bool);
            break;
        case HIGH_SCORE:
            menu_state = high_score(app, &fullscreen, &fullscreen_bool);
            break;
        case SETTINGS:
            menu_state = settings(app, &fullscreen, &fullscreen_bool);
            break;
        case START_GAME:
            return;
        }
    }
}


SDL_Color color_select(int selection)
{
    SDL_Color
        black
        = { 0, 0, 0 },
        gray = { 127, 127, 127 },
        white = { 255, 255, 255 },
        green = { 45, 93, 9 },
        dark_green = { 9, 34, 3 };

    switch (selection) {
    case BLACK:
        return black;

    case GRAY:
        return gray;

    case WHITE:
        return white;

    case GREEN:
        return green;

    case DARK_GREEN:
        return dark_green;
    }
}

// Universal button renderer for menus
Button* menu_button_background(App* app, int x, int y, int w, int h, char* resource[])
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
Button* menu_button_text(App* app, int x, int y, int w, int h, char* text, TTF_Font* font, SDL_Color color)
{
    Button* button = malloc(sizeof(Button));
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    button->rect = rect;
    button->texture = texture;
    return button;
}

void render_button(App* app, Button* button)
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
void port_ip_input(App* app, char input[], int x, int y, int w, int h, bool ip_not_port)
{

    bool done = false;
    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* enter_ip_background = menu_button_background(app, 230, 250, 500, 180, "./resources/ip_field.png");
    Button* enter_port_background = menu_button_background(app, 300, 390, 360, 150, "./resources/port_field.png");
    Button* join_background = menu_button_background(app, 300, 600, 360, 150, "./resources/menuButton.png");

    Button* enter_ip = menu_button_text(app, 280, 290, 410, 110, "Enter IP", font, color_select(WHITE));
    Button* enter_port = menu_button_text(app, 350, 420, 250, 90, "Enter Port", font, color_select(WHITE));

    Button* join_button = menu_button_text(app, 337, 630, 290, 90, "Join", font, color_select(GREEN));
    Button* return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, color_select(WHITE));

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
                text = menu_button_text(app, x, y, w, h, input, font, color_select(WHITE));
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

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, enter_ip_background);
        render_button(app, enter_port_background);
        render_button(app, join_background);

        // Makes sure the button pressed is not renderd
        if (ip_not_port) {
            render_button(app, enter_ip);
        } else if (!ip_not_port) {
            render_button(app, enter_port);
        }
        printf("Checkpoint\n");
        if (text != NULL) {
            render_button(app, text);
        } // Renders the user input
        render_button(app, join_button);
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

int main_menu(App* app, SDL_Rect* r, bool* fullscreen)
{

    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, r->w, r->h };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* start_game_background = NULL;
    Button* start_game_button = NULL;
    Button* high_score_background = NULL;
    Button* high_score_button = NULL;
    Button* settings_background = NULL;
    Button* settings_button = NULL;
    Button* exit_button = NULL;

    if (*fullscreen) {
        background_view.h = r->h;
        background_view.w = r->w;
        start_game_background = menu_button_background(app, r->w / 2 - 150, 400, 360, 150, "./resources/menuButton.png");
        start_game_button = menu_button_text(app, r->w / 2 - 115, 430, 290, 90, "Start Game", font, color_select(GREEN));
        high_score_background = menu_button_background(app, r->w / 2 - 150, 550, 360, 150, "./resources/menuButton.png");
        high_score_button = menu_button_text(app, r->w / 2 - 115, 580, 290, 90, "High Score", font, color_select(GREEN));
        settings_background = menu_button_background(app, r->w / 2 - 150, 700, 360, 150, "./resources/menuButton.png");
        settings_button = menu_button_text(app, r->w / 2 - 115, 730, 290, 90, "Settings", font, color_select(GREEN));
        exit_button = menu_button_text(app, r->w / 2 - 70, 865, 200, 75, "Exit Game", font, color_select(WHITE));
    } else if (!*fullscreen) {
        background_view.h = WINDOW_HEIGHT;
        background_view.w = WINDOW_WIDTH;
        start_game_background = menu_button_background(app, 450, 400, 360, 150, "./resources/menuButton.png");
        start_game_button = menu_button_text(app, 485, 430, 290, 90, "Start Game", font, color_select(GREEN));
        high_score_background = menu_button_background(app, 450, 550, 360, 150, "./resources/menuButton.png");
        high_score_button = menu_button_text(app, 485, 580, 290, 90, "High Score", font, color_select(GREEN));
        settings_background = menu_button_background(app, 450, 700, 360, 150, "./resources/menuButton.png");
        settings_button = menu_button_text(app, 485, 730, 290, 90, "Settings", font, color_select(GREEN));
        exit_button = menu_button_text(app, 527, 865, 200, 75, "Exit Game", font, color_select(WHITE));
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
                    *fullscreen = true;
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
                    SDL_SetWindowFullscreen(app->window, 0);
                    *fullscreen = false;
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
        render_button(app, start_game_background);
        render_button(app, start_game_button);
        render_button(app, high_score_background);
        render_button(app, high_score_button);
        render_button(app, settings_background);
        render_button(app, settings_button);
        render_button(app, exit_button);

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

int select_game_menu(App* app, SDL_Rect* r, bool* fullscreen)
{
    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, r->w, r->h };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* single_player_background = NULL;
    Button* single_player_button = NULL;
    Button* host_multiplayer_background = NULL;
    Button* host_multiplayer_button = NULL;
    Button* join_multiplayer_background = NULL;
    Button* join_multiplayer_button = NULL;
    Button* return_button = NULL;

    if (*fullscreen) {
        background_view.h = r->h;
        background_view.w = r->w;
        single_player_background = menu_button_background(app, r->w / 2 - 150, 400, 360, 150, "./resources/menuButton.png");
        single_player_button = menu_button_text(app, r->w / 2 - 115, 430, 290, 90, "Single Player", font, color_select(GREEN));
        host_multiplayer_background = menu_button_background(app, r->w / 2 - 150, 550, 360, 150, "./resources/menuButton.png");
        host_multiplayer_button = menu_button_text(app, r->w / 2 - 115, 580, 290, 90, "Host Multiplayer", font, color_select(GREEN));
        join_multiplayer_background = menu_button_background(app, r->w / 2 - 150, 700, 360, 150, "./resources/menuButton.png");
        join_multiplayer_button = menu_button_text(app, r->w / 2 - 115, 730, 290, 90, "Join Multiplayer", font, color_select(GREEN));
        return_button = menu_button_text(app, r->w / 2 - 70, 865, 200, 75, "Back", font, color_select(WHITE));
    } else if (!*fullscreen) {
        background_view.h = WINDOW_HEIGHT;
        background_view.w = WINDOW_WIDTH;
        single_player_background = menu_button_background(app, 450, 400, 360, 150, "./resources/menuButton.png");
        single_player_button = menu_button_text(app, 485, 430, 290, 90, "Single Player", font, color_select(GREEN));
        host_multiplayer_background = menu_button_background(app, 450, 550, 360, 150, "./resources/menuButton.png");
        host_multiplayer_button = menu_button_text(app, 485, 580, 290, 90, "Host Multiplayer", font, color_select(GREEN));
        join_multiplayer_background = menu_button_background(app, 450, 700, 360, 150, "./resources/menuButton.png");
        join_multiplayer_button = menu_button_text(app, 485, 730, 290, 90, "Join Multiplayer", font, color_select(GREEN));
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
                if (hover_state(single_player_button, Mx, My)) {
                    // Makes space on the heap
                    free(single_player_background);
                    free(single_player_button);
                    free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    free(join_multiplayer_background);
                    free(join_multiplayer_button);
                    free(return_button);
                    return START_GAME;

                } else if (hover_state(join_multiplayer_button, Mx, My)) {
                    // Makes space on the heap
                    free(single_player_background);
                    free(single_player_button);
                    free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    free(join_multiplayer_background);
                    free(join_multiplayer_button);
                    free(return_button);
                    return JOIN_MULTIPLAYER;

                } else if (hover_state(host_multiplayer_button, Mx, My)) {
                    // Makes space on the heap
                    free(single_player_background);
                    free(single_player_button);
                    free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    free(join_multiplayer_background);
                    free(join_multiplayer_button);
                    free(return_button);
                    return HOST_MULTIPLAYER;

                } else if (hover_state(return_button, Mx, My)) {
                    // Makes space on the heap
                    free(single_player_background);
                    free(single_player_button);
                    free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    free(join_multiplayer_background);
                    free(join_multiplayer_button);
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

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, single_player_background);
        render_button(app, single_player_button);
        render_button(app, host_multiplayer_background);
        render_button(app, host_multiplayer_button);
        render_button(app, join_multiplayer_background);
        render_button(app, join_multiplayer_button);
        render_button(app, return_button);

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

int join_multiplayer(App* app, char* ip_address, char* port_nr, SDL_Rect* r, bool* fullscreen)
{
    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* enter_ip_background = NULL;
    Button* enter_port_background = NULL;
    Button* join_background = NULL;
    Button* enter_ip = NULL;
    Button* enter_port = NULL;
    Button* join_button = NULL;
    Button* return_button = NULL;

    if (*fullscreen) {
        background_view.h = r->h;
        background_view.w = r->w;
        enter_ip_background = menu_button_background(app, r->w / 2 - 225, 250, 500, 180, "./resources/ip_field.png");
        enter_port_background = menu_button_background(app, r->w / 2 - 150, 390, 360, 150, "./resources/port_field.png");
        join_background = menu_button_background(app, r->w / 2 - 150, 600, 360, 150, "./resources/menuButton.png");
        enter_ip = menu_button_text(app, r->w / 2 - 175, 290, 410, 110, "Enter IP", font, color_select(WHITE));
        enter_port = menu_button_text(app, r->w / 2 - 115, 420, 250, 90, "Enter Port", font, color_select(WHITE));
        join_button = menu_button_text(app, r->w / 2 - 115, 630, 290, 90, "Join", font, color_select(GREEN));
        return_button = menu_button_text(app, r->w / 2 - 70, 865, 200, 75, "Back", font, color_select(WHITE));
    } else if (!*fullscreen) {
        background_view.h = WINDOW_HEIGHT;
        background_view.w = WINDOW_WIDTH;
        enter_ip_background = menu_button_background(app, 330, 250, 500, 180, "./resources/ip_field.png");
        enter_port_background = menu_button_background(app, 450, 390, 360, 150, "./resources/port_field.png");
        join_background = menu_button_background(app, 450, 600, 360, 150, "./resources/menuButton.png");
        enter_ip = menu_button_text(app, 380, 290, 410, 110, "Enter IP", font, color_select(WHITE));
        enter_port = menu_button_text(app, 498, 420, 250, 90, "Enter Port", font, color_select(WHITE));
        join_button = menu_button_text(app, 485, 630, 290, 90, "Join", font, color_select(GREEN));
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
                if (hover_state(enter_ip_background, Mx, My)) {
                    // Gets input from user
                    port_ip_input(app, ip_address, 280, 290, 410, 110, false);
                    //port_ip_input(app, ip_address, 280, 290, 410, 110, false, r, fullscreen);
                    strcpy(ip_address, ""); // !!!!!!!!!!!!!!!!!!!WARNING REMOVE LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                } else if (hover_state(enter_port_background, Mx, My)) {
                    // Gets input from user
                    port_ip_input(app, port_nr, 350, 420, 250, 90, true);
                    //port_ip_input(app, port_nr, 350, 420, 250, 90, true, r, fullscreen);
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
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_f:
                    SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    *fullscreen = true;
                    // Makes space on the heap
                    free(enter_ip_background);
                    free(enter_port_background);
                    free(join_background);
                    free(enter_ip);
                    free(enter_port);
                    free(join_button);
                    free(return_button);
                    free(return_button);

                    return JOIN_MULTIPLAYER;
                case SDLK_ESCAPE:
                    // return main_menu sätt bool till false
                    SDL_SetWindowFullscreen(app->window, 0);
                    *fullscreen = false;
                    free(enter_ip_background);
                    free(enter_port_background);
                    free(join_background);
                    free(enter_ip);
                    free(enter_port);
                    free(join_button);
                    free(return_button);

                    return JOIN_MULTIPLAYER;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, enter_ip_background);
        render_button(app, enter_port_background);
        render_button(app, join_background);

        render_button(app, enter_ip);
        render_button(app, enter_port);

        render_button(app, join_button);
        render_button(app, return_button);

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

int host_multiplayer(App* app, SDL_Rect* r, bool* fullscreen)
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