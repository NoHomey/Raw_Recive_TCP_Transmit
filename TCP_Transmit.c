#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "TCP_SOCKET_COMMON.h"
 
int main(void) {
    int sock;
    struct sockaddr_in server;
    char message[] = MSG_TEXT;
    char server_reply[MSG_LENGTH];
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock != -1) {
        server.sin_addr.s_addr = inet_addr(IP_ADDR);
        server.sin_family = AF_INET;
        server.sin_port = htons(SOCKET_PORT);
        if(connect(sock, (struct sockaddr*) &server , sizeof(struct sockaddr_in)) >= 0) {
            while(1) {
                if(send(sock, message, MSG_LENGTH, 0) >= 0) {
                    if(recv(sock, server_reply, MSG_LENGTH, 0) >= 0) {
                        puts(server_reply);
                    }
                }
            }
        }
    }
    close(sock);
    return 0;
}
