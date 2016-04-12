#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <limits.h>

#define SERVER_SOCKET_PORT 8080
#define SNIFFER_IP_ADDR "192.168.0.104"
#define SENDING_MESG_TEXT "Hellow Snif!!!"
#define SENDING_MESG_LENGTH 15
#define IS_ERROR -1

int main(void) {
    int sock;
    unsigned long int i;
    struct sockaddr_in server;
    char message[] = SENDING_MESG_TEXT;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock != IS_ERROR) {
        server.sin_addr.s_addr = inet_addr(SNIFFER_IP_ADDR);
        server.sin_family = AF_INET;
        server.sin_port = htons(SERVER_SOCKET_PORT);
        if(connect(sock, (struct sockaddr*) &server , sizeof(struct sockaddr_in)) >= 0) {
            for(i = 0; i < ULONG_MAX; ++i) {
                if(send(sock, message, SENDING_MESG_LENGTH, 0) != IS_ERROR) {
                    sleep(1);
                } else {
                    perror("Error while sending to sniffer\n");
                    goto close_socket;
                }
            }
        }  else {
            perror("Error while connecting to sniffer\n");
            goto close_socket;
        }
    }  else {
        perror("Error while openning tcp socket\n");
        goto close_socket;
    }
    close_socket: {
        close(sock);
        return 0;
    }
}
