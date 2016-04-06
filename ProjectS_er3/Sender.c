#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "TCP_SOCKET_COMMON.h"

int main(void) {
    int sock;
    struct sockaddr_in server;
    char message[] = SENDING_MESG_TEXT;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock != -1) {
        server.sin_addr.s_addr = inet_addr(SNIFFER_IP_ADDR);
        server.sin_family = AF_INET;
        server.sin_port = htons(SERVER_SOCKET_PORT);
        if(connect(sock, (struct sockaddr*) &server , sizeof(struct sockaddr_in)) != IS_ERROR) {
            while(1) {
                if(send(sock, message, SENDING_MESG_LENGTH, 0) != IS_ERROR) {
                    sleep(1);
                } else {
                    perror("Error while sending to sniffer\n");
                    return 1;
                }
            }
        }  else {
            perror("Error while connecting to sniffer\n");
            return 1;
        }
    }  else {
        perror("Error while openning tcp socket\n");
        return 1;
    }
    close(sock);
    return 0;
}
