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

void menu_init(App *app, SDL_Texture *background, SDL_Texture *newGame, SDL_Texture *exit)
{
    //Let's make the menu's background
    
    load_texture(app, &background, "./resources/background.png");
    //Now the Background's rect, it should give us power to control it's size and location
    SDL_Rect *background_view = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    //Now, our background should start from the top left since it's coordinates are in 
    //(0, 0) and it should stretch 400 to the right and 400 down making it a square, 400X400
    
    //The New Game button
    
    load_texture(app, &newGame, "./resources/new_game_Button.png"); 
    
    //The game button's rect
    SDL_Rect newGame_Rect;
    newGame_Rect.x = 0;
    newGame_Rect.y = 100;
    newGame_Rect.w = 50;
    newGame_Rect.h = 25;
    
    //I'll just skip to the exit button

    load_texture(app, &exit, "./resources/button_Exit.png");
    
    //The exit button's rect
    SDL_Rect exit_Rect;
    exit_Rect.x = 0;
    exit_Rect.y = 200;
    exit_Rect.w = 50;
    exit_Rect.h = 25;
    

    /*
    {
        SDL_SetTextureColorMod(newGame, 250, 0, 0);
        This would make the texture change color if the mouse is inside the rect,
    it takes the name of the texture which is newGame, and the following variables
    are the r, g, and b, if you were to make all except red into 0 then the texture
    would be tinted red, I assumed you wanted the player to know that it is hovering
    over the new game so I added this as a bonus

        if (event->type == SDL_MOUSEBUTTONDOWN) //this calls an event, I assume that you already know how to make an event right?
        {
            if (event->button.button == SDL_BUTTON_LEFT)
            { //if it is pressed then play1 becomes true which you could use to initiate the newgame
                play1 = true;
            }
        }
    }
    else
    {
        SDL_SetTextureColorMod(Gettexture(), 250, 250, 250);
        // Tints the texture white if the cursor isn't over the button
    }
    */
}

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

void text_input (App *app, int x, int y, int w, int h, char ip_adress[]) {

    int done = 0;

    SDL_StartTextInput();

    SDL_Rect tmp;
    tmp.x = x;
    tmp.y = y;
    tmp.w = w;
    tmp.h = h;

    SDL_SetTextInputRect(&tmp);
    SDL_RenderDrawRect(app, &tmp);
    SDL_SetRenderDrawColor(app, 0, 0, 0, 255); // the rect color
    SDL_RenderFillRect(app, &tmp);
    SDL_RenderPresent(app); // copy to screen

    while (!done) {
        SDL_Event event;

        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    done = 1;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new text onto the end of our text */
                    strcat(ip_adress, event.text.text);
                    break;
                case SDL_SCANCODE_KP_ENTER:
                    done = 1;
                    break;
            }
        }
        SDL_RenderDrawRect(app, &tmp);
        SDL_RenderPresent(app);
    }

    SDL_StopTextInput();

    printf("\nthe text input is: %s\n\n",ip_adress);
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

int join_multiplayer (App *app, char *ip_adress) {

    int Mx, My;

    SDL_Texture *background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    TTF_Font *font = TTF_OpenFont("./resources/adventure.otf", 250);
    
    Button *enter_port_background = menu_button_background (app, 300, 390, 360, 150, "./resources/port_field.png");
    //SDL_Rect port_input = {337, 420, 290, 90};
    
    Button *enter_ip_background = menu_button_background (app, 230, 250, 500, 180, "./resources/ip_field.png");    
    Button *join_background = menu_button_background (app, 300, 600, 360, 150, "./resources/menuButton.png");
    Button *join_button = menu_button_text(app, 337, 630, 290, 90, "Join", font, color_select(GREEN));
    Button *return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, color_select(WHITE));
    
    text_input(app, 327, 120, 310, 90, ip_adress);
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
                if (hover_state(join_button, Mx, My)) {
                    return START_GAME;
                } else if (hover_state(port_input, Mx, My)) {
                    

                } else if (hover_state(return_button, Mx, My)) {
                    return SELECT_GAME;
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
        render_button(app, join_button);
        render_button(app, return_button);

        //If-state for wether the text should switch color on hover or not
        if (hover_state(join_button, Mx, My)) {
            SDL_SetTextureColorMod(join_button->texture, 9, 34, 3);
            
        } else if (hover_state(return_button, Mx, My)) {
            SDL_SetTextureColorMod(return_button->texture, 127, 127, 127);

        } else {
            SDL_SetTextureColorMod(join_button->texture, 45, 93, 9);
            SDL_SetTextureColorMod(return_button->texture, 255, 255, 255);

        }

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }
}