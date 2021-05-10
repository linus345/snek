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
            menu_state = select_game_menu(app, &fullscreen_bool);
            break;
        case JOIN_MULTIPLAYER:
            menu_state = join_multiplayer(app, ip_address, port_nr, &fullscreen_bool);
            break;/*
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
}

SDL_Color

    black = { 0, 0, 0, 255},
    gray = { 127, 127, 127, 255},
    white = { 255, 255, 255, 255},
    green = { 45, 93, 9, 255},
    dark_green = { 9, 34, 3, 255};

// Universal button renderer for menus
Button* menu_button_background(App* app, char resource[])
{
    Button* button = malloc(sizeof(Button));
    load_texture(app, &button->texture, resource);
    return button;
}

// Universal text renderer for buttons
Button* menu_button_text(App* app, char* text, TTF_Font* font, SDL_Color color)
{
    Button* button = malloc(sizeof(Button));
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    button->texture = texture;
    return button;
}

void render_button(App* app, Button* button, bool* fullscreen_bool)
{
    if (fullscreen_bool) {
        optimizeFullscreen(&button->rect);
    }
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
void port_ip_input(App* app, char input[], bool ip_not_port, bool* fullscreen_bool)
{

    bool done = false;
    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* enter_ip_background = menu_button_background(app, "./resources/ip_field.png");
    Button* enter_port_background = menu_button_background(app, "./resources/port_field.png");
    Button* join_background = menu_button_background(app, "./resources/menuButton.png");

    Button* enter_ip = menu_button_text(app, "Enter IP", font, white);
    Button* enter_port = menu_button_text(app, "Enter Port", font, white);

    Button* join_button = menu_button_text(app, "Join", font, green);
    Button* return_button = menu_button_text(app, "Back", font, white);
    
        *enter_ip_background = (Button) { .rect.x =230, .rect.y =250, .rect.w =500, .rect.h = 180};
        *enter_port_background = (Button) { .rect.x =300, .rect.y =390, .rect.w =360, .rect.h = 150};
        *join_background = (Button) { .rect.x =300, .rect.y =600, .rect.w =360, .rect.h = 150};
        *enter_ip = (Button) { .rect.x =280, .rect.y =290, .rect.w =410, .rect.h = 110};
        *enter_port = (Button) { .rect.x =350, .rect.y =420, .rect.w =250, .rect.h = 90};
        *join_button = (Button) { .rect.x =337, .rect.y =630, .rect.w =290, .rect.h = 90};
        *return_button = (Button) { .rect.x =380, .rect.y =865, .rect.w =200, .rect.h = 75};

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
                if (hover_state(return_button, Mx, My)) {
                    done = true;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);
        printf("Checkpoint 1\n");

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
        printf("Checkpoint 2\n");

        render_button(app, text, fullscreen_bool); // Renders the user input
        render_button(app, join_button, fullscreen_bool);
        render_button(app, return_button, fullscreen_bool);

        //If-state for wether the text should switch color on hover or not
        printf("Checkpoint 3\n");
        if (hover_state(return_button, Mx, My)) {
            SDL_SetTextureColorMod(return_button->texture, 127, 127, 127);

        } else {
            SDL_SetTextureColorMod(return_button->texture, 255, 255, 255);
        }
        printf("Checkpoint 4\n");
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

    Button* button1 = menu_button_background(app, "./resources/menuButton.png");
    Button* text1 = menu_button_text(app, "Start Game", font, green);
    Button* button2 = menu_button_background(app, "./resources/menuButton.png");
    Button* text2 = menu_button_text(app, "High Score", font, green);
    Button* button3 = menu_button_background(app, "./resources/menuButton.png");
    Button* text3 = menu_button_text(app, "Settings", font, green);
    Button* exit_button = menu_button_text(app, "Exit Game", font, white);

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
                    *fullscreen_bool = true;
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
                    *fullscreen_bool = false;
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
               
        *button1 = (Button) {.rect.x = 300, .rect.y = 400, .rect.w = 360, .rect.h = 150};
        *text1 = (Button) { .rect.x = 337, .rect.y = 430, .rect.w = 290, .rect.h = 90};
        *button2 = (Button) { .rect.x = 300, .rect.y = 550, .rect.w = 360, .rect.h = 150};        
        *text2 = (Button) { .rect.x = 337, .rect.y = 570, .rect.w = 290, .rect.h = 90};
        *button3 = (Button) { .rect.x = 300, .rect.y = 700, .rect.w = 360, .rect.h = 150};
        *text3 = (Button) { .rect.x = 337, .rect.y = 730, .rect.w = 290, .rect.h = 90};
        *exit_button = (Button) { .rect.x = 380, .rect.y = 865, .rect.w = 290, .rect.h = 90};
        
        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, button1, fullscreen_bool);
        render_button(app, text1, fullscreen_bool);
        render_button(app, button2, fullscreen_bool);
        render_button(app, text2, fullscreen_bool);
        render_button(app, button3, fullscreen_bool);
        render_button(app, text3, fullscreen_bool);
        render_button(app, exit_button, fullscreen_bool);

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

int select_game_menu(App* app, bool* fullscreen_bool)
{

    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    //Button* single_player_background = menu_button_background(app, 300, 400, 360, 150, "./resources/menuButton.png", fullscreen_bool);
    Button* single_player_button = menu_button_text(app, "Single Player", font, green);
    //Button* host_multiplayer_background = menu_button_background(app, 300, 550, 360, 150, "./resources/menuButton.png", fullscreen_bool);
    Button* host_multiplayer_button = menu_button_text(app, "Host Multiplayer", font, green);
    //Button* join_multiplayer_background = menu_button_background(app, 300, 700, 360, 150, "./resources/menuButton.png", fullscreen_bool);
    Button* join_multiplayer_button = menu_button_text(app, "Join Multiplayer", font, green);
    Button* return_button = menu_button_text(app, "Back", font, white);

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
                    //free(single_player_background);
                    free(single_player_button);
                    //free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    //free(join_multiplayer_background);
                    free(join_multiplayer_button);
                    return START_GAME;

                } else if (hover_state(join_multiplayer_button, Mx, My)) {
                    // Makes space on the heap
                    //free(single_player_background);
                    free(single_player_button);
                    //free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    //free(join_multiplayer_background);
                    free(join_multiplayer_button);
                    return JOIN_MULTIPLAYER;

                } else if (hover_state(host_multiplayer_button, Mx, My)) {
                    // Makes space on the heap
                    //free(single_player_background);
                    free(single_player_button);
                    //free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    //free(join_multiplayer_background);
                    free(join_multiplayer_button);
                    return HOST_MULTIPLAYER;

                } else if (hover_state(return_button, Mx, My)) {
                    // Makes space on the heap
                    //free(single_player_background);
                    free(single_player_button);
                    //free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    //free(join_multiplayer_background);
                    free(join_multiplayer_button);
                    return MAIN_MENU;
                }
                break;
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        *single_player_button = (Button) { .rect.x =337, .rect.y =430, .rect.w =290, .rect.h = 90};
        *host_multiplayer_button = (Button) { .rect.x =337, .rect.y =580, .rect.w =290, .rect.h = 90};
        *join_multiplayer_button = (Button) { .rect.x =337, .rect.y =730, .rect.w =290, .rect.h = 90};
        *return_button = (Button) { .rect.x =380, .rect.y =865, .rect.w =200, .rect.h = 75};

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        //render_button(app, single_player_background, fullscreen_bool);
        render_button(app, single_player_button, fullscreen_bool);
        //render_button(app, host_multiplayer_background, fullscreen_bool);
        render_button(app, host_multiplayer_button, fullscreen_bool);
        //render_button(app, join_multiplayer_background, fullscreen_bool);
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
    return 0;
}

int join_multiplayer(App* app, char* ip_address, char* port_nr, bool* fullscreen_bool)
{

    int Mx, My;

    SDL_Texture* background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* enter_ip_background = menu_button_background(app, "./resources/ip_field.png");
    Button* enter_port_background = menu_button_background(app, "./resources/port_field.png");
    Button* join_background = menu_button_background(app, "./resources/menuButton.png");
    Button* enter_ip = menu_button_text(app, "Enter IP", font, white);
    Button* enter_port = menu_button_text(app, "Enter Port", font, white);
    Button* join_button = menu_button_text(app, "Join", font, white);
    Button* return_button = menu_button_text(app, "Back", font, white);

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
                    port_ip_input(app, ip_address, false, fullscreen_bool);
                    strcpy(ip_address, ""); // !!!!!!!!!!!!!!!!!!!WARNING REMOVE LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                } else if (hover_state(enter_port_background, Mx, My)) {
                    // Gets input from user
                    port_ip_input(app, port_nr, true, fullscreen_bool);
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

        *enter_ip_background = (Button) { .rect.x =230, .rect.y =250, .rect.w =500, .rect.h = 180};
        *enter_port_background = (Button) { .rect.x =300, .rect.y =390, .rect.w =360, .rect.h = 150};
        *join_background = (Button) { .rect.x =300, .rect.y =600, .rect.w =360, .rect.h = 150};
        *enter_ip = (Button) { .rect.x =280, .rect.y =290, .rect.w =410, .rect.h = 110};
        *enter_port = (Button) { .rect.x =350, .rect.y =420, .rect.w =250, .rect.h = 90};
        *join_button = (Button) { .rect.x =337, .rect.y =630, .rect.w =290, .rect.h = 90};
        *return_button = (Button) { .rect.x =380, .rect.y =865, .rect.w =200, .rect.h = 75};

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
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
} */