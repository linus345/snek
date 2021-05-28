#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "network.h"
#include "game.h"
#include "snake.h"
#include "player.h"
#include "circular_buffer.h"

// global variable declared in main.c
extern bool thread_done;

void free_net(UDPsocket udp_sock, UDPpacket *pack_recv, UDPpacket *pack_send)
{
    // free packets
    // TODO: getting munmap_chunk(): invalid pointer when trying to free packets
    /* SDLNet_FreePacket(pack_recv); */
    /* SDLNet_FreePacket(pack_send); */
    // close udp socket
    SDLNet_UDP_Close(udp_sock);
}

UDPsocket open_client_socket() {
    // open udp socket
    UDPsocket udp_sock = SDLNet_UDP_Open(0);
    if(!udp_sock) {
        printf("SDLNet_UDP_Open: %s", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    return udp_sock;
}

UDPpacket *allocate_packet(int size) {
    // allocate memory for sent packet
    UDPpacket *pack = SDLNet_AllocPacket(size);
    if(pack == NULL) {
        fprintf(stderr, "Error: SDLNet_AllocPacket %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    // initialize memory to 0
    memset(pack->data, 0, size);
    return pack;
}

IPaddress resolve_host(char *host, int port) {
    IPaddress addr;
    if(SDLNet_ResolveHost(&addr, host, port) != 0) {
        fprintf(stderr, "Error: SDLNet_ResolveHost %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    return addr;
}

void log_packet(UDPpacket *pack_recv)
{
    printf("Channel: %d\n", pack_recv->channel);
    printf("Data: %s\n", pack_recv->data);
    printf("Len: %d\n", pack_recv->len);
    printf("Max len: %d\n", pack_recv->maxlen);
    printf("Status: %d\n", pack_recv->status);
    printf("src host: %u\n", pack_recv->address.host);
    printf("src port: %u\n", pack_recv->address.port);
}

int receive_packets_in_new_thread(void *void_args)
{
    Thread_Args *args = (Thread_Args *) void_args;
    printf("message from thread\n");
    // poll for packets
    while(!thread_done) {
        if(SDLNet_UDP_Recv(args->udp_sock, args->pack_recv)) {
            /* printf("--------received packet in thread--------\n"); */
            /* log_packet(args->pack_recv); */
            // add packet data to shared buffer
            write_to_buffer(args->buf, args->pack_recv->data);
        }
    }

    // exit with success
    return 0;
}

void handle_received_packet(App *app, Uint8 *data, Game_State *game_state, Player *players[])
{
    int request_type;
    /* printf("--------received packet in thread--------\n"); */
    /* log_packet(pack_recv); */
    // get request type from packet
    sscanf((char *) data, "%d", &request_type);
    switch(request_type) {
        case NEW_CLIENT_JOINED:
            new_client_joined(data, game_state, players);
            break;
        case UPDATE_SNAKE_POS:
            update_snake_pos_from_req(data, players);
            break;
        case RANDOM_POS:
            get_fruit_pos_and_spawn(data, game_state->fruits, &game_state->nr_of_fruits, game_state->nr_of_players, players[game_state->client_id]->snake);
            break;
        case RECEIVE_TICKS:
            handle_received_ticks(data, &game_state->current_time);
            break;
        case COLLISION:
            handle_collision(data, players);
            break;
        case ATE_FRUIT:
            handle_ate_fruit(app, data, players, game_state);
            break;
    }
}

void get_client_id(UDPpacket *pack_recv, int *client_id)
{
    int temp;
    sscanf((char *) pack_recv->data, "%d %d", &temp, client_id);
}

void new_client_joined(Uint8 *data, Game_State *game_state, Player *players[])
{
    int type, id;
    // get data from received packet
    sscanf((char *) data, "%d", &type);
    // could be both SUCCESSFUL_CONNECTION and NEW_CLIENT_JOINED
    if(type == SUCCESSFUL_CONNECTION) {
        // gets the number of clients that already connected to the server before this client joined
        // format: type id nr_of_clients
        sscanf((char *) data, "%d %d %d %u", &type, &game_state->client_id, &game_state->nr_of_players, &game_state->current_time);
        // initialize players on this client that was already connected to the server
        for(int i = 0; i < game_state->nr_of_players; i++) {
            players[i] = new_player(i);
        }

        // add new player, id is between 0-3
        players[game_state->client_id] = new_player(game_state->client_id);
        // indicate that client is connected
        game_state->connected = true;
    } else {
        // NEW_CLIENT_JOINED
        // format: type id
        sscanf((char *) data, "%d %d", &type, &id);

        // client already connected, return from function
        if(players[id] != NULL) {
            return;
        }

        // add new player, id is between 0-3
        players[id] = new_player(id);
        // increment number of players if new client joined
        game_state->nr_of_players++;
    }

    if(game_state->nr_of_players > 4) {
        // shouldn't happen, if it does something probably went wrong
        exit(EXIT_FAILURE);
    }
}

void update_snake_pos_from_req(Uint8 *data, Player *players[])
{
    int type, id, packet_nr, x, y, dir, angle;
    // format: type id last_received_packet_nr x y direction angle
    sscanf((char *) data, "%d %d %d %d %d %d %d", &type, &id, &packet_nr, &x, &y, &dir, &angle);
    printf("snake pos: id: %d, pack_nr: %d, x: %d, y: %d, dir: %d, angle: %d\n", id, packet_nr, x, y, dir, angle);
    if(players[id]->last_received_packet_nr > packet_nr) {
        // return early because we've already received a newer packet
        return;
    }
    printf("received packet_nr: %d from %d\n", packet_nr, id);
    // update last received packet number to the packet number we just got
    players[id]->last_received_packet_nr = packet_nr;
    // update snake position
    players[id]->snake->dir = dir;
    change_snake_velocity(players[id]->snake);
    new_snake_pos(players[id]->snake, false);
    players[id]->snake->head.pos.x = x;
    players[id]->snake->head.pos.y = y;
    players[id]->snake->head.angle = angle;
}

void handle_received_ticks(Uint8 *data, unsigned *current_time)
{
    int type;
    // format: type ticks
    sscanf((char *) data, "%d %u", &type, current_time);
}

void handle_collision(Uint8 *data, Player *players[])
{
    int type, id;
    // format: type id
    sscanf((char *) data, "%d %d", &type, &id);
    // update player alive state to dead
    players[id]->alive = false;
}

void handle_ate_fruit(App *app, Uint8 *data, Player *players[], Game_State *game_state)
{
    int type, id, fruit_index;
    // format: type id fruit_index
    sscanf((char *) data, "%d %d %d", &type, &id, &fruit_index);
    printf("--------------\n");
    printf("type: %d, id: %d, fruit_index: %d\n", type, id, fruit_index);
    printf("--------------\n");
    // update snake that ate fruit and remove the fruit that was ate
    update_state_if_fruit_collision(app, players[id], game_state->fruits, &game_state->nr_of_fruits, fruit_index);
}

void send_packet(UDPsocket udp_sock, UDPpacket *pack_send) {
    int sent;
    sent = SDLNet_UDP_Send(udp_sock, pack_send->channel, pack_send);
    if (!sent) {
        // TODO: better error handling
        fprintf(stderr, "Error: SDLNet_UDP_Send %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

void join_game_request(UDPsocket udp_sock, IPaddress server_addr, UDPpacket *pack_send) {
    // format message and size before sending
    /* char msg[2]; */
    char msg[PACKET_DATA_SIZE] = {0};
    // format: type
    sprintf(msg, "%d", CLIENT_JOIN);

    // send join game request
    /* strcpy(pack_send->data, msg); */
    memcpy(pack_send->data, msg, sizeof(msg));
    pack_send->channel = -1;
    pack_send->len = sizeof(pack_send->data)+1;
    pack_send->maxlen = sizeof(pack_send->data)+1;
    pack_send->address = server_addr;

    send_packet(udp_sock, pack_send);    

    // free packet data
    /* free(pack_send->data); */
}

void send_snake_position(UDPsocket socket, IPaddress server_addr, UDPpacket *pack_send, Player *player) {
    // format snake position information before send
    /* char msg[30]; */
    char msg[PACKET_DATA_SIZE] = {0};
    // increment how many packets client have sent
    player->packet_nr++;
    printf("sending packet_nr: %d from: %d\n", player->packet_nr, player->client_id);
    // format: type id packet_nr x y direction angle
    sprintf(msg, "%d %d %d %d %d %d %d", UPDATE_SNAKE_POS, player->client_id, player->packet_nr, player->snake->head.pos.x, player->snake->head.pos.y, player->snake->dir, player->snake->head.angle);
    /* strcpy(pack_send->data, msg); */
    memcpy(pack_send->data, msg, sizeof(msg));
    pack_send->channel = -1;
    pack_send->len = sizeof(pack_send->data)+24;
    pack_send->maxlen = 1024;
    pack_send->address = server_addr;

    send_packet(socket, pack_send);

    // free packet data
    /* free(pack_send->data); */
}

void send_collision(UDPsocket socket, IPaddress server_addr, UDPpacket *pack_send, int client_id)
{
    // format message
    /* char msg[4]; */
    char msg[PACKET_DATA_SIZE] = {0};
    // format: type id
    sprintf(msg, "%d %d", COLLISION, client_id);

    /* strcpy(pack_send->data, msg); */
    memcpy(pack_send->data, msg, sizeof(msg));
    pack_send->channel = -1;
    pack_send->len = sizeof(pack_send->data)+4;
    pack_send->maxlen = 1024;
    pack_send->address = server_addr;

    send_packet(socket, pack_send);

    // free packet data
    /* free(pack_send->data); */
}

void send_ate_fruit(UDPsocket socket, IPaddress server_addr, UDPpacket *pack_send, int client_id, int fruit_index)
{
    // format message
    char msg[PACKET_DATA_SIZE] = {0};
    // format: type id fruit_index
    sprintf(msg, "%d %d %d", ATE_FRUIT, client_id, fruit_index);

    /* strcpy(pack_send->data, msg); */
    memcpy(pack_send->data, msg, sizeof(msg));
    pack_send->channel = -1;
    pack_send->len = sizeof(pack_send->data)+4;
    pack_send->maxlen = 1024;
    pack_send->address = server_addr;

    printf("sending ATE_FRUIT: type id fruit_index\n");
    log_packet(pack_send);

    send_packet(socket, pack_send);

    // free packet data
    /* free(pack_send->data); */
}
