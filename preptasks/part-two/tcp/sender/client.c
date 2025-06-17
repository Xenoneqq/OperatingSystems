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

int main() {
  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd == -1) {
    perror("error opening socket");
    close(client_fd);
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(SERVER_PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;

  if(connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
    perror("error connecting to server");
    close(client_fd);
    exit(EXIT_FAILURE);
  }

  char message[] = "Hello There!";
  size_t bytes_sent = send(client_fd, message, sizeof(message), 0);
  if (bytes_sent == -1) {
    perror("error sending message");
    close(client_fd);
    exit(EXIT_FAILURE);
  }

  shutdown(client_fd, SHUT_RDWR);
  close(client_fd);
  return 0;
}