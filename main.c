#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include "app.h"
#include "game.h"
#include "player.h"
#include "snake.h"
#include "fruit.h"
#include "network.h"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error: SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    printf("successfully initialized SDL\n");

    if (SDLNet_Init() != 0) {
        fprintf(stderr, "Error: SDLNet_Init: %s\n", SDLNet_GetError());
        return 2;
    }
    printf("successfully initialized SDL_net\n");

    srand(time(NULL));

    App *app = init_app();

    Pos snake_texture[6];
    // head
    snake_texture[0].x = 0;
    snake_texture[0].y = 0;
    // body
    snake_texture[1].x = 32;
    snake_texture[1].y = 0;
    // tail
    snake_texture[2].x = 32;
    snake_texture[2].y = 32;
    // turning bodypart
    snake_texture[3].x = 0;
    snake_texture[3].y = 32;
    // mouth_open
    snake_texture[4].x = 64;
    snake_texture[4].y = 0;
    // mouth eating
    snake_texture[5].x = 64;
    snake_texture[5].y = 32;

    SDL_Texture *snake_sprite_tex;
    load_texture(app, &snake_sprite_tex, "./resources/snake-sprite.png");

    // intilaze fruits array
    Fruit *fruits[MAX_PLAYERS];
    int nr_of_fruits = 0;
    //bool start_up_fruit = true;

    Pos fruit_texture[4];
    //cherry
    fruit_texture[0].x = 0;
    fruit_texture[0].y = 0;
    //apple
    fruit_texture[1].x = 32;
    fruit_texture[1].y = 0;
    //pear
    fruit_texture[2].x = 0;
    fruit_texture[2].y = 32;
    //mango
    fruit_texture[3].x = 32;
    fruit_texture[3].y = 32;

    SDL_Texture *fruit_sprite_tex;
    load_texture(app, &fruit_sprite_tex, "./resources/fruit-sprite.png");

    // background texture
    SDL_Texture *background_tex;
    load_texture(app, &background_tex, "./resources/background.png");

    // timer
    unsigned last_time = 0, current_time;


    //////////// NETWORK ////////////

    // open socket
    UDPsocket udp_sock = open_client_socket();

    // allocate memory for sent packet
    UDPpacket *pack_send = allocate_packet(1024);

    // allocate memory for received packet
    UDPpacket *pack_recv = allocate_packet(1024);

    // resolve server address
    int server_port = 1234;
    IPaddress server_addr = resolve_host("127.0.0.1", server_port);

    // keep track of number of joined clients
    int nr_of_players = 0;
    // client id for this specific client, needed to select correct player from the players array
    int client_id;
    // array to store info about conneced players
    Player *players[MAX_PLAYERS] = {NULL, NULL, NULL, NULL};

    // send join game request
    join_game_request(udp_sock, server_addr, pack_send);
    // wait for server to respond to request, only wait for 3 seconds
    unsigned time_when_req_sent = SDL_GetTicks();
    // used when client connecting and in main loop
    int request_type;

    while(nr_of_players <= 0) {
        if(SDLNet_UDP_Recv(udp_sock, pack_recv)) {
            // get request type from packet
            sscanf(pack_recv->data, "%d", &request_type);
            switch(request_type) {
                // these are the only expected types
                case SUCCESSFUL_CONNECTION:
                    // adds new player and increments nr_of_players
                    new_client_joined(pack_recv, players, &nr_of_players, &client_id);
                    break;
                case FAILED_CONNECTION:
                    // TODO: handle it differently if it failed because 4 clients
                    // already is connected
                    printf("Failed to connect, retrying...\n");
                    // TODO: temporary exit until fixing above todo
                    exit(EXIT_FAILURE);
                    // update time for new request
                    time_when_req_sent = SDL_GetTicks();
                    break;
            }
        }
        // 3 second timer to break out of loop if packet not received yet
        // prevents from getting stuck in while loop
        if(time_when_req_sent > time_when_req_sent + 3000) {
            printf("join: request timed out\n");
            // exit for now
            exit(EXIT_FAILURE);
            break;
        }
    }

    //////////////////////////////////


    while (app->running) {
        SDL_Event event;
        // check for event
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    // exit main loop
                    app->running = false;
                    break;
                case SDL_KEYDOWN:
                    // key pressed?
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                        case SDLK_w:
                            if (players[client_id]->snake->dir != Down)
                                players[client_id]->snake->dir = Up;
                            break;
                        case SDLK_DOWN:
                        case SDLK_s:
                            if (players[client_id]->snake->dir != Up)
                                players[client_id]->snake->dir = Down;
                            break;
                        case SDLK_RIGHT:
                        case SDLK_d:
                            if (players[client_id]->snake->dir != Left)
                                players[client_id]->snake->dir = Right;
                            break;
                        case SDLK_LEFT:
                        case SDLK_a:
                            if (players[client_id]->snake->dir != Right)
                                players[client_id]->snake->dir = Left;
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }

        // Check for recieved package
        if(SDLNet_UDP_Recv(udp_sock, pack_recv)) {
            int temp, id, packet_nr, x, y, dir, angle;
            // get request type from packet
            sscanf(pack_recv->data, "%d", &request_type);
            switch(request_type) {
                case NEW_CLIENT_JOINED:
                    new_client_joined(pack_recv, players, &nr_of_players, &client_id);
                    break;
                case UPDATE_SNAKE_POS:
                    // TODO: create function for code below and find out why/if there is a delay/out of sync between clients
                    // format: type id last_received_packet_nr x y direction angle
                    sscanf(pack_recv->data, "%d %d %d %d %d %d %d", &temp, &id, &packet_nr, &x, &y, &dir, &angle);
                    printf("snake pos: id: %d, pack_nr: %d, x: %d, y: %d, dir: %d, angle: %d\n", id, packet_nr, x, y, dir, angle);
                    if(players[id]->last_received_packet_nr > packet_nr) {
                        // break early because we've already received a newer packet
                        break;
                    }
                    // update last received packet number to the packet we just got
                    players[id]->last_received_packet_nr = packet_nr;
                    // update snake position
                    players[id]->snake->dir = dir;
                    players[id]->snake->head.pos.x = x;
                    players[id]->snake->head.pos.y = y;
                    players[id]->snake->head.angle = angle;
                    break;
            }
        }

        // Checks if any collisons has occured with the walls
        if (collison_with_wall(players[client_id]->snake)) {
            /* app->running = false; */
            players[client_id]->alive = false;
        }
        // Checks if any collisons has occured with a snake
        if (collison_with_snake(players[client_id]->snake)) {
            /* app->running = false; */
            players[client_id]->alive = false;
        }
                
        current_time = SDL_GetTicks();
        if (current_time > last_time + players[client_id]->snake->speed) {
            /* change_snake_velocity(players[client_id]->snake); */

            // update snake velocity based on direction state
            // test singleplayer position update
            for(int i = 0; i < nr_of_players; i++) {
                if(players[i] == NULL && players[i]->alive) {
                    // skip current player if player doesn't exist or if player is dead
                    continue;
                }
                change_snake_velocity(players[i]->snake);
                new_snake_pos(players[i]->snake);
                head_adjecent_with_fruit(&players[i]->snake->head, fruits, nr_of_fruits);
            }
            /* new_snake_pos(players[client_id]->snake); */
            /* head_adjecent_with_fruit(&players[client_id]->snake->head, fruits, nr_of_fruits); */
            /* printf("client_id: %d\n", client_id); */
            /* printf("players[]->client_id: %d\n", players[client_id]->client_id); */
            send_snake_position(udp_sock, server_addr, pack_send, players[client_id]);

            last_time = current_time;
        }
        // temporarily disable fruit collision TODO
        /* if(fruit_collision(players[client_id]->snake, fruits, nr_of_fruits)) { */
        /*     free(fruits[nr_of_fruits-1]); */
        /*     nr_of_fruits--; */
        /*     new_snake_body_part(&players[client_id]->snake->body[players[client_id]->snake->body_length-1].pos, */ 
        /*         players[client_id]->snake->body[players[client_id]->snake->body_length-1].angle, */
        /*         &players[client_id]->snake->body_length); */
        /*     players[client_id]->snake->speed -= 50; */
        /* } */

        // fruit rendering
        SDL_Rect fruit_src[MAX_PLAYERS];
        SDL_Rect fruit_dst[MAX_PLAYERS];

        Fruit *temp_fruit = NULL;
        if(nr_of_fruits < nr_of_players) {
            // spawn new fruit
            while(temp_fruit == NULL) {
                temp_fruit = new_fruit(fruits, nr_of_fruits, players[client_id]->snake);
            }
            fruits[nr_of_fruits++] = temp_fruit;
        }
        for (int i = 0; i < nr_of_fruits; i++) {
            fruit_src[i].x = fruit_texture[fruits[i]->type].x;
            fruit_src[i].y = fruit_texture[fruits[i]->type].y;
            fruit_src[i].w = CELL_SIZE;
            fruit_src[i].h = CELL_SIZE;

            fruit_dst[i].x = fruits[i]->pos.x;
            fruit_dst[i].y = fruits[i]->pos.y;
            fruit_dst[i].w = CELL_SIZE;
            fruit_dst[i].h = CELL_SIZE;
        }

        // clear screen before next render
        SDL_RenderClear(app->renderer);
        SDL_Rect background_dst = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderCopy(app->renderer, background_tex, NULL, &background_dst);

        // render fruits
        for (int i = 0; i < nr_of_fruits; i++) {
            SDL_RenderCopyEx(app->renderer, fruit_sprite_tex, &fruit_src[i], &fruit_dst[i], 0, NULL, SDL_FLIP_NONE);
        }
        // render all snakes
        SDL_Rect head_src;
        SDL_Rect head_dst;
        SDL_Rect body_src[MAX_SNAKE_LENGTH];
        SDL_Rect body_dst[MAX_SNAKE_LENGTH];
        SDL_Rect tail_src;
        SDL_Rect tail_dst;
        SDL_RendererFlip flip;
        int rotation;
        for(int i = 0; i < nr_of_players; i++) {
            if(players[i] == NULL && players[i]->alive) {
                // skip current player if player doesn't exist
                continue;
            }
            // snake head rendering
            head_dst.x = players[i]->snake->head.pos.x;
            head_dst.y = players[i]->snake->head.pos.y;
            head_dst.w = CELL_SIZE;
            head_dst.h = CELL_SIZE;

            if (players[i]->snake->head.mouth_open) {
                // Snake open mouth next to fruit
                head_src.x = snake_texture[4].x;
                head_src.y = snake_texture[4].y;
            } else if (players[i]->snake->head.mouth_eating) {
                // Snake eating after open mouth
                head_src.x = snake_texture[5].x;
                head_src.y = snake_texture[5].y;
            } else {
                // Snake closed mouth, default
                head_src.x = snake_texture[0].x;
                head_src.y = snake_texture[0].y;
            }
            head_src.w = CELL_SIZE;
            head_src.h = CELL_SIZE;

            // snake body rendering
            for (int j = 0; j < players[i]->snake->body_length; j++) {
                if (players[i]->snake->body[j].is_turn) {
                    body_src[j].x = snake_texture[3].x;
                    body_src[j].y = snake_texture[3].y;
                }
                else {
                    body_src[j].x = snake_texture[1].x;
                    body_src[j].y = snake_texture[1].y;
                }
                body_src[j].w = CELL_SIZE;
                body_src[j].h = CELL_SIZE;

                body_dst[j].x = players[i]->snake->body[j].pos.x;
                body_dst[j].y = players[i]->snake->body[j].pos.y;
                body_dst[j].w = CELL_SIZE;
                body_dst[j].h = CELL_SIZE;
            }

            // snake tail rendering
            tail_src.x = snake_texture[2].x;
            tail_src.y = snake_texture[2].y;
            tail_src.w = CELL_SIZE;
            tail_src.h = CELL_SIZE;
            tail_dst.x = players[i]->snake->tail.pos.x;
            tail_dst.y = players[i]->snake->tail.pos.y;
            tail_dst.w = CELL_SIZE;
            tail_dst.h = CELL_SIZE;

            // clear screen before next render
            /* SDL_RenderClear(app->renderer); */

            /* SDL_Rect background_dst = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}; */
            /* SDL_RenderCopy(app->renderer, background_tex, NULL, &background_dst); */

            /* // render fruits */
            /* for (int i = 0; i < nr_of_fruits; i++) { */
            /*     SDL_RenderCopyEx(app->renderer, fruit_sprite_tex, &fruit_src[i], &fruit_dst[i], 0, NULL, SDL_FLIP_NONE); */
            /* } */

            // render head
            SDL_RenderCopyEx(app->renderer, snake_sprite_tex, &head_src, &head_dst, players[i]->snake->head.angle, NULL, SDL_FLIP_NONE);
            // render body
            flip = SDL_FLIP_NONE;
            for (int j = 0; j < players[i]->snake->body_length; j++) {
                rotation = players[i]->snake->body[j].angle;
                flip = SDL_FLIP_NONE;
                if (players[i]->snake->body[j].is_turn) {
                    rotation = players[i]->snake->body[j].turn_rotation;
                    if (players[i]->snake->body[j].should_flip_vertical) {
                        flip = SDL_FLIP_VERTICAL;
                    }
                    else if (players[i]->snake->body[j].should_flip_horizontal) {
                        flip = SDL_FLIP_HORIZONTAL;
                    }
                }
                SDL_RenderCopyEx(app->renderer, snake_sprite_tex, &body_src[j], &body_dst[j], rotation, NULL, flip);
            }
            // render tail
            SDL_RenderCopyEx(app->renderer, snake_sprite_tex, &tail_src, &tail_dst, players[i]->snake->tail.angle, NULL, SDL_FLIP_NONE);
        }

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
    }

    quit_app(app);
    
    return 0;
}
