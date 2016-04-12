#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <limits.h>

#define SOCKET_MESG_LENGTH 65536
#define SERVER_IP_ADDR "192.168.0.100"
#define SERVER_SOCKET_PORT 8080
#define WANTED_SENDER_IP_ADDR "192.168.0.103"
#define SENDING_MESG_TEXT "Hellow Server!"
#define SENDING_MESG_LENGTH 15
#define IS_ERROR -1

int main(void) {
    struct sockaddr sock_addr;
    struct sockaddr_in source, server;
    struct iphdr* ip_header;
    char message[] = SENDING_MESG_TEXT;
    char server_reply[SENDING_MESG_LENGTH];
    int sock_addr_size = sizeof(struct sockaddr);
    unsigned char* buffer = (unsigned char*) malloc(SOCKET_MESG_LENGTH);
    unsigned long int i;
    int sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    int sock_tcp = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_SOCKET_PORT);
    if((sock_raw != IS_ERROR) && (sock_tcp != IS_ERROR) && (connect(sock_tcp, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) != IS_ERROR)) {
        for(i = 0; i < ULONG_MAX; ++i) {
            if(recvfrom(sock_raw, buffer, SOCKET_MESG_LENGTH, 0, &sock_addr, (socklen_t*) &sock_addr_size) != IS_ERROR) {
                ip_header = ((struct iphdr*) (buffer + sizeof(struct ethhdr)));
                source.sin_addr.s_addr = ip_header->saddr;
                //puts(inet_ntoa(source.sin_addr));
                if(strcmp(inet_ntoa(source.sin_addr), WANTED_SENDER_IP_ADDR) == 0) {
                    if(send(sock_tcp, message, SENDING_MESG_LENGTH, 0) != IS_ERROR) {
                        if(recv(sock_tcp, server_reply, SENDING_MESG_LENGTH, 0) != IS_ERROR) {
                            puts(server_reply);
                        } else {
                            perror("Error while reciving from server\n");
                            goto close_sockets;
                        }
                    } else {
                        perror("Error while sending to server\n");
                        goto close_sockets;
                    }
                }
            } else {
                perror("Error while sniffing ips\n");
                goto close_sockets;
            }
        }
    } else {
        perror("Error while openning sockets or connecting to server\n");
        goto close_sockets;
    }
    close_sockets: {
        close(sock_raw);
        close(sock_tcp);
        return 0;
    }
}
