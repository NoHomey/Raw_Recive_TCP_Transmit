#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Constants.h"

#define SOCKET_MESG_LENGTH 65536

int main(void) {
    struct sockaddr sock_addr;
    struct sockaddr_in source;
    struct sockaddr_in server;
    struct iphdr* ip_header;
    char message[] = SENDING_MESG_TEXT;
    char server_reply[SENDING_MESG_LENGTH];
    int sock_addr_size = sizeof(struct sockaddr);
    unsigned char* buffer = (unsigned char*) malloc(SOCKET_MESG_LENGTH);
    int sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    int sock_tcp = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_SOCKET_PORT);
    if((sock_raw != IS_ERROR) && (sock_tcp != IS_ERROR) && (connect(sock_tcp, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) != IS_ERROR)) {
        while(1) {
            if(recvfrom(sock_raw, buffer, SOCKET_MESG_LENGTH, 0, &sock_addr, (socklen_t*) &sock_addr_size) != IS_ERROR) {
                ip_header = ((struct iphdr*) (buffer + sizeof(struct ethhdr)));
                source.sin_addr.s_addr = ip_header->saddr;
                if(strcmp(inet_ntoa(source.sin_addr), WANTED_SENDER_IP_ADDR) == 0) {
                    if(send(sock_tcp, message, SENDING_MESG_LENGTH, 0) != IS_ERROR) {
                        if(recv(sock_tcp, server_reply, SENDING_MESG_LENGTH, 0) != IS_ERROR) {
                            puts(server_reply);
                        } else {
                            perror("Error while reciving from server\n");
                            return 1;
                        }
                    } else {
                        perror("Error while sending to server\n");
                        return 1;
                    }
                }
            } else {
                perror("Error while sniffing ips\n");
                return 1;
            }
        }
    } else {
        perror("Error while openning sockets or connecting to server\n");
        return 1;
    }
    close(sock_raw);
    return 0;
}
