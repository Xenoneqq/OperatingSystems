#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 12345

int main(){
    int server_fd, client_fd;
    struct sockaddr_in server_address, client_address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0)

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address))

    listen(server_fd, 5)

    client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address)

    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    bytes_read = read(client_fd, buffer, BUFFER_SIZE);
    buffer[bytes_read] = '\0';
    printf("server: Message from client: \"%s\"\n", buffer);

    shutdown(client_fd, SHUT_RDWR);
    shutdown(server_fd, SHUT_RDWR);
    close(client_fd);
    close(server_fd);
    return 0;
}