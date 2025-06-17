#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int sockfd, clientfd;

void handler(int sig){
  shutdown(sockfd, SHUT_RDWR);
  shutdown(clientfd, SHUT_RDWR);
  close(sockfd);
  close(clientfd);
  printf("Server shutdown safely.");
  fflush(stdout);
  exit(0);
}

int main() {
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1) {
    perror("socket");
    exit(1);
  }

  signal(SIGINT, handler);

  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = INADDR_ANY;

  if(bind(sockfd, (struct sockaddr *) &server, sizeof(struct sockaddr_in)) == -1){
    perror("bind");
    close(sockfd);
    exit(1);
  }

  char buffer[BUFFER_SIZE];
  while(1){
    size_t bytes_read = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
    if(bytes_read == -1){
      perror("read");
    }
    if(bytes_read == 0){
      printf("Client disconnected\n");
      fflush(stdout);
    }
    else {
      buffer[bytes_read] = '\0';
      printf("%s\n", buffer);
      fflush(stdout);
    }
  }
}