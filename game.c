#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "app.h"
#include "fruit.h"
#include "game.h"
#include "menu.h"
#include "network.h"
#include "player.h"
#include "rendering.h"

extern bool thread_done;

Game_State* init_game_state()
{
    Game_State* game_state = malloc(sizeof(Game_State));

    // client_id and current_time doesn't need to be initialized
    game_state->nr_of_players = 0;
    game_state->connected = false;
    game_state->last_time = 0;
    game_state->nr_of_fruits = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        game_state->fruits[i] = NULL;
    }

    return game_state;
}

void load_texture(App* app, SDL_Texture** texture, char* path)
{
    printf("Loading texture: %s\n", path);
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        fprintf(stderr, "error creating surface\n");
        exit(EXIT_FAILURE);
    }

    *texture = SDL_CreateTextureFromSurface(app->renderer, surface);
    SDL_FreeSurface(surface);
    if (!*texture) {
        fprintf(stderr, "error creating texture\n");
        exit(EXIT_FAILURE);
    }
}

void main_loop(App* app, Game_State* game_state)
{
    int menu_state = MAIN_MENU;
    while (app->running) {
        switch (menu_state) {
        case MAIN_MENU:
            menu_state = main_menu(app);
            break;
        case SELECT_GAME:
            menu_state = select_game_menu(app);
            break;
        case JOIN_MULTIPLAYER:
            menu_state = join_multiplayer(app);
            break; /*
        case HOST_MULTIPLAYER:
            menu_state = host_multiplayer(app);
            break;
        case HIGH_SCORE:
            menu_state = high_score(app);
            break;
        case SETTINGS:
            menu_state = settings(app);
            break;*/
        case TYPE_NAME:
            menu_state = type_name(app);
            break;
        case START_GAME:
            menu_state = game(app, game_state);
            break;
        }
    }
}

