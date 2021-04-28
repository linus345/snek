#ifndef NETWORK_H
#define NETWORK_H

#include <SDL2/SDL_net.h>

UDPsocket open_client_socket();
UDPpacket *allocate_packet(int size);
IPaddress resolve_host(char *host, int port);
void handle_received_packet(UDPpacket *pack_recv);

#endif