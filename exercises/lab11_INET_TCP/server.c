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
    int listen_fd = -1;
    int client_fd = -1;
    socklen_t addr_len;
    char buffer[MAX_BUFFER_SIZE + 1];
    ssize_t bytes_read;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("server: error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_len = sizeof(server_addr);

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

    while(1) {
        bytes_read = read(client_fd, buffer, MAX_BUFFER_SIZE);
        if (bytes_read == -1) {
            perror("server: error or client disconnected");
            close(client_fd);
            close(listen_fd);
            exit(EXIT_FAILURE);
        }
        else if (bytes_read == 0) {
          perror("server: connection closed");
          close(client_fd);
          close(listen_fd);
          exit(EXIT_FAILURE);
        }

        buffer[bytes_read] = '\0';
        printf("server: Message from client: \"%s\"\n", buffer);
    }


    shutdown(client_fd, SHUT_RDWR);
    close(client_fd);
    close(listen_fd);

    return 0;
}