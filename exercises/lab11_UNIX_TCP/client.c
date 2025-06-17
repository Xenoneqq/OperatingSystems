#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>

#define ABSTRACT_SOCKET_NAME "simple_socket"
#define MAX_BUFFER_SIZE 100

int main() {
    int sock = -1;
    struct sockaddr_un server_addr;
    socklen_t addr_len;
    char buffer[MAX_BUFFER_SIZE];
    int msg_len;
    ssize_t bytes_sent;

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("error creating socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    server_addr.sun_path[0] = '\0';
    strncpy(server_addr.sun_path + 1, ABSTRACT_SOCKET_NAME, sizeof(server_addr.sun_path) - 2);

    addr_len = offsetof(struct sockaddr_un, sun_path) + 1 + strlen(ABSTRACT_SOCKET_NAME);

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

    bytes_sent = write(sock, buffer, msg_len);
    if (bytes_sent == -1) {
        perror("error sending data to server");
        close(sock);
        exit(EXIT_FAILURE);
    }

    shutdown(sock, SHUT_RDWR);
    close(sock);

    return 0;
}