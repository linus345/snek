#ifndef NETWORK_H
#define NETWORK_H

#include "snake.h"
#include <SDL2/SDL_net.h>

enum Request_Type {
    CLIENT_JOIN = 0,
    SUCCESSFUL_CONNECTION = 1,
    FAILED_CONNECTION = 2,
    UPDATE_SNAKE_POS = 3
};

UDPsocket open_client_socket();
UDPpacket *allocate_packet(int size);
IPaddress resolve_host(char *host, int port);
void handle_received_packet(UDPpacket *pack_recv, int *client_id);
void get_client_id(UDPpacket *pack_recv, int *client_id);
void send_packet(UDPsocket socket, UDPpacket *packet);
void join_game_request(UDPsocket socket, IPaddress server_addr, UDPpacket *packet);
void send_snake_position(UDPsocket socket, IPaddress server_addr, UDPpacket *packet, int client_id, Snake *snake);


#endif 
