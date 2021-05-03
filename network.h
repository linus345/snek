#ifndef NETWORK_H
#define NETWORK_H

#include <SDL2/SDL_net.h>
#include "snake.h"
#include "player.h"

enum Request_Type {
    CLIENT_JOIN = 0,
    NEW_CLIENT_JOINED = 1,
    SUCCESSFUL_CONNECTION = 2,
    FAILED_CONNECTION = 3,
    UPDATE_SNAKE_POS = 4
};

UDPsocket open_client_socket();
UDPpacket *allocate_packet(int size);
IPaddress resolve_host(char *host, int port);
void log_packet(UDPpacket *pack_recv);
void get_client_id(UDPpacket *pack_recv, int *client_id);
void new_client_joined(UDPpacket *pack_recv, Player *players[], int *nr_of_players, int *client_id);
void update_snake_pos_from_req(UDPpacket *pack_recv, Player *players[], int nr_of_players);
void send_packet(UDPsocket socket, UDPpacket *packet);
void join_game_request(UDPsocket socket, IPaddress server_addr, UDPpacket *packet);
void send_snake_position(UDPsocket socket, IPaddress server_addr, UDPpacket *packet, Player *player);


#endif 
