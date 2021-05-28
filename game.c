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
    bool end_of_round = false;

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

    game_state->scoreboard = create_scoreboard(app, players);

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
                    if (players[game_state->client_id]->snake->vel_y == 0)
                        players[game_state->client_id]->snake->dir = Up;
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    if (players[game_state->client_id]->snake->vel_y == 0)
                        players[game_state->client_id]->snake->dir = Down;
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    if (players[game_state->client_id]->snake->vel_x == 0)
                        players[game_state->client_id]->snake->dir = Right;
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    if (players[game_state->client_id]->snake->vel_x == 0)
                        players[game_state->client_id]->snake->dir = Left;
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
                        if(game_state->scoreboard->mute != NULL)
                            free(game_state->scoreboard->mute);
                        game_state->scoreboard->mute = menu_button_background(app, "./resources/Textures/speaker_icon.png");
                        app->sound->muted = false;
                    } else if (!app->sound->muted) { // Mutes all the sounds and changes the speaker icon
                        // free mute before updating it
                        if(game_state->scoreboard->mute != NULL)
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
        fruit_collision(app, udp_sock, server_addr, pack_send, players, game_state->fruits, &game_state->nr_of_fruits, game_state->client_id);

        /////////////////////////////////////////////// Rendering section //////////////////////////////////////////////////////
        
        // TODO: maybe only call when fruit is eaten
        update_scoreboard(app, players, game_state->scoreboard); // updates the scoreboard

        if (end_of_round) {
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
            render_snakes(app, players, game_state->nr_of_players, snake_sprite_tex, snake_texture);
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

Scoreboard* create_scoreboard(App* app, Player* players[])
{
    Scoreboard* scoreboard = malloc(sizeof(Scoreboard));

    scoreboard->background = menu_button_background(app, "./resources/Textures/Forest_green.jpg");
    scoreboard->scoreboard = menu_button_background(app, "./resources/Textures/menuButton.png");
    /*
    Screen_item* goal_text = menu_button_text(app, "Goal", font, white_txt);
    Screen_item* goal_nr = menu_button_text(app, "250", font, white_txt);
    */
    TTF_Font* font = TTF_OpenFont("./resources/Fonts/adventure.otf", 250);

    scoreboard->continue_button = menu_button_text(app, "Press to continue", font, white);

    for(int i = 0; i < MAX_PLAYERS; i++) {
        // skip if player does not exist
        if(players[i] == NULL) {
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
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(scoreboard->name[i] != NULL)
            free(scoreboard->name[i]);
        if(scoreboard->score[i] != NULL)
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
    if(font == NULL)
        return;
    char buffer[50];

    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(players[i] == NULL)
            continue;

        sprintf(buffer, "%d", players[i]->points);
        // free previous score before creating new one
        free(scoreboard->score[i]);
        // create new score text
        scoreboard->score[i] = menu_button_text(app, buffer, font, green);
    }

    TTF_CloseFont(font);
    font = NULL;
}
