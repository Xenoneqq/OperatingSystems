#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <netinet/in.h>

#define MAX_BUFFER_SIZE 100
#define SERV_PORT 12345

int main() {
    int sock = -1;
    socklen_t addr_len;
    char buffer[MAX_BUFFER_SIZE];
    int msg_len;
    ssize_t bytes_sent;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_len = sizeof(server_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, addr_len) == -1) {
        perror("error connecting to server");
        close(sock);
        exit(EXIT_FAILURE);
    }

    msg_len = snprintf(buffer, sizeof(buffer), "HELLO WORLD! From PID %d", getpid());
    if (msg_len >= sizeof(buffer)) {
        fprintf(stderr, "Client: Warning: message truncated.\n");
        msg_len = sizeof(buffer) - 1;
    }

    while(1) {
        bytes_sent = send(sock, buffer, msg_len, 0);
        if (bytes_sent == -1) {
            perror("error sending message");
            close(sock);
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }

    shutdown(sock, SHUT_RDWR);
    close(sock);

    return 0;
}