#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SOCKET_MSG_LENGTH 65536

int main(void) {
    struct sockaddr sock_addr;
    struct sockaddr_in source;
    struct iphdr *ip_header;
    int sock_addr_size = sizeof(struct sockaddr);
    unsigned char *buffer = (unsigned char *) malloc(SOCKET_MSG_LENGTH);
    int sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    if (sock_raw >= 0) {
        while (1) {
            if (recvfrom(sock_raw, buffer, SOCKET_MSG_LENGTH, 0, &sock_addr, (socklen_t *) &sock_addr_size) >= 0) {
                ip_header = ((struct iphdr *) (buffer + sizeof(struct ethhdr)));
                source.sin_addr.s_addr = ip_header->saddr;
                printf("IP: %s\n", inet_ntoa(source.sin_addr));
            }
        }
    }
    close(sock_raw);
    return 0;
}
