#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int clientfd;

void handler(int sig){
  shutdown(clientfd, SHUT_RDWR);
  close(clientfd);
  exit(0);
}

int main(){
  clientfd = socket(AF_INET, SOCK_DGRAM, 0);
  signal(SIGINT, handler);

  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);

  inet_aton("127.0.0.1", &server.sin_addr)
  connect(clientfd, (struct sockaddr*)&server, sizeof(server))

  char chat_buffer[BUFFER_SIZE];
  while(1){
    fgets(chat_buffer, BUFFER_SIZE, stdin);
    send(clientfd, chat_buffer, strlen(chat_buffer), 0);
  }

}