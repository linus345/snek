#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_net.h>
#include "network.h"
#include "game.h"
#include "snake.h"
#include "player.h"
#include "circular_buffer.h"

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
    if(!pack) {
        fprintf(stderr, "Error: SDLNet_AllocPacket %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
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
    while(true) {
        if(SDLNet_UDP_Recv(args->udp_sock, args->pack_recv)) {
            /* printf("--------received packet in thread--------\n"); */
            /* log_packet(args->pack_recv); */
            // add packet to shared buffer
            printf("-------------------writing to buffer-------------------\n");
            write_to_buffer(args->buf, args->pack_recv);
            printf("-----------------------write done----------------------\n");
        }
    }
}

void handle_received_packet(UDPpacket *pack_recv, Game_State *game_state, Player *players[])
{
    int request_type;
    printf("--------received packet in thread--------\n");
    log_packet(pack_recv);
    // get request type from packet
    sscanf(pack_recv->data, "%d", &request_type);
    switch(request_type) {
        case NEW_CLIENT_JOINED:
            new_client_joined(pack_recv, game_state, players);
            break;
        case UPDATE_SNAKE_POS:
            update_snake_pos_from_req(pack_recv, players);
            break;
        case RANDOM_POS:
            get_fruit_pos_and_spawn(pack_recv, game_state->fruits, &game_state->nr_of_fruits, players[game_state->client_id]->snake);
            break;
        case RECEIVE_TICKS:
            handle_received_ticks(pack_recv, &game_state->current_time);
            break;
        case COLLISION:
            handle_collision(pack_recv, players);
            break;
    }
}

void get_client_id(UDPpacket *pack_recv, int *client_id)
{
    int temp;
    sscanf(pack_recv->data, "%d %d", &temp, client_id);
}

void new_client_joined(UDPpacket *pack_recv, Game_State *game_state, Player *players[])
{
    int type, id;
    // get data from received packet
    sscanf(pack_recv->data, "%d", &type);
    // could be both SUCCESSFUL_CONNECTION and NEW_CLIENT_JOINED
    if(type == SUCCESSFUL_CONNECTION) {
        // gets the number of clients that already connected to the server before this client joined
        // format: type id nr_of_clients
        sscanf(pack_recv->data, "%d %d %d %u", &type, &game_state->client_id, &game_state->nr_of_players, &game_state->current_time);
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
        sscanf(pack_recv->data, "%d %d", &type, &id);

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

void update_snake_pos_from_req(UDPpacket *pack_recv, Player *players[])
{
    int type, id, packet_nr, x, y, dir, angle;
    // format: type id last_received_packet_nr x y direction angle
    sscanf(pack_recv->data, "%d %d %d %d %d %d %d", &type, &id, &packet_nr, &x, &y, &dir, &angle);
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

void handle_received_ticks(UDPpacket *pack_recv, unsigned *current_time)
{
    int type;
    // format: type ticks
    sscanf(pack_recv->data, "%d %u", &type, current_time);
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

void handle_collision(UDPpacket *pack_recv, Player *players[])
{
    int type, id;
    // format: type id
    sscanf(pack_recv->data, "%d %d", &type, &id);
    // update player alive state to dead
    players[id]->alive = false;
}

void join_game_request(UDPsocket udp_sock, IPaddress server_addr, UDPpacket *pack_send) {
    // format message and size before sending
    char msg[2];
    sprintf(msg, "%d", CLIENT_JOIN);

    // send join game request
    pack_send->data = msg;
    pack_send->channel = -1;
    pack_send->len = sizeof(pack_send->data)+1;
    pack_send->maxlen = sizeof(pack_send->data)+1;
    pack_send->address = server_addr;

    send_packet(udp_sock, pack_send);    
}

void send_snake_position(UDPsocket socket, IPaddress server_addr, UDPpacket *packet, Player *player) {
    // format snake position information before send
    char msg[30];
    // increment how many packets client have sent
    player->packet_nr++;
    printf("sending packet_nr: %d from: %d\n", player->packet_nr, player->client_id);
    // format: type id packet_nr x y direction angle
    sprintf(msg, "%d %d %d %d %d %d %d", UPDATE_SNAKE_POS, player->client_id, player->packet_nr, player->snake->head.pos.x, player->snake->head.pos.y, player->snake->dir, player->snake->head.angle);
    packet->data = msg;
    packet->channel = -1;
    packet->len = sizeof(packet->data)+24;
    packet->maxlen = 1024;
    packet->address = server_addr;

    send_packet(socket, packet);
}

void send_collision(UDPsocket socket, IPaddress server_addr, UDPpacket *pack_send, int client_id)
{
    // format message
    char msg[4];
    // format: type id
    sprintf(msg, "%d %d", COLLISION, client_id);

    pack_send->data = msg;
    pack_send->channel = -1;
    pack_send->len = sizeof(pack_send->data)+4;
    pack_send->maxlen = 1024;
    pack_send->address = server_addr;

    send_packet(socket, pack_send);
}
