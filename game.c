#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "app.h"
#include "game.h"

Snake *new_snake(int player_nr)
{
    // allocate memory on heap
    Snake *snake = malloc(sizeof(Snake));

    // snake initially has 1 body part + head and tail
    snake->body_length = 1;
    // initialize start speed
    snake->speed = SPEED;

    // position snake differently depending on player
    switch (player_nr)
    {
    // position snake top left, moving to the right
    case 1:
        snake->vel_x = snake->speed;
        snake->vel_y = 0;
        snake->head.x = 100;
        snake->head.y = CELL_SIZE;
        snake->dir = Right;
        snake->body[0].x = snake->head.x - CELL_SIZE;
        snake->body[0].y = snake->head.y;
        snake->tail.x = snake->head.x - CELL_SIZE * 2;
        snake->tail.y = snake->head.y;
        break;
    // position snake top right, moving to downwards
    case 2:
        snake->vel_x = 0;
        snake->vel_y = snake->speed;
        snake->head.x = WINDOW_WIDTH - 100;
        snake->head.y = CELL_SIZE * 3;
        snake->dir = Down;
        snake->body[0].x = snake->head.x;
        snake->body[0].y = snake->head.y - CELL_SIZE;
        snake->tail.x = snake->head.x;
        snake->tail.y = snake->head.y - CELL_SIZE * 2;
        break;
    // position snake bottom right, moving to the left
    case 3:
        snake->vel_x = -snake->speed;
        snake->vel_y = 0;
        snake->head.x = WINDOW_WIDTH - CELL_SIZE * 3;
        snake->head.y = WINDOW_HEIGHT - 100;
        snake->dir = Left;
        snake->body[0].x = snake->head.x + CELL_SIZE;
        snake->body[0].y = snake->head.y;
        snake->tail.x = snake->head.x + CELL_SIZE * 2;
        snake->tail.y = snake->head.y;
        break;
    // position snake bottom left, moving upwards
    case 4:
        snake->vel_x = 0;
        snake->vel_y = -snake->speed;
        snake->head.x = 100;
        snake->head.y = WINDOW_HEIGHT - CELL_SIZE * 3;
        snake->dir = Up;
        snake->body[0].x = snake->head.x;
        snake->body[0].y = snake->head.y + CELL_SIZE;
        snake->tail.x = snake->head.x;
        snake->tail.y = snake->head.y + CELL_SIZE * 2;
        break;
    }

    return snake;
}

Player *new_player(int host, int port, int player_nr)
{
    Player *player = malloc(sizeof(Player));

    player->points = 0;
    player->alive = true;
    player->host = host;
    player->port = port;
    player->snake = new_snake(player_nr);

    return player;
}

void GAME_OVER()
{
    TTF_Font *Sans = TTF_OpenFont("Sans.ttf", 24); //this opens a font style and sets a size

    SDL_Color White = {255, 255, 255}; // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Sans, "put your text here", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

    SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 0;    //controls the rect's x coordinate
    Message_rect.y = 0;    // controls the rect's y coordinte
    Message_rect.w = 100;  // controls the width of the rect
    Message_rect.h = 100;  // controls the height of the rect

    //Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understand

    //Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

    //Don't forget to free your surface and texture
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}
