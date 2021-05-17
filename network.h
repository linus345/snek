#ifndef NETWORK_H
#define NETWORK_H

#include <SDL2/SDL_net.h>
#include "snake.h"
#include "player.h"
#include "circular_buffer.h"

typedef struct {
    UDPsocket udp_sock;
    UDPpacket *pack_recv;
    UDPpacket *pack_send;
    Circular_Buffer *buf;
} Thread_Args;

enum Request_Type {
    CLIENT_JOIN = 0,
    NEW_CLIENT_JOINED = 1,
    SUCCESSFUL_CONNECTION = 2,
    FAILED_CONNECTION = 3,
    UPDATE_SNAKE_POS = 4,
    RANDOM_POS = 5,
    RECEIVE_TICKS = 6,
    COLLISION = 7
};

void free_net(UDPsocket udp_sock, UDPpacket *pack_recv, UDPpacket *pack_send);
UDPsocket open_client_socket();
UDPpacket *allocate_packet(int size);
IPaddress resolve_host(char *host, int port);
void log_packet(UDPpacket *pack_recv);
int receive_packets_in_new_thread(void *void_args);
void handle_received_packet(UDPpacket *pack_recv, Game_State *game_state, Player *players[]);
void get_client_id(UDPpacket *pack_recv, int *client_id);
void new_client_joined(UDPpacket *pack_recv, Game_State *game_state, Player *players[]);
void update_snake_pos_from_req(UDPpacket *pack_recv, Player *players[]);
void handle_received_ticks(UDPpacket *pack_recv, unsigned *current_time);
void handle_collision(UDPpacket *pack_recv, Player *players[]);
void send_packet(UDPsocket udp_sock, UDPpacket *pack_send);
void join_game_request(UDPsocket udp_sock, IPaddress server_addr, UDPpacket *pack_send);
void send_snake_position(UDPsocket socket, IPaddress server_addr, UDPpacket *packet, Player *player);
void send_collision(UDPsocket socket, IPaddress server_addr, UDPpacket *pack_send, int client_id);

#endif 
