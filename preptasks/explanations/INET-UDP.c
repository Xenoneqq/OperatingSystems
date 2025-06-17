#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 12345

int main(){
  int sock = socket(AF_INET, SOCK_DGRAM, 0);

  sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(SERVER_PORT);
  server.sin_addr.s_addr = INADDR_ANY;
  //server.sin_addr.s_addr = inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

  bind(sock, (struct sockaddr*)&server, sizeof(server));
  recvfrom(sock, buffer, sizeof(buffer), 0, NULL, NULL);
  //recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &client_length);
  sendto(sock, buffer, sizeof(buffer), 0, NULL, NULL);
  //sendto(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&server, sizeof(server));
}