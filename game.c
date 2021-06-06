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
extern SDL_Color black, gray, white, green, dark_green;

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
        game_state->players[i] = NULL;
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
    UDPsocket udp_sock = open_client_socket();
    TTF_Font* font = TTF_OpenFont("./resources/Fonts/adventure.otf", 250);
    int menu_state = MAIN_MENU;

    while (app->running) {
        switch (menu_state) {
        case MAIN_MENU:
            menu_state = main_menu(app, font);
            break;
        case SELECT_GAME:
            menu_state = select_game_menu(app, font);
            break;
        case JOIN_MULTIPLAYER:
            menu_state = join_multiplayer(app, font);
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
            menu_state = type_name(app, font);
            break;
        case START_GAME:
            menu_state = game(app, font, game_state, udp_sock);
            break;
        case LOBBY:
            menu_state = lobby(app, font, game_state, udp_sock);
            break;
        }
    }
    TTF_CloseFont(font);
}

int lobby(App* app, TTF_Font* font, Game_State* game_state, UDPsocket udp_sock)
{
    int Mx, My;
    enum Menu_selection next_menu_state = LOBBY;
    bool playsound = true;

    TTF_Font* small_font = TTF_OpenFont("./resources/Fonts/adventure.otf", 150);

    Pos snake_texture[3];
    // tail
    snake_texture[0].x = 32;
    snake_texture[0].y = 32;
    // body
    snake_texture[1].x = 32;
    snake_texture[1].y = 0;
    // head
    snake_texture[2].x = 0;
    snake_texture[2].y = 0;

    SDL_Rect snake_src, snake_dst;
    snake_src.w = CELL_SIZE;
    snake_src.h = CELL_SIZE;
    snake_dst.w = CELL_SIZE;
    snake_dst.h = CELL_SIZE;

    SDL_Texture* snake_sprite_textures[5];
    // green snake texture
    load_texture(app, &snake_sprite_textures[0], "./resources/Textures/snake-sprite-green.png");
    // blue snake texture
    load_texture(app, &snake_sprite_textures[1], "./resources/Textures/snake-sprite-blue.png");
    // purple snake texture
    load_texture(app, &snake_sprite_textures[2], "./resources/Textures/snake-sprite-purple.png");
    // yellow snake texture
    load_texture(app, &snake_sprite_textures[3], "./resources/Textures/snake-sprite-yellow.png");
    // orange snake texture
    load_texture(app, &snake_sprite_textures[4], "./resources/Textures/snake-sprite-orange.png");

    // arrow texture
    SDL_Texture* arrow_tex;
    load_texture(app, &arrow_tex, "./resources/Textures/menu_arrow.png");
    SDL_Rect arrow_left_dst[MAX_PLAYERS], arrow_right_dst[MAX_PLAYERS];
    for (int i = 0; i < MAX_PLAYERS; i++) {
        arrow_left_dst[i].w = CELL_SIZE;
        arrow_left_dst[i].h = CELL_SIZE;
        arrow_left_dst[i].x = 450 - CELL_SIZE * 2;
        arrow_left_dst[i].y = 25 + 100 * i;
        arrow_right_dst[i].w = CELL_SIZE;
        arrow_right_dst[i].h = CELL_SIZE;
        arrow_right_dst[i].x = 450 + CELL_SIZE * 4;
        arrow_right_dst[i].y = 25 + 100 * i;
    }

    Screen_item* background = menu_button_background(app, "./resources/Textures/background.png");
    Screen_item* start_button = menu_button_text(app, "Start", font, white);
    Screen_item* exit_button = menu_button_text(app, "Exit", font, white);

    // player name for each connected player
    Screen_item* players_screen_name[MAX_PLAYERS];
    // player snake selected color for each connected player
    SDL_Rect players_screen_snake[MAX_PLAYERS][3];
    for (int i = 0; i < MAX_PLAYERS; i++) {
        players_screen_name[i] = NULL;
    }

    // allocate memory for sent packet
    UDPpacket* pack_send = allocate_packet(PACKET_DATA_SIZE);

    // allocate memory for received packet
    UDPpacket* pack_recv = allocate_packet(PACKET_DATA_SIZE);

    // resolve server address TODO: bind address?
    int server_port = atoi(app->port);
    IPaddress server_addr = resolve_host(app->ip, server_port);

    // send join game request
    join_game_request(udp_sock, server_addr, pack_send);
    // wait for server to respond to request, only wait for 3 seconds
    unsigned time_when_req_sent = SDL_GetTicks();
    // used when client connecting and in main loop
    int request_type;

    while (app->running && next_menu_state == LOBBY) {
        //SDL_MouseButtonEvent mouse_event;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                // exit main loop
                app->running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (hover_state(start_button, Mx, My)) { /* START */
                    // Plays button press effect
                    play_sound(app->sound->press);
                    // go to other menu, exits loop
                    next_menu_state = START_GAME;
                } else if (hover_state(exit_button, Mx, My)) { /* EXIT */
                    // Plays button press effect
                    play_sound(app->sound->back);
                    // go to other menu, exits loop
                    next_menu_state = MAIN_MENU;
                } else if (is_hovering_over(&arrow_left_dst[game_state->client_id], Mx, My)) { /* ARROW LEFT PLAYER0 */
                    // Plays button press effect
                    play_sound(app->sound->back);
                    prev_player_color(game_state->players[game_state->client_id]);
                    send_color_change(udp_sock, server_addr, pack_send, game_state->players[game_state->client_id]);
                } else if (is_hovering_over(&arrow_right_dst[game_state->client_id], Mx, My)) { /* ARROW RIGHT PLAYER0 */
                    // Plays button press effect
                    play_sound(app->sound->back);
                    next_player_color(game_state->players[game_state->client_id]);
                    send_color_change(udp_sock, server_addr, pack_send, game_state->players[game_state->client_id]);
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    // return to main menu
                    next_menu_state = MAIN_MENU;
                    break;
                }
                break;
            }
        }

        // check for successful connection and if other clients have joined
        if (SDLNet_UDP_Recv(udp_sock, pack_recv)) {
            // get request type from packet
            sscanf((char*)pack_recv->data, "%d", &request_type);
            switch (request_type) {
            // these are the only expected types at this point
            case SUCCESSFUL_CONNECTION:
                successfully_connected(pack_recv->data, game_state, game_state->players, players_screen_name, players_screen_snake);
                break;
            case NEW_CLIENT_JOINED:
                // adds new player and increments nr_of_players
                new_client_joined(pack_recv->data, game_state, game_state->players, players_screen_name, players_screen_snake);
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
            case COLOR_CHANGE:
                handle_color_change(pack_recv->data, game_state->players);
                break;
            }
        }

        // 3 second timer to break out of loop if packet not received yet
        // prevents from getting stuck in while loop
        /* if(time_when_req_sent > time_when_req_sent + 3000) { */
        /*     printf("join: request timed out\n"); */
        /*     // exit for now */
        /*     exit(EXIT_FAILURE); */
        /*     break; */
        /* } */

        // clear screen before next render
        SDL_RenderClear(app->renderer);

        render_item(app, &background->rect, background->texture, BACKGROUND, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        render_item(app, &start_button->rect, start_button->texture, NULL, WINDOW_WIDTH - (WINDOW_WIDTH / 3), WINDOW_HEIGHT / 2 - BUTTON_H / 2, BUTTON_W, BUTTON_H);
        render_item(app, &exit_button->rect, exit_button->texture, NULL, WINDOW_WIDTH - (WINDOW_WIDTH / 3), (WINDOW_HEIGHT / 2 - TEXT_H / 2) + BUTTON_H, TEXT_W, TEXT_H);

        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (game_state->players[i] == NULL)
                continue;

            // free if not NULL before giving it a new value
            if (players_screen_name[i] != NULL) {
                free(players_screen_name[i]->texture);
                free(players_screen_name[i]);
            }

            players_screen_name[i] = menu_button_text(app, game_state->players[i]->name, small_font, white);

            render_item(app, &players_screen_name[i]->rect, players_screen_name[i]->texture, NULL, 20, 100 * i, TEXT_W, TEXT_H);
            // render left arrow
            SDL_RenderCopyEx(app->renderer, arrow_tex, NULL, &arrow_left_dst[game_state->client_id], 0, NULL, SDL_FLIP_NONE);
            for (int j = 0; j < 3; j++) {
                /* if(players_screen_snake[i][j] == NULL) */
                /*     continue; */

                /* render_item(app, &players_screen_snake[i][j], snake_sprite_textures[game_state->players[i]->color][j], NULL, 100 + CELL_SIZE * j, 25*i, CELL_SIZE, CELL_SIZE); */
                // render snake body color preview
                snake_src.x = snake_texture[j].x;
                snake_src.y = snake_texture[j].y;
                snake_dst.x = 450 + CELL_SIZE * j;
                snake_dst.y = 25 + 100 * i;
                SDL_RenderCopyEx(app->renderer, snake_sprite_textures[game_state->players[i]->color], &snake_src, &snake_dst, 90, NULL, SDL_FLIP_NONE);
            }
            // render right arrow
            SDL_RenderCopyEx(app->renderer, arrow_tex, NULL, &arrow_right_dst[game_state->client_id], 0, NULL, SDL_FLIP_HORIZONTAL);
        }

        //If-state for wether the text should switch color on hover or not
        if (hover_state(start_button, Mx, My)) {
            play_hover_sound(app->sound, &playsound);
            SDL_SetTextureColorMod(exit_button->texture, 127, 127, 127);
        } else if (hover_state(exit_button, Mx, My)) {
            play_hover_sound(app->sound, &playsound);
            SDL_SetTextureColorMod(exit_button->texture, 127, 127, 127);
        } else {
            if (!playsound) { // Makes sure the sound effect only plays once
                playsound = true;
            }
            SDL_SetTextureColorMod(start_button->texture, 255, 255, 255);
            SDL_SetTextureColorMod(exit_button->texture, 255, 255, 255);
            SDL_SetTextureColorMod(start_button->texture, 127, 127, 127);
        }

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_Delay(1000 / 60);
        SDL_GetMouseState(&Mx, &My);
    }
    SDL_StopTextInput();

    // free allocated memory on heap
    for (int i = 0; i < 5; i++) {
        SDL_DestroyTexture(snake_sprite_textures[i]);
    }
    TTF_CloseFont(small_font);
    free(arrow_tex);
    SDL_DestroyTexture(background->texture);
    free(background);
    SDL_DestroyTexture(start_button->texture);
    free(start_button);
    SDL_DestroyTexture(exit_button->texture);
    free(exit_button);

    return next_menu_state;
}

