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
    int listen_fd = -1;
    int client_fd = -1;
    struct sockaddr_un server_addr;
    socklen_t addr_len;
    char buffer[MAX_BUFFER_SIZE + 1];
    ssize_t bytes_read;

    listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("server: error creating socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    server_addr.sun_path[0] = '\0';
    strncpy(server_addr.sun_path + 1, ABSTRACT_SOCKET_NAME, sizeof(server_addr.sun_path) - 2);

    addr_len = offsetof(struct sockaddr_un, sun_path) + 1 + strlen(ABSTRACT_SOCKET_NAME);

    if (bind(listen_fd, (struct sockaddr *)&server_addr, addr_len) == -1) {
        perror("server: error binding");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(listen_fd, 2) == -1) {
        perror("server: error listening");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    client_fd = accept(listen_fd, NULL, NULL);
    if (client_fd == -1) {
        perror("server: error accepting connection");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    bytes_read = read(client_fd, buffer, MAX_BUFFER_SIZE);
    if (bytes_read == -1) {
        perror("server: error receiving message");
        close(client_fd);
        close(listen_fd);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0';

    printf("server: Message from client: \"%s\"\n", buffer);

    shutdown(client_fd, SHUT_RDWR);
    close(client_fd);
    close(listen_fd);

    return 0;
}