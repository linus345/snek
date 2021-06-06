#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "app.h"
#include "player.h"
#include "sound.h"
#include "fruit.h"

#define CELL_SIZE 32
#define MAX_PLAYERS 4

//Menu button creation struct consisting of SDL_Rect, SDL_Texture and SDL_Color.
typedef struct Screen_item {
    SDL_Rect rect;
    SDL_Texture *texture;
    /* SDL_Color color; */
} Screen_item;

typedef struct scoreboard {
    Screen_item* background;
    Screen_item* scoreboard;
    Screen_item* name[MAX_PLAYERS];
    Screen_item* score[MAX_PLAYERS];
    Screen_item* mute;
    Screen_item* return_button;
    Screen_item* continue_button;
} Scoreboard;

typedef struct {
    int nr_of_players;
    int client_id;
    unsigned current_time;
    unsigned last_time;
    bool connected;
    int nr_of_fruits;
    Fruit *fruits[MAX_PLAYERS];
    Player *players[MAX_PLAYERS];
    Scoreboard *scoreboard;
} Game_State;

//void main_loop(App* app);
Game_State *init_game_state();
void load_texture(App* app, SDL_Texture** texture, char* path);
void main_loop(App* app, Game_State *game_state);
int lobby(App* app, TTF_Font* font, Game_State* game_state, UDPsocket udp_sock);
void optimizeFullscreen(App* app, SDL_Rect* rect);
int game(App* app, TTF_Font* font, Game_State *game_state, UDPsocket udp_sock);
Scoreboard* create_scoreboard(App* app, TTF_Font* font, Player* players[]);
void free_scoreboard(Scoreboard* scoreboard);
void update_scoreboard(App* app, Player* players[], Scoreboard* scoreboard);

#endif
