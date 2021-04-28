#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

// Universal button renderer for menus
Button *init_menu_button (App *app, int x, int y, int w, int h, char *resource[]) {
    Button *button = malloc(sizeof(Button));
    load_texture(app, &button->texture, resource);
    button->rect.x=x;
    button->rect.y=y;
    button->rect.w=w;
    button->rect.h=h;
    return button;
}

// Universal text renderer for buttons
Button *create_button(App *app, int x, int y, int w, int h, char *text, TTF_Font *font, SDL_Color color) {
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
    SDL_RenderCopy(app->renderer, button->texture, NULL, &button->rect);
}