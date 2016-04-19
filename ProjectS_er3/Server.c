#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_SOCKET_PORT 8080
#define SENDING_MSG_LENGTH 15
#define IS_ERROR -1

int main(int argc, char **argv) {
    char client_message[SENDING_MSG_LENGTH];
    int sock, client_sock, addr_len;
    struct sockaddr_in server, client;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == IS_ERROR) {
        perror("Error while opening tcp socket\n");
        goto close_sockets;
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVER_SOCKET_PORT);
    if (bind(sock, (struct sockaddr *) &server, sizeof(server)) == IS_ERROR) {
        perror("Error while binding socket\n");
        goto close_sockets;
    }
    if (listen(sock, 9) == IS_ERROR) {
        perror("Error while listening on socket\n");
        goto close_sockets;
    }
    addr_len = sizeof(struct sockaddr_in);
    client_sock = accept(sock, (struct sockaddr *) &client, (socklen_t *) &addr_len);
    if (client_sock == IS_ERROR) {
        perror("Error while listening on socket\n");
        goto close_sockets;
    }
    while ((recv(client_sock, client_message, SENDING_MSG_LENGTH, 0)) > 0) {
        if (argv[1][0] == 'a') {
            write(client_sock, client_message, SENDING_MSG_LENGTH);
        }
    }
    close_sockets:
    {
        close(sock);
        return 0;
    }
}
