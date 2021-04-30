#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"
#include "menu.h"
#include "app.h"

SDL_Color color_select (int selection) {
    SDL_Color
    black = {0, 0, 0},
    gray = {127, 127, 127},
    white = {255, 255, 255},
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
Button *menu_button_background (App *app, int x, int y, int w, int h, char *resource[]) {
    Button *button = malloc(sizeof(Button));
    load_texture(app, &button->texture, resource);
    button->rect.x=x;
    button->rect.y=y;
    button->rect.w=w;
    button->rect.h=h;
    return button;
}

// Universal text renderer for buttons
Button *menu_button_text(App *app, int x, int y, int w, int h, char *text, TTF_Font *font, SDL_Color color) {
    Button *button = malloc(sizeof(Button));
    SDL_Surface * surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    rect.w=w;
    rect.h=h;
    button->rect=rect;
    button->texture=texture;
    return button;
}

/*
Button input_field() {

}
*/

void render_button (App *app, Button *button) {
    SDL_RenderCopy(app->renderer, button->texture, NULL, &button->rect);
}

bool hover_state (Button *button, int Mx, int My) {
    if (Mx >= button->rect.x && Mx <= button->rect.x + button->rect.w && My >= button->rect.y && My <= button->rect.y + button->rect.h) {
        return true;
    }
    return false;
}

void text_input (App *app, char input[], int x, int y, int w, int h, bool ip_not_port) {

    bool done = false;
    int Mx, My;

    SDL_Texture *background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    TTF_Font *font = TTF_OpenFont("./resources/adventure.otf", 250);
 
    Button *enter_ip_background = menu_button_background (app, 230, 250, 500, 180, "./resources/ip_field.png");
    Button *enter_port_background = menu_button_background (app, 300, 390, 360, 150, "./resources/port_field.png"); 
    Button *join_background = menu_button_background (app, 300, 600, 360, 150, "./resources/menuButton.png");
        
    Button *enter_ip = menu_button_text(app, 280, 290, 410, 110, "Enter IP", font, color_select(WHITE));
    Button *enter_port = menu_button_text(app, 350, 420, 250, 90, "Enter Port", font, color_select(WHITE));  

    Button *join_button = menu_button_text(app, 337, 630, 290, 90, "Join", font, color_select(GREEN));
    Button *return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, color_select(WHITE));

    Button *text;


    SDL_StartTextInput();
    while (!done) {
        SDL_Event event;

        if (SDL_PollEvent(&event)) {
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

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, enter_ip_background);
        render_button(app, enter_port_background);
        render_button(app, join_background);

        // Makes sure the button pressed is not renderd
        if (ip_not_port)
        {
            render_button(app, enter_ip);
        } else if (!ip_not_port)
        {
            render_button(app, enter_port);
        }
        
        render_button(app, text);   // Renders the user input
        render_button(app, join_button);
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

    SDL_StopTextInput();

    printf("\nthe text input is: %s\n\n",input);

    // Make space on the heap
    free(enter_ip_background);
    free(enter_port_background);
    free(join_background);
    free(enter_ip);
    free(enter_port);
    free(join_button);
    free(return_button);
}

int main_menu (App *app) {
    
    int Mx, My;

    SDL_Texture *background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    TTF_Font *font = TTF_OpenFont("./resources/adventure.otf", 250);
    
    Button *start_game_background = menu_button_background (app, 300, 400, 360, 150, "./resources/menuButton.png");
    Button *start_game_button = menu_button_text(app, 337, 430, 290, 90, "Start Game", font, color_select(GREEN));
    Button *high_score_background = menu_button_background (app, 300, 550, 360, 150, "./resources/menuButton.png");
    Button *high_score_button = menu_button_text(app, 337, 580, 290, 90, "High Score", font, color_select(GREEN));
    Button *settings_background = menu_button_background (app, 300, 700, 360, 150, "./resources/menuButton.png");
    Button *settings_button = menu_button_text(app, 337, 730, 290, 90, "Settings", font, color_select(GREEN));
    Button *exit_button = menu_button_text(app, 380, 865, 200, 75, "Exit Game", font, color_select(WHITE));

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
                    free(start_game_background);
                    free(start_game_button);
                    free(high_score_background);
                    free(high_score_button);
                    return SELECT_GAME;
                } else if (hover_state(exit_button, Mx, My)) {
                    app->running = false;

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

int select_game_menu (App *app) {

    int Mx, My;
    char input_ip[13], input_port[5];

    SDL_Texture *background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    TTF_Font *font = TTF_OpenFont("./resources/adventure.otf", 250);
    
    Button *single_player_background = menu_button_background (app, 300, 400, 360, 150, "./resources/menuButton.png");
    Button *single_player_button = menu_button_text(app, 337, 430, 290, 90, "Single Player", font, color_select(GREEN));
    Button *host_multiplayer_background = menu_button_background (app, 300, 550, 360, 150, "./resources/menuButton.png");
    Button *host_multiplayer_button = menu_button_text(app, 337, 580, 290, 90, "Host Multiplayer", font, color_select(GREEN));
    Button *join_multiplayer_background = menu_button_background (app, 300, 700, 360, 150, "./resources/menuButton.png");
    Button *join_multiplayer_button = menu_button_text(app, 337, 730, 290, 90, "Join Multiplayer", font, color_select(GREEN));
    Button *return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, color_select(WHITE));

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
                    free(single_player_background);
                    free(single_player_button);
                    free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    free(join_multiplayer_background);
                    free(join_multiplayer_button);
                    return START_GAME;

                } else if (hover_state(join_multiplayer_button, Mx, My)) {                    
                    free(single_player_background);
                    free(single_player_button);
                    free(host_multiplayer_background);
                    free(host_multiplayer_button);
                    free(join_multiplayer_background);
                    free(join_multiplayer_button);
                    return JOIN_MULTIPLAYER;

                } else if (hover_state(return_button, Mx, My)) {
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

int join_multiplayer (App *app, char *input) {

    int Mx, My;

    SDL_Texture *background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    TTF_Font *font = TTF_OpenFont("./resources/adventure.otf", 250);
    
    
    Button *enter_ip_background = menu_button_background (app, 230, 250, 500, 180, "./resources/ip_field.png");
    Button *enter_port_background = menu_button_background (app, 300, 390, 360, 150, "./resources/port_field.png"); 
    Button *join_background = menu_button_background (app, 300, 600, 360, 150, "./resources/menuButton.png");
    Button *enter_ip = menu_button_text(app, 280, 290, 410, 110, "Enter IP", font, color_select(WHITE));
    Button *enter_port = menu_button_text(app, 350, 420, 250, 90, "Enter Port", font, color_select(WHITE));
    Button *join_button = menu_button_text(app, 337, 630, 290, 90, "Join", font, color_select(GREEN));
    Button *return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, color_select(WHITE));
    
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
                    text_input(app, input, 280, 290, 410, 110, false);
                    strcpy(input,""); // !!!!!!!!!!!!!!!!!!!WARNING REMOVE LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                }
                else if (hover_state(enter_port_background, Mx, My)) {
                    // Gets input from user
                    text_input(app, input, 350, 420, 250, 90, true);
                    strcpy(input,""); // !!!!!!!!!!!!!!!!!!!WARNING REMOVE LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                }
                else if (hover_state(return_button, Mx, My)) {
                    return SELECT_GAME;
                }
                else if (hover_state(join_background, Mx, My)) {
                    return START_GAME;
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