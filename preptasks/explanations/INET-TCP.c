#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 12345

int main() {
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(SERVER_PORT);
  server.sin_addr.s_addr = INADDR_ANY;
  //server.sin_addr.s_addr = inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

  bind(sock, (struct sockaddr*)&server, sizeof(server));
  listen(sock, 5);
  int client_fd = accept(sock, NULL, NULL);
  //int client_fd = accept(sock, (struct sockaddr*)&client, (socklen_t*)&client_len);
  read(client_fd, buffer, BUFFER_SIZE);
  //send(client_fd, buffer, BUFFER_SIZE, 0);

}