int game(App* app, Game_State* game_state)
{
    int Mx, My;

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

    SDL_Texture* snake_sprite_tex;
    load_texture(app, &snake_sprite_tex, "./resources/Textures/snake-sprite.png");

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

    SDL_Texture* fruit_sprite_tex;
    load_texture(app, &fruit_sprite_tex, "./resources/Textures/fruit-sprite.png");

    // background texture
    SDL_Texture* background_tex;
    load_texture(app, &background_tex, "./resources/Textures/background.png");

    //////////// NETWORK ////////////

    /* // open socket */
    UDPsocket udp_sock = open_client_socket();

    /* // allocate memory for sent packet */
    UDPpacket* pack_send = allocate_packet(PACKET_DATA_SIZE);

    /* // allocate memory for received packet */
    UDPpacket* pack_recv = allocate_packet(PACKET_DATA_SIZE);

    // resolve server address TODO: bind address?
    int server_port = atoi(app->port);
    IPaddress server_addr = resolve_host(app->ip, server_port);

    // array to store info about connected players
    Player* players[MAX_PLAYERS] = { NULL, NULL, NULL, NULL };
    /* // keep track of number of joined clients */
    /* int nr_of_players = 0; */
    /* // client id for this specific client, needed to select correct player from the players array */
    /* int client_id; */

    // send join game request
    join_game_request(udp_sock, server_addr, pack_send);
    // wait for server to respond to request, only wait for 3 seconds
    unsigned time_when_req_sent = SDL_GetTicks();
    // used when client connecting and in main loop
    int request_type;

    Scoreboard* scoreboard = create_scoreboard(app, players);

    while (!game_state->connected) {
        if (SDLNet_UDP_Recv(udp_sock, pack_recv)) {
            // get request type from packet
            sscanf((char*)pack_recv->data, "%d", &request_type);
            switch (request_type) {
            // these are the only expected types
            case SUCCESSFUL_CONNECTION:
                // adds new player and increments nr_of_players
                new_client_joined(pack_recv->data, game_state, players);
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
        if (time_when_req_sent > time_when_req_sent + 3000) {
            printf("join: request timed out\n");
            // exit for now
            exit(EXIT_FAILURE);
            break;
        }
    }

    //////////////////////////////////

    // initialize thread safe buffer with enough space for 10 PACKET_DATA_SIZE byte packets
    Circular_Buffer* buf = init_buffer(10, PACKET_DATA_SIZE);

    // set global variable thread_done to false before creating thread
    thread_done = false;
    // arguments that thread needs to access
    Thread_Args args = { udp_sock, pack_recv, pack_send, buf };
    // create thread that listens for udp packets
    SDL_Thread* recv_thread = SDL_CreateThread(receive_packets_in_new_thread, "recv_thread", (void*)&args);
    // check if thread creation was successful or not
    if (recv_thread == NULL) {
        fprintf(stderr, "Error: SDL_CreateThread: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

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
                    if (players[game_state->client_id]->snake->dir != Down)
                        players[game_state->client_id]->snake->dir = Up;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    if (players[game_state->client_id]->snake->dir != Up)
                        players[game_state->client_id]->snake->dir = Down;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    if (players[game_state->client_id]->snake->dir != Left)
                        players[game_state->client_id]->snake->dir = Right;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    if (players[game_state->client_id]->snake->dir != Right)
                        players[game_state->client_id]->snake->dir = Left;
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (hover_state(scoreboard->return_button, Mx, My)) {
                    // Makes space on the heap
                    free_scoreboard(scoreboard);
                    return MAIN_MENU;
                } else if (hover_state(scoreboard->mute, Mx, My)) {
                    if (app->sound->muted) { // Unmutes all the sounds and changes the speaker icon
                        scoreboard->mute = menu_button_background(app, "./resources/Textures/speaker_icon.png");
                        app->sound->muted = false;
                    } else if (!app->sound->muted) { // Mutes all the sounds and changes the speaker icon
                        scoreboard->mute = menu_button_background(app, "./resources/Textures/speaker_icon_mute.png");
                        app->sound->muted = true;
                    }
                }
                break;
            }
        }

        // Check for recieved package
        /* handle_received_packets(net, game_state, players); */

        // handle every new packet before rendering this frame
        while (!buf_is_empty(buf)) {
            // handle packets
            void* data = malloc(PACKET_DATA_SIZE);
            read_from_buffer(buf, data);
            handle_received_packet(app, data, game_state, players);
            free(data);
        }

        // Checks if any collisons has occured with the walls
        if (collison_with_wall(players[game_state->client_id]->snake)) {
            players[game_state->client_id]->alive = false;
            send_collision(udp_sock, server_addr, pack_send, game_state->client_id);
            if (!app->sound->muted) {
                play_sound(app->sound->wall_collison); // plays wall collison sound effect
            }
            //show_scoreboard = true; // Remove once multiplayer has been implimented
        }
        // Checks if any collisons has occured with a snake
        if (collison_with_snake(players[game_state->client_id]->snake)) {
            players[game_state->client_id]->alive = false;
            send_collision(udp_sock, server_addr, pack_send, game_state->client_id);
            if (!app->sound->muted) {
                play_sound(app->sound->wall_collison); // plays wall collison sound effect
            }
            //show_scoreboard = true; // Remove once multiplayer has been implimented
        }

        if (players[game_state->client_id]->alive && game_state->current_time > game_state->last_time + players[game_state->client_id]->snake->speed) {
            // update snake velocity based on direction state
            change_snake_velocity(players[game_state->client_id]->snake);
            new_snake_pos(players[game_state->client_id]->snake, true);
            head_adjecent_with_fruit(&players[game_state->client_id]->snake->head, game_state->fruits, game_state->nr_of_fruits);
            send_snake_position(udp_sock, server_addr, pack_send, players[game_state->client_id]);
            game_state->last_time = game_state->current_time;
        }

        /* update_state_if_fruit_collision(players[game_state->client_id]->snake, game_state->fruits, &game_state->nr_of_fruits); */
        fruit_collision(app, udp_sock, server_addr, pack_send, players[game_state->client_id]->snake, game_state->fruits, &game_state->nr_of_fruits, game_state->client_id);

        // fruit rendering
        SDL_Rect fruit_src[MAX_PLAYERS] = { 0 };
        SDL_Rect fruit_dst[MAX_PLAYERS] = { 0 };

        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (game_state->fruits[i] == NULL) {
                // don't try to render a fruit that doesn't exist
                continue;
            }
            fruit_src[i].x = fruit_texture[game_state->fruits[i]->type].x;
            fruit_src[i].y = fruit_texture[game_state->fruits[i]->type].y;
            fruit_src[i].w = CELL_SIZE;
            fruit_src[i].h = CELL_SIZE;

            fruit_dst[i].x = game_state->fruits[i]->pos.x;
            fruit_dst[i].y = game_state->fruits[i]->pos.y;
            fruit_dst[i].w = CELL_SIZE;
            fruit_dst[i].h = CELL_SIZE;
        }

        // clear screen before next render
        SDL_RenderClear(app->renderer);
        SDL_Rect background_dst = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
        SDL_RenderCopy(app->renderer, background_tex, NULL, &background_dst);

        render_scoreboard(app, scoreboard);

        // render fruits
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (fruit_src[i].w != CELL_SIZE || fruit_dst[i].w != CELL_SIZE) {
                // skip rendering if there is no fruit
                continue;
            }
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
        for (int i = 0; i < game_state->nr_of_players; i++) {
            if (players[i] == NULL) {
                // skip current player if player doesn't exist
                continue;
            } else if (!players[i]->alive) {
                // skip current player if player is dead
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
                } else {
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
                    } else if (players[i]->snake->body[j].should_flip_horizontal) {
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

        SDL_GetMouseState(&Mx, &My);
        SDL_Delay(1000 / 60);
    }
    //if (show_scoreboard) {
    //    //play_sound(app->sound->scoreboard);
    //    app->running = true;
    //    return scoreboard(app, score);
    //} else {
    //    return MAIN_MENU;
    //}

    // indicate that receive thread should stop running
    thread_done = true;
    // wait for thread to cleanup and exit
    SDL_WaitThread(recv_thread, NULL);

    // free receive buffer from memory
    free_buffer(buf);

    // cleanup Network structure
    printf("freeing packets...\n");
    free_net(udp_sock, pack_recv, pack_send);
    printf("packets freed\n");

    return MAIN_MENU;
}
/*
int scoreboard(App* app, int score)
{
    //bool playsound = true;

    
    TTF_Font* font = TTF_OpenFont("./resources/Fonts/adventure.otf", 250);
    SDL_Color white_txt = { 255, 255, 255, 255 };

    Screen_item* scorescreen_background = menu_button_background(app, "./resources/Textures/Forest_green.jpg");

    Screen_item* goal_text = menu_button_text(app, "Goal:", font, white_txt);
    Screen_item* goal_nr = menu_button_text(app, "250", font, white_txt);

    Screen_item* scoreboard1 = menu_button_background(app, "./resources/Textures/menuButton.png");
    Screen_item* scoreboard2 = menu_button_background(app, "./resources/Textures/menuButton.png");
    Screen_item* scoreboard3 = menu_button_background(app, "./resources/Textures/menuButton.png");
    Screen_item* scoreboard4 = menu_button_background(app, "./resources/Textures/menuButton.png");

    Screen_item* player1_name = menu_button_text(app, app->player_name, font, white_txt);
    Screen_item* player2_name = menu_button_text(app, "Stoffe", font, white_txt);
    Screen_item* player3_name = menu_button_text(app, "Victor", font, white_txt);
    Screen_item* player4_name = menu_button_text(app, "Alma", font, white_txt);
    
    char buffer[10];
    sprintf(buffer, "%d", score);

    Screen_item* player1_score = menu_button_text(app, buffer, font, white_txt);
    Screen_item* player2_score = menu_button_text(app, " ", font, white_txt);
    Screen_item* player3_score = menu_button_text(app, " ", font, white_txt);
    Screen_item* player4_score = menu_button_text(app, " ", font, white_txt);
    
    Screen_item* continue_button = menu_button_text(app, "Press to continue", font, white_txt);

    int Mx, My;
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
                case SDLK_F11:
                    if (app->fullscreen) {
                        SDL_SetWindowFullscreen(app->window, 0);
                        app->fullscreen = false;
                    } else {
                        SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        app->fullscreen = true;
                    }
                    // Makes space on the heap
                    break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (hover_state(continue_button, Mx, My)) {
                    play_sound(app->sound->press);
                    // Makes space on the heap
                    //free(goal_text);
                    //free(goal_nr);
                    free(continue_button);
                    return MAIN_MENU;
                }
                break;
            }
        }

        // clear screen before next render
        SDL_RenderClear(app->renderer);

        render_item(app, &scorescreen_background->rect, scorescreen_background->texture, STRETCH, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        render_item(app, &goal_text->rect, goal_text->texture, UNSPECIFIED, WINDOW_WIDTH / 2 - 129, 100, 300, 120);

        render_item(app, &scoreboard1->rect, scoreboard1->texture, UNSPECIFIED, FSD_BUTTON_X, FSD_BUTTON_Y, FSD_BUTTON_W, FSD_BUTTON_H);
        render_item(app, &scoreboard2->rect, scoreboard2->texture, UNSPECIFIED, FSD_BUTTON_X, FSD_BUTTON_Y + FY_OFFSET, FSD_BUTTON_W, FSD_BUTTON_H);
        render_item(app, &scoreboard3->rect, scoreboard3->texture, UNSPECIFIED, FSD_BUTTON_X, FSD_BUTTON_Y + (2 * FY_OFFSET), FSD_BUTTON_W, FSD_BUTTON_H);
        render_item(app, &scoreboard4->rect, scoreboard4->texture, UNSPECIFIED, FSD_BUTTON_X, FSD_BUTTON_Y + (3 * FY_OFFSET), FSD_BUTTON_W, FSD_BUTTON_H);

        render_item(app, &player1_name->rect, player1_name->texture, UNSPECIFIED, F_NAME_X, F_NAME_Y, F_NAME_W, F_NAME_H);
        render_item(app, &player1_score->rect, player1_score->texture, UNSPECIFIED, F_SCORE_X, F_SCORE_Y, F_SCORE_W, F_SCORE_H);

        render_item(app, &player2_name->rect, player2_name->texture, UNSPECIFIED, F_NAME_X, F_NAME_Y + FY_OFFSET, F_NAME_W, F_NAME_H);
        render_item(app, &player3_name->rect, player3_name->texture, UNSPECIFIED, F_NAME_X, F_NAME_Y + (2 * FY_OFFSET), F_NAME_W, F_NAME_H);
        render_item(app, &player4_name->rect, player4_name->texture, UNSPECIFIED, F_NAME_X, F_NAME_Y + (3 * FY_OFFSET), F_NAME_W, F_NAME_H);

        render_item(app, &continue_button->rect, continue_button->texture, UNSPECIFIED, WINDOW_WIDTH / 2 - 129, WINDOW_HEIGHT - 145, 300, 120);

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }

    return MAIN_MENU;
}
*/