int game(App* app, TTF_Font* font, Game_State* game_state, UDPsocket udp_sock)
{
    int Mx, My;
    bool end_of_round = false, playsound_col_once = true, playscore_end_once = true;

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

    SDL_Texture* snake_sprite_textures[NR_OF_COLORS];
    load_texture(app, &snake_sprite_textures[0], "./resources/Textures/snake-sprite-green.png");
    load_texture(app, &snake_sprite_textures[1], "./resources/Textures/snake-sprite-blue.png");
    load_texture(app, &snake_sprite_textures[2], "./resources/Textures/snake-sprite-purple.png");
    load_texture(app, &snake_sprite_textures[3], "./resources/Textures/snake-sprite-yellow.png");
    load_texture(app, &snake_sprite_textures[4], "./resources/Textures/snake-sprite-orange.png");

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

    // allocate memory for sent packet
    UDPpacket* pack_send = allocate_packet(PACKET_DATA_SIZE);

    // allocate memory for received packet
    UDPpacket* pack_recv = allocate_packet(PACKET_DATA_SIZE);

    // resolve server address TODO: bind address?
    int server_port = atoi(app->port);
    IPaddress server_addr = resolve_host(app->ip, server_port);

    /* Network *net = init_net(app, PACKET_DATA_SIZE);

    // array to store info about connected players
    Player* players[MAX_PLAYERS] = { NULL, NULL, NULL, NULL };
    // keep track of number of joined clients
    // int nr_of_players = 0;
    // client id for this specific client, needed to select correct player from the players array
    // int client_id;

    // send join game request
    join_game_request(udp_sock, server_addr, pack_send);
    // wait for server to respond to request, only wait for 3 seconds
    unsigned time_when_req_sent = SDL_GetTicks();
    // used when client connecting and in main loop
    int request_type;

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
    }*/

    //////////////////////////////////

    game_state->scoreboard = create_scoreboard(app, font, game_state->players);

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
                    if (game_state->players[game_state->client_id]->snake->vel_y == 0)
                        game_state->players[game_state->client_id]->snake->dir = Up;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    if (game_state->players[game_state->client_id]->snake->vel_y == 0)
                        game_state->players[game_state->client_id]->snake->dir = Down;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    if (game_state->players[game_state->client_id]->snake->vel_x == 0)
                        game_state->players[game_state->client_id]->snake->dir = Right;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    if (game_state->players[game_state->client_id]->snake->vel_x == 0)
                        game_state->players[game_state->client_id]->snake->dir = Left;
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (hover_state(game_state->scoreboard->return_button, Mx, My)) {
                    // Makes space on the heap
                    free_scoreboard(game_state->scoreboard);
                    return MAIN_MENU;
                } else if (hover_state(game_state->scoreboard->mute, Mx, My)) {
                    if (app->sound->muted) { // Unmutes all the sounds and changes the speaker icon
                        // free mute before updating it
                        if (game_state->scoreboard->mute != NULL)
                            free(game_state->scoreboard->mute);
                        game_state->scoreboard->mute = menu_button_background(app, "./resources/Textures/speaker_icon.png");
                        app->sound->muted = false;
                    } else if (!app->sound->muted) { // Mutes all the sounds and changes the speaker icon
                        // free mute before updating it
                        if (game_state->scoreboard->mute != NULL)
                            free(game_state->scoreboard->mute);
                        game_state->scoreboard->mute = menu_button_background(app, "./resources/Textures/speaker_icon_mute.png");
                        app->sound->muted = true;
                    }
                } else if (hover_state(game_state->scoreboard->continue_button, Mx, My)) {
                    if (end_of_round) {
                        end_of_round = false;
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
            handle_received_packet(app, data, game_state, game_state->players);
            free(data);
        }

        // Checks if any collisons has occured with the walls
        if (collison_with_wall(game_state->players[game_state->client_id]->snake)) {
            game_state->players[game_state->client_id]->alive = false;
            send_collision(udp_sock, server_addr, pack_send, game_state->client_id);
            if (!app->sound->muted && playsound_col_once) { // makes sure the sound isnt mute and dosent play more than once
                play_sound(app->sound->wall_collison); // plays wall collison sound effect
                playsound_col_once = false; // Makes sure collison sound doesnt go on repeat in spectator mode
            }
        }

        // Checks if any collisons has occured with a snake
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (game_state->players[i] == NULL)
                continue;

            if (game_state->client_id == i)
                continue;

            if (collison_with_snake(game_state->players[game_state->client_id]->snake, game_state->players[i]->snake)) {
                game_state->players[game_state->client_id]->alive = false;
                send_collision(udp_sock, server_addr, pack_send, game_state->client_id);
                if (!app->sound->muted && playsound_col_once) { // makes sure the sound isnt mute and dosent play more than once
                    play_sound(app->sound->wall_collison); // plays wall collison sound effect
                    playsound_col_once = false; // Makes sure collison sound doesnt go on repeat in spectator mode
                }
            }
        }

        if (game_state->players[game_state->client_id]->alive && game_state->current_time > game_state->last_time + game_state->players[game_state->client_id]->snake->speed) {
            // update snake velocity based on direction state
            change_snake_velocity(game_state->players[game_state->client_id]->snake);
            new_snake_pos(game_state->players[game_state->client_id]->snake, true);
            head_adjecent_with_fruit(&game_state->players[game_state->client_id]->snake->head, game_state->fruits, game_state->nr_of_fruits);
            send_snake_position(udp_sock, server_addr, pack_send, game_state->players[game_state->client_id]);
            game_state->last_time = game_state->current_time;
        }

        /* update_state_if_fruit_collision(players[game_state->client_id]->snake, game_state->fruits, &game_state->nr_of_fruits); */
        fruit_collision(app, udp_sock, server_addr, pack_send, game_state->players, game_state->fruits, &game_state->nr_of_fruits, game_state->client_id);

        /////////////////////////////////////////////// Rendering section //////////////////////////////////////////////////////

        // TODO: maybe only call when fruit is eaten
        update_scoreboard(app, game_state->players, game_state->scoreboard); // updates the scoreboard

        if (end_of_round) {
            if (!app->sound->muted && playscore_end_once) {
                play_sound(app->sound->scoreboard); // plays wall collison sound effect
                playscore_end_once = false; // Makes sure collison sound doesnt go on repeat in spectator mode
            }

            // clear screen before next render
            SDL_RenderClear(app->renderer);
            render_end_of_round(app, game_state->scoreboard);
        } else if (!end_of_round) {
            // clear screen before next render
            SDL_RenderClear(app->renderer);
            SDL_Rect background_dst = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
            SDL_RenderCopy(app->renderer, background_tex, NULL, &background_dst);

            render_scoreboard(app, game_state->scoreboard);

            render_fruits(app, game_state->fruits, fruit_sprite_tex, fruit_texture);
            // render all snakes
            render_snakes(app, game_state->players, game_state->nr_of_players, snake_sprite_textures, snake_texture);
        }

        // present on screen
        SDL_RenderPresent(app->renderer);

        SDL_GetMouseState(&Mx, &My);
        SDL_Delay(1000 / 60);
    }

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

Scoreboard* create_scoreboard(App* app, TTF_Font* font, Player* players[])
{
    Scoreboard* scoreboard = malloc(sizeof(Scoreboard));

    scoreboard->background = menu_button_background(app, "./resources/Textures/Forest_green.jpg");
    scoreboard->scoreboard = menu_button_background(app, "./resources/Textures/menuButton.png");
    /*
    Screen_item* goal_text = menu_button_text(app, "Goal", font, white_txt);
    Screen_item* goal_nr = menu_button_text(app, "250", font, white_txt);
    */

    scoreboard->continue_button = menu_button_text(app, "Press to continue", font, white);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        // skip if player does not exist
        if (players[i] == NULL) {
            scoreboard->name[i] = NULL;
            scoreboard->score[i] = NULL;
            continue;
        }

        // TODO: use players[i]->name instead of app->player_name
        /* scoreboard->name[i] = menu_button_text(app, players[i]->name, font, green); */
        scoreboard->name[i] = menu_button_text(app, app->player_name, font, green);
        scoreboard->score[i] = menu_button_text(app, "0", font, green);
    }

    scoreboard->mute = menu_button_background(app, "./resources/Textures/speaker_icon.png");
    scoreboard->return_button = menu_button_background(app, "./resources/Textures/exit_button.png");

    return scoreboard;
}

void free_scoreboard(Scoreboard* scoreboard)
{
    free(scoreboard->background);
    free(scoreboard->scoreboard);
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (scoreboard->name[i] != NULL)
            free(scoreboard->name[i]);
        if (scoreboard->score[i] != NULL)
            free(scoreboard->score[i]);
    }
    free(scoreboard->mute);
    free(scoreboard->return_button);
    free(scoreboard->continue_button);
    free(scoreboard);
}

void update_scoreboard(App* app, Player* players[], Scoreboard* scoreboard)
{
    TTF_Font* font = TTF_OpenFont("./resources/Fonts/adventure.otf", 250);
    if (font == NULL)
        return;
    char buffer[50];

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (players[i] == NULL)
            continue;

        sprintf(buffer, "%d", players[i]->points);
        // free previous score before creating new one
        free(scoreboard->score[i]->texture);
        free(scoreboard->score[i]);
        // create new score text
        scoreboard->score[i] = menu_button_text(app, buffer, font, green);
    }

    TTF_CloseFont(font);
    font = NULL;
}

bool signs_of_life(Game_State* game_state, Player* players[])
{
    int tmp = 0;
    for (int i = 0; i < game_state->nr_of_players; i++) {
        if (players[i]->alive == false) {
            tmp++;
        }
    }
    if (tmp == game_state->nr_of_players)
        return true;
    else
        return false;
}