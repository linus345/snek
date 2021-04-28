#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_net.h>
#include "network.h"
#include "snake.h"

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

void handle_received_packet(UDPpacket *pack_recv) {
    printf("Channel: %d\n", pack_recv->channel);
    printf("Data: %s\n", pack_recv->data);
    printf("Len: %d\n", pack_recv->len);
    printf("Max len: %d\n", pack_recv->maxlen);
    printf("Status: %d\n", pack_recv->status);
    printf("src host: %u\n", pack_recv->address.host);
    printf("src port: %u\n", pack_recv->address.port);
}

void send_packet(UDPsocket socket, UDPpacket *packet) {
    int sent;
    sent = SDLNet_UDP_Send(socket, packet->channel, packet);
    if (!sent) {
        fprintf(stderr, "Error: SDLNet_UDP_Send %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

void join_game_request(UDPsocket socket, IPaddress server_addr, UDPpacket *packet) {
    // formate message and size before sending
    char msg[2];
    sprintf(msg, "%d", JOIN);

    // send join game request
    packet->data = msg;
    packet->channel = -1;
    packet->len = sizeof(packet->data)+1;
    packet->maxlen = sizeof(packet->data)+1;
    packet->address = server_addr;

    send_packet(socket, packet);    
}

void send_snake_position(UDPsocket socket, IPaddress server_addr, UDPpacket *packet, Snake *snake) {
    // formate snake position information before send
    char msg[20];
    sprintf(msg, "%d %d %d", UPDATE_SNAKE_POS, snake->head.pos.x, snake->head.pos.y);
    packet->data = msg;
    packet->channel = -1;
    packet->len = sizeof(packet->data)+3;
    packet->maxlen = 1024;
    packet->address = server_addr;

    send_packet(socket, packet);
}