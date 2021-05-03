#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_net.h>
#include "network.h"
#include "snake.h"
#include "player.h"

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

void get_client_id(UDPpacket *pack_recv, int *client_id)
{
    int temp;
    sscanf(pack_recv->data, "%d %d", &temp, client_id);
}

void new_client_joined(UDPpacket *pack_recv, Player *players[], int *nr_of_players, int *client_id)
{
    int type, id;
    // get data from received packet
    sscanf(pack_recv->data, "%d", &type);
    // could be both SUCCESSFUL_CONNECTION and NEW_CLIENT_JOINED
    if(type == SUCCESSFUL_CONNECTION) {
        // gets the number of clients that already connected to the server before this client joined
        // format: type id nr_of_clients
        sscanf(pack_recv->data, "%d %d %d", &type, client_id, nr_of_players);
        // initialize players on this client that was already connected to the server
        for(int i = 0; i < *nr_of_players; i++) {
            players[i] = new_player(i);
        }

        // add new player, id is between 0-3
        players[*client_id] = new_player(*client_id);
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
        (*nr_of_players)++;
    }

    if(*nr_of_players > 4) {
        // shouldn't happen, if it does something probably went wrong
        exit(EXIT_FAILURE);
    }
}

void update_snake_pos_from_req(UDPpacket *pack_recv, Player *players[], int nr_of_players)
{
    int type, id, packet_nr, x, y, dir, angle;
    // format: type id last_received_packet_nr x y direction angle
    sscanf(pack_recv->data, "%d %d %d %d %d %d %d", &type, &id, &packet_nr, &x, &y, &dir, &angle);
    printf("snake pos: id: %d, pack_nr: %d, x: %d, y: %d, dir: %d, angle: %d\n", id, packet_nr, x, y, dir, angle);
    if(players[id]->last_received_packet_nr > packet_nr) {
        // return early because we've already received a newer packet
        return;
    }
    // update last received packet number to the packet number we just got
    players[id]->last_received_packet_nr = packet_nr;
    // update snake position
    players[id]->snake->dir = dir;
    players[id]->snake->head.next_pos.x = x;
    players[id]->snake->head.next_pos.y = y;
    /* players[id]->snake->head.angle = angle; */
}

void send_packet(UDPsocket socket, UDPpacket *packet) {
    int sent;
    sent = SDLNet_UDP_Send(socket, packet->channel, packet);
    if (!sent) {
        // TODO: better error handling
        fprintf(stderr, "Error: SDLNet_UDP_Send %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

void join_game_request(UDPsocket socket, IPaddress server_addr, UDPpacket *packet) {
    // format message and size before sending
    char msg[2];
    sprintf(msg, "%d", CLIENT_JOIN);

    // send join game request
    packet->data = msg;
    packet->channel = -1;
    packet->len = sizeof(packet->data)+1;
    packet->maxlen = sizeof(packet->data)+1;
    packet->address = server_addr;

    send_packet(socket, packet);    
}

void send_snake_position(UDPsocket socket, IPaddress server_addr, UDPpacket *packet, Player *player) {
    // format snake position information before send
    char msg[30];
    // increment how many packets client have sent
    player->packet_nr++;
    // format: type id packet_nr x y direction angle
    sprintf(msg, "%d %d %d %d %d %d %d", UPDATE_SNAKE_POS, player->client_id, player->packet_nr, player->snake->head.pos.x, player->snake->head.pos.y, player->snake->dir, player->snake->head.angle);
    packet->data = msg;
    packet->channel = -1;
    packet->len = sizeof(packet->data)+24;
    packet->maxlen = 1024;
    packet->address = server_addr;

    send_packet(socket, packet);
}
