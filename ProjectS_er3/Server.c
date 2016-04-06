#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Constants.h"

int main(void) {
    char client_message[SENDING_MESG_LENGTH];
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc != -1) {
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(SERVER_SOCKET_PORT);
        if(bind(socket_desc, (struct sockaddr*) &server, sizeof(server)) != IS_ERROR) {
            listen(socket_desc, 3);
            c = sizeof(struct sockaddr_in);
            client_sock = accept(socket_desc, (struct sockaddr*) &client, (socklen_t*) &c);
            if(client_sock != IS_ERROR) {
                while((read_size = recv(client_sock, client_message, SENDING_MESG_LENGTH, 0)) > 0) {
                    write(client_sock, client_message, SENDING_MESG_LENGTH);
                }
            } else {
                perror("Error while accepting socket\n");
                goto close_sockets;
            }
        } else {
            perror("Error while binding socket\n");
            goto close_sockets;
        }
    } else {
        perror("Error while openning tcp socket\n");
        goto close_sockets;
    }
    close_sockets: {
        close(socket_desc);
        close(client_sock);
        return 0;
    }
}
