#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <limits.h>

#define SERVER_SOCKET_PORT 8080
#define SNIFFER_IP_ADDR "192.168.0.104"
#define SENDING_MSG_TEXT "Hellow Snif!!!"
#define SENDING_MSG_LENGTH 15
#define IS_ERROR -1

int main(void) {
    int sender_sock;
    unsigned long int i;
    struct sockaddr_in sniffer_addr;
    char message[] = SENDING_MSG_TEXT;
    sender_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sender_sock == IS_ERROR) {
        perror("Error while opening tcp socket\n");
        goto close_socket;
    }
    sniffer_addr.sin_addr.s_addr = inet_addr(SNIFFER_IP_ADDR);
    sniffer_addr.sin_family = AF_INET;
    sniffer_addr.sin_port = htons(SERVER_SOCKET_PORT);
    if (connect(sender_sock, (struct sockaddr *) &sniffer_addr, sizeof(struct sockaddr_in)) == IS_ERROR) {
        perror("Error while connecting to sniffer\n");
        goto close_socket;
    }
    for (i = 0; i < ULONG_MAX; ++i) {
        if (send(sender_sock, message, SENDING_MSG_LENGTH, 0) == IS_ERROR) {
            perror("Error while sending to sniffer\n");
            goto close_socket;
        }
        sleep(1);
    }
    close_socket:
    {
        close(sender_sock);
        return 0;
    }
}
