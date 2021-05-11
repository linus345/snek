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

<<<<<<< HEAD
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
=======
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
>>>>>>> parent of 7b89b1b (Testing)
    }
}

SDL_Color

    black
    = { 0, 0, 0, 255 },
    gray = { 127, 127, 127, 255 },
    white = { 255, 255, 255, 255 },
    green = { 45, 93, 9, 255 },
    dark_green = { 9, 34, 3, 255 };

// Universal button renderer for menus
<<<<<<< HEAD
Button* menu_button_background(App* app, char resource[])
{
    Button* button = malloc(sizeof(Button));
    load_texture(app, &button->texture, resource);
=======
Button *menu_button_background (App *app, int x, int y, int w, int h, char *resource[]) {
    Button *button = malloc(sizeof(Button));
    load_texture(app, &button->texture, resource);
    button->rect.x=x;
    button->rect.y=y;
    button->rect.w=w;
    button->rect.h=h;
>>>>>>> parent of 7b89b1b (Testing)
    return button;
}

// Universal text renderer for buttons
<<<<<<< HEAD
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
=======
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

void render_button (App *app, Button *button) {
>>>>>>> parent of 7b89b1b (Testing)
    SDL_RenderCopy(app->renderer, button->texture, NULL, &button->rect);
}

bool hover_state (Button *button, int Mx, int My) {
    if (Mx >= button->rect.x && Mx <= button->rect.x + button->rect.w && My >= button->rect.y && My <= button->rect.y + button->rect.h) {
        return true;
    }
    return false;
}

// User input for ip address och port number
<<<<<<< HEAD
void port_ip_input(App* app, char input[], bool ip_not_port, bool* fullscreen_bool)
{
=======
void port_ip_input (App *app, char input[], int x, int y, int w, int h, bool ip_not_port) {
>>>>>>> parent of 7b89b1b (Testing)

    bool done = false;
    int Mx, My;

    SDL_Texture *background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

<<<<<<< HEAD
    Button* enter_ip_background = menu_button_background(app, "./resources/ip_field.png");
    Button* enter_port_background = menu_button_background(app, "./resources/port_field.png");
    Button* join_background = menu_button_background(app, "./resources/menuButton.png");

    Button* enter_ip = menu_button_text(app, "Enter IP", font, white);
    Button* enter_port = menu_button_text(app, "Enter Port", font, white);

    Button* join_button = menu_button_text(app, "Join", font, green);
    Button* return_button = menu_button_text(app, "Back", font, white);

    *enter_ip_background = (Button) { .rect.x = 230, .rect.y = 250, .rect.w = 500, .rect.h = 180 };
    *enter_port_background = (Button) { .rect.x = 300, .rect.y = 390, .rect.w = 360, .rect.h = 150 };
    *join_background = (Button) { .rect.x = 300, .rect.y = 600, .rect.w = 360, .rect.h = 150 };
    *enter_ip = (Button) { .rect.x = 280, .rect.y = 290, .rect.w = 410, .rect.h = 110 };
    *enter_port = (Button) { .rect.x = 350, .rect.y = 420, .rect.w = 250, .rect.h = 90 };
    *join_button = (Button) { .rect.x = 337, .rect.y = 630, .rect.w = 290, .rect.h = 90 };
    *return_button = (Button) { .rect.x = 380, .rect.y = 865, .rect.w = 200, .rect.h = 75 };

    Button* text = NULL;
=======
    TTF_Font *font = TTF_OpenFont("./resources/adventure.otf", 250);
 
    Button *enter_ip_background = menu_button_background (app, 230, 250, 500, 180, "./resources/ip_field.png");
    Button *enter_port_background = menu_button_background (app, 300, 390, 360, 150, "./resources/port_field.png"); 
    Button *join_background = menu_button_background (app, 300, 600, 360, 150, "./resources/menuButton.png");
        
    Button *enter_ip = menu_button_text(app, 280, 290, 410, 110, "Enter IP", font, color_select(WHITE));
    Button *enter_port = menu_button_text(app, 350, 420, 250, 90, "Enter Port", font, color_select(WHITE));  

    Button *join_button = menu_button_text(app, 337, 630, 290, 90, "Join", font, color_select(GREEN));
    Button *return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, color_select(WHITE));

    Button *text;

>>>>>>> parent of 7b89b1b (Testing)

    SDL_StartTextInput();
    SDL_Event event;
    while (!done) {

        printf("While loop\n");

        if (SDL_PollEvent(&event)) {
            printf("If-statement successful\n");
            switch (event.type) {
<<<<<<< HEAD
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
=======
                case SDL_QUIT:
                    app->running = false;
>>>>>>> parent of 7b89b1b (Testing)
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
        printf("Checkpoint 1\n");

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, enter_ip_background, fullscreen_bool);
        render_button(app, enter_port_background, fullscreen_bool);
        render_button(app, join_background, fullscreen_bool);

        // Makes sure the button pressed is not renderd
<<<<<<< HEAD
        if (ip_not_port) {
            render_button(app, enter_ip, fullscreen_bool);
        } else if (!ip_not_port) {
            render_button(app, enter_port, fullscreen_bool);
        }
        printf("Checkpoint 2\n");

        render_button(app, text, fullscreen_bool); // Renders the user input
        render_button(app, join_button, fullscreen_bool);
        render_button(app, return_button, fullscreen_bool);
=======
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
>>>>>>> parent of 7b89b1b (Testing)

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

    printf("\nthe text input is: %s\n\n",input);

    // Makes space on the heap
    free(enter_ip_background);
    free(enter_port_background);
    free(join_background);
    free(enter_ip);
    free(enter_port);
    free(join_button);
    free(return_button);
}

<<<<<<< HEAD
int main_menu(App* app, SDL_Rect* fullscreen, bool* fullscreen_bool)
{

=======
int main_menu (App *app, SDL_Rect *r) {
    
>>>>>>> parent of 7b89b1b (Testing)
    int Mx, My;

    SDL_Texture *background;
    load_texture(app, &background, "./resources/background.png");
<<<<<<< HEAD
    SDL_Rect background_view = { 0, 0, fullscreen->w, fullscreen->h };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* button1 = menu_button_background(app, "./resources/menuButton.png");
    Button* text1 = menu_button_text(app, "Start Game", font, green);
    Button* button2 = menu_button_background(app, "./resources/menuButton.png");
    Button* text2 = menu_button_text(app, "High Score", font, green);
    Button* button3 = menu_button_background(app, "./resources/menuButton.png");
    Button* text3 = menu_button_text(app, "Settings", font, green);
    Button* exit_button = menu_button_text(app, "Exit Game", font, white);
=======
    SDL_Rect background_view = {0, 0, r->w, r->h};

    TTF_Font *font = TTF_OpenFont("./resources/adventure.otf", 250);
    
    Button *start_game_background = menu_button_background (app, 300, 400, 360, 150, "./resources/menuButton.png");
    Button *start_game_button = menu_button_text(app, 337, 430, 290, 90, "Start Game", font, color_select(GREEN));
    Button *high_score_background = menu_button_background (app, 300, 550, 360, 150, "./resources/menuButton.png");
    Button *high_score_button = menu_button_text(app, 337, 580, 290, 90, "High Score", font, color_select(GREEN));
    Button *settings_background = menu_button_background (app, 300, 700, 360, 150, "./resources/menuButton.png");
    Button *settings_button = menu_button_text(app, 337, 730, 290, 90, "Settings", font, color_select(GREEN));
    Button *exit_button = menu_button_text(app, 380, 865, 200, 75, "Exit Game", font, color_select(WHITE));
>>>>>>> parent of 7b89b1b (Testing)

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
<<<<<<< HEAD
                    free(button1);
                    free(button2);
                    free(button3);
                    free(text1);
                    free(text2);
                    free(text3);
                    free(exit_button);
=======
                    free(start_game_background);
                    free(start_game_button);
                    free(high_score_background);
                    free(high_score_button);
>>>>>>> parent of 7b89b1b (Testing)
                    return SELECT_GAME;
                } else if (hover_state(text2, Mx, My)) {
                    // Makes space on the heap
<<<<<<< HEAD
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
=======
                    free(start_game_background);
                    free(start_game_button);
                    free(high_score_background);
                    free(high_score_button);
                    return HIGH_SCORE;
                } else if (hover_state(settings_button, Mx, My))
                {
                    // Makes space on the heap
                    free(start_game_background);
                    free(start_game_button);
                    free(high_score_background);
                    free(high_score_button);
>>>>>>> parent of 7b89b1b (Testing)
                    return SETTINGS;
                } else if (hover_state(exit_button, Mx, My)) {
                    app->running = false;
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_f:
                    SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
<<<<<<< HEAD
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
=======
                    break;
                case SDLK_ESCAPE:
                    SDL_SetWindowFullscreen(app->window, NULL);
                    break; 
>>>>>>> parent of 7b89b1b (Testing)
                }
                break;   
            }
        }
        // clear screen before next render
        SDL_RenderClear(app->renderer);

        button1->rect.x = 300;
        button1->rect.y = 400;
        button1->rect.w = 360;
        button1->rect.h = 150;
        text1->rect.x = 337;
        text1->rect.y = 430;
        text1->rect.w = 290;
        text1->rect.h = 90;

        button2->rect.x = 300;
        button2->rect.y = 550;
        button2->rect.w = 360;
        button2->rect.h = 150;
        text2->rect.x = 337;
        text2->rect.y = 57;
        text2->rect.w = 290;
        text2->rect.h = 90;

        button3->rect.x = 300;
        button3->rect.y = 700;
        button3->rect.w = 360;
        button3->rect.h = 150;
        text3->rect.x = 337;
        text3->rect.y = 730;
        text3->rect.w = 730;
        text3->rect.h = 90;

        exit_button->rect.x = 380;
        exit_button->rect.y = 865;
        exit_button->rect.w = 290;
        exit_button->rect.h = 90;

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
        render_button(app, button1, fullscreen_bool);
        render_button(app, text1, fullscreen_bool);
        render_button(app, button2, fullscreen_bool);
        render_button(app, text2, fullscreen_bool);
        render_button(app, button3, fullscreen_bool);
        render_button(app, text3, fullscreen_bool);
        render_button(app, exit_button, fullscreen_bool);

        //If-state for wether the text should switch color on hover or not
<<<<<<< HEAD
        if (hover_state(text1, Mx, My)) {
            SDL_SetTextureColorMod(text1->texture, 9, 34, 3);

        } else if (hover_state(text2, Mx, My)) {
            SDL_SetTextureColorMod(text2->texture, 9, 34, 3);
=======
        if (hover_state(start_game_button, Mx, My)) {
            SDL_SetTextureColorMod(start_game_button->texture, 9, 34, 3);
            
        } else if (hover_state(high_score_button, Mx, My)) {
            SDL_SetTextureColorMod(high_score_button->texture, 9, 34, 3);
>>>>>>> parent of 7b89b1b (Testing)

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

<<<<<<< HEAD
int select_game_menu(App* app, bool* fullscreen_bool)
{
=======
int select_game_menu (App *app) {
>>>>>>> parent of 7b89b1b (Testing)

    int Mx, My;

    SDL_Texture *background;
    load_texture(app, &background, "./resources/background.png");
<<<<<<< HEAD
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    //Button* single_player_background = menu_button_background(app, 300, 400, 360, 150, "./resources/menuButton.png", fullscreen_bool);
    Button* single_player_button = menu_button_text(app, "Single Player", font, green);
    //Button* host_multiplayer_background = menu_button_background(app, 300, 550, 360, 150, "./resources/menuButton.png", fullscreen_bool);
    Button* host_multiplayer_button = menu_button_text(app, "Host Multiplayer", font, green);
    //Button* join_multiplayer_background = menu_button_background(app, 300, 700, 360, 150, "./resources/menuButton.png", fullscreen_bool);
    Button* join_multiplayer_button = menu_button_text(app, "Join Multiplayer", font, green);
    Button* return_button = menu_button_text(app, "Back", font, white);
=======
    SDL_Rect background_view = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    TTF_Font *font = TTF_OpenFont("./resources/adventure.otf", 250);
    
    Button *single_player_background = menu_button_background (app, 300, 400, 360, 150, "./resources/menuButton.png");
    Button *single_player_button = menu_button_text(app, 337, 430, 290, 90, "Single Player", font, color_select(GREEN));
    Button *host_multiplayer_background = menu_button_background (app, 300, 550, 360, 150, "./resources/menuButton.png");
    Button *host_multiplayer_button = menu_button_text(app, 337, 580, 290, 90, "Host Multiplayer", font, color_select(GREEN));
    Button *join_multiplayer_background = menu_button_background (app, 300, 700, 360, 150, "./resources/menuButton.png");
    Button *join_multiplayer_button = menu_button_text(app, 337, 730, 290, 90, "Join Multiplayer", font, color_select(GREEN));
    Button *return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, color_select(WHITE));
>>>>>>> parent of 7b89b1b (Testing)

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

<<<<<<< HEAD
                } else if (hover_state(join_multiplayer_button, Mx, My)) {
                    // Makes space on the heap
                    //free(single_player_background);
=======
                } else if (hover_state(join_multiplayer_button, Mx, My)) {   
                    // Makes space on the heap                 
                    free(single_player_background);
>>>>>>> parent of 7b89b1b (Testing)
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
                
                
                }else if (hover_state(return_button, Mx, My)) {
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

        *single_player_button = (Button) { .rect.x = 337, .rect.y = 430, .rect.w = 290, .rect.h = 90 };
        *host_multiplayer_button = (Button) { .rect.x = 337, .rect.y = 580, .rect.w = 290, .rect.h = 90 };
        *join_multiplayer_button = (Button) { .rect.x = 337, .rect.y = 730, .rect.w = 290, .rect.h = 90 };
        *return_button = (Button) { .rect.x = 380, .rect.y = 865, .rect.w = 200, .rect.h = 75 };

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

<<<<<<< HEAD
int join_multiplayer(App* app, char* ip_address, char* port_nr, bool* fullscreen_bool)
{
=======
int join_multiplayer (App *app, char *ip_address, char *port_nr) {
>>>>>>> parent of 7b89b1b (Testing)

    int Mx, My;

    SDL_Texture *background;
    load_texture(app, &background, "./resources/background.png");
<<<<<<< HEAD
    SDL_Rect background_view = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* enter_ip_background = menu_button_background(app, "./resources/ip_field.png");
    Button* enter_port_background = menu_button_background(app, "./resources/port_field.png");
    Button* join_background = menu_button_background(app, "./resources/menuButton.png");
    Button* enter_ip = menu_button_text(app, "Enter IP", font, white);
    Button* enter_port = menu_button_text(app, "Enter Port", font, white);
    Button* join_button = menu_button_text(app, "Join", font, white);
    Button* return_button = menu_button_text(app, "Back", font, white);

=======
    SDL_Rect background_view = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    TTF_Font *font = TTF_OpenFont("./resources/adventure.otf", 250);
    
    
    Button *enter_ip_background = menu_button_background (app, 230, 250, 500, 180, "./resources/ip_field.png");
    Button *enter_port_background = menu_button_background (app, 300, 390, 360, 150, "./resources/port_field.png"); 
    Button *join_background = menu_button_background (app, 300, 600, 360, 150, "./resources/menuButton.png");
    Button *enter_ip = menu_button_text(app, 280, 290, 410, 110, "Enter IP", font, color_select(WHITE));
    Button *enter_port = menu_button_text(app, 350, 420, 250, 90, "Enter Port", font, color_select(WHITE));
    Button *join_button = menu_button_text(app, 337, 630, 290, 90, "Join", font, color_select(GREEN));
    Button *return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, color_select(WHITE));
    
>>>>>>> parent of 7b89b1b (Testing)
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
<<<<<<< HEAD
                    port_ip_input(app, ip_address, false, fullscreen_bool);
                    strcpy(ip_address, ""); // !!!!!!!!!!!!!!!!!!!WARNING REMOVE LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                } else if (hover_state(enter_port_background, Mx, My)) {
                    // Gets input from user
                    port_ip_input(app, port_nr, true, fullscreen_bool);
                    strcpy(port_nr, ""); // !!!!!!!!!!!!!!!!!!!WARNING REMOVE LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                } else if (hover_state(return_button, Mx, My)) {
=======
                    port_ip_input(app, ip_address, 280, 290, 410, 110, false);
                    strcpy(ip_address,""); // !!!!!!!!!!!!!!!!!!!WARNING REMOVE LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                }
                else if (hover_state(enter_port_background, Mx, My)) {
                    // Gets input from user
                    port_ip_input(app, port_nr, 350, 420, 250, 90, true);
                    strcpy(port_nr,""); // !!!!!!!!!!!!!!!!!!!WARNING REMOVE LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                }
                else if (hover_state(return_button, Mx, My)) {
>>>>>>> parent of 7b89b1b (Testing)
                    // Makes space on the heap
                    free(enter_ip_background);
                    free(enter_port_background);
                    free(join_background);
                    free(enter_ip);
                    free(enter_port);
                    free(join_button);
                    free(return_button);
                    return SELECT_GAME;
                }
                else if (hover_state(join_background, Mx, My)) {
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

        *enter_ip_background = (Button) { .rect.x = 230, .rect.y = 250, .rect.w = 500, .rect.h = 180 };
        *enter_port_background = (Button) { .rect.x = 300, .rect.y = 390, .rect.w = 360, .rect.h = 150 };
        *join_background = (Button) { .rect.x = 300, .rect.y = 600, .rect.w = 360, .rect.h = 150 };
        *enter_ip = (Button) { .rect.x = 280, .rect.y = 290, .rect.w = 410, .rect.h = 110 };
        *enter_port = (Button) { .rect.x = 350, .rect.y = 420, .rect.w = 250, .rect.h = 90 };
        *join_button = (Button) { .rect.x = 337, .rect.y = 630, .rect.w = 290, .rect.h = 90 };
        *return_button = (Button) { .rect.x = 380, .rect.y = 865, .rect.w = 200, .rect.h = 75 };

        SDL_RenderCopy(app->renderer, background, NULL, &background_view);
<<<<<<< HEAD
        render_button(app, enter_ip_background, fullscreen_bool);
        render_button(app, enter_port_background, fullscreen_bool);
        render_button(app, join_background, fullscreen_bool);

        render_button(app, enter_ip, fullscreen_bool);
        render_button(app, enter_port, fullscreen_bool);
=======
        render_button(app, enter_ip_background);
        render_button(app, enter_port_background);
        render_button(app, join_background);
        
        render_button(app, enter_ip);
        render_button(app, enter_port); 
>>>>>>> parent of 7b89b1b (Testing)

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
<<<<<<< HEAD
/*
int host_multiplayer(App* app, bool* fullscreen_bool)
{
=======

int host_multiplayer (App *app) {
>>>>>>> parent of 7b89b1b (Testing)

    int Mx, My;

    SDL_Texture *background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

<<<<<<< HEAD
    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, white, fullscreen_bool);
=======
    TTF_Font *font = TTF_OpenFont("./resources/adventure.otf", 250);
    
>>>>>>> parent of 7b89b1b (Testing)

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

<<<<<<< HEAD
int high_score(App* app, bool* fullscreen_bool)
{
=======
int high_score (App *app) {
>>>>>>> parent of 7b89b1b (Testing)

    int Mx, My;

    SDL_Texture *background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

<<<<<<< HEAD
    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, white, fullscreen_bool);
=======
    TTF_Font *font = TTF_OpenFont("./resources/adventure.otf", 250);
    
>>>>>>> parent of 7b89b1b (Testing)

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

<<<<<<< HEAD
int settings(App* app, bool* fullscreen_bool)
{
=======
int settings (App *app) {
>>>>>>> parent of 7b89b1b (Testing)

    int Mx, My;

    SDL_Texture *background;
    load_texture(app, &background, "./resources/background.png");
    SDL_Rect background_view = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

<<<<<<< HEAD
    TTF_Font* font = TTF_OpenFont("./resources/adventure.otf", 250);

    Button* return_button = menu_button_text(app, 380, 865, 200, 75, "Back", font, white, fullscreen_bool);
=======
    TTF_Font *font = TTF_OpenFont("./resources/adventure.otf", 250);
    
>>>>>>> parent of 7b89b1b (Testing)

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