#ifndef NETWORK_H
#define NETWORK_H

#include "snake.h"
#include <SDL2/SDL_net.h>

enum Request_Type {
  JOIN = 0,
  UPDATE_SNAKE_POS = 1
};

UDPsocket open_client_socket();
UDPpacket *allocate_packet(int size);
IPaddress resolve_host(char *host, int port);
void handle_received_packet(UDPpacket *pack_recv);

void send_packet(UDPsocket socket, UDPpacket *packet);
void join_game_request(UDPsocket socket, IPaddress server_addr, UDPpacket *packet);
void send_snake_position(UDPsocket socket, IPaddress server_addr, UDPpacket *packet, Snake *snake);


#endif 