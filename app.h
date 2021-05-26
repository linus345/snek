#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "network.h"
#include "sound.h"

#define WINDOW_WIDTH 1210
#define WINDOW_HEIGHT 960

#define GAME_WIDTH 960
#define GAME_HEIGHT 960

typedef struct {
    int x;
    int y;
} Pos;

typedef struct App {
    bool running, fullscreen;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_DisplayMode display;
    char ip[16], port[5], player_name[16];
    Sound_effects* sound;
    UDPsocket udp_sock;
    UDPpacket *pack_send;
    UDPpacket *pack_recv;
    IPaddress server_addr;
} App;

App *init_app();
void quit_app(App *app);

#endif
