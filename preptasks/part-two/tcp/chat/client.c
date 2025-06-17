#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 12345

int client_fd;

void handler(int status) {
  shutdown(client_fd, SHUT_RDWR);
  close(client_fd);
  printf("Program finished normally");
  return 0;
}

int main() {
  client_fd = socket(AF_INET, SOCK_STREAM, 0);
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

  signal(SIGINT, handler);
  char message[BUFFER_SIZE];
  while(1) {
    printf("[client]: ");
    if (fgets(message, BUFFER_SIZE, stdin) == NULL) break;
    size_t len = strlen(message);
    if (len > 0 && message[len - 1] == '\n')
      message[len - 1] = '\0';

    ssize_t sent = send(client_fd, message, strlen(message), 0);
    if (sent == -1) {
      perror("send");
      break;
    }
  }

  shutdown(client_fd, SHUT_RDWR);
  close(client_fd);
  return 0;
}