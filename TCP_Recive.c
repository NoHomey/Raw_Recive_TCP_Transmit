#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "TCP_SOCKET_COMMON.h"
 
int main(void) {
    char client_message[MSG_LENGTH];
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc != -1) {
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(SOCKET_PORT);
        if(bind(socket_desc, (struct sockaddr*) &server, sizeof(server)) >= 0) {
            listen(socket_desc , 3);
            c = sizeof(struct sockaddr_in);
            client_sock = accept(socket_desc, (struct sockaddr*) &client, (socklen_t*) &c);
            if (client_sock >= 0) {
                while(recv(client_sock, client_message, MSG_LENGTH, 0) > 0) {
                    write(client_sock, client_message, MSG_LENGTH);
                }
            }
        }
    }
    return 0;
}
