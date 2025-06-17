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

#define BUFFER_SIZE 1024
#define SERVER_PORT 12345

int server_fd, client_fd;

void handler(int status) {
  shutdown(client_fd, SHUT_RDWR);
  close(client_fd);
  shutdown(server_fd, SHUT_RDWR);
  close(server_fd);
  printf("Program finished normally");
  return 0;
}

int main(){
  struct sockaddr_in server_address, client_address;

  if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    perror("socket");
    exit(1);
  }

  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(SERVER_PORT);

  if(bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1){
    perror("bind");
    close(server_fd);
    exit(1);
  }

  if(listen(server_fd, 5) == -1){
    perror("listen");
    close(server_fd);
    exit(1);
  }

  if((client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address)) == -1){
    perror("accept");
    close(server_fd);
    exit(1);
  }

  char buffer[BUFFER_SIZE];
  size_t bytes_read;
  signal(SIGINT, handler);

  while(1){
    bytes_read = read(client_fd, buffer, BUFFER_SIZE);

    if(bytes_read == -1){
      perror("read");
      break;
    }
    else if(bytes_read == 0){
      printf("[server]: Connection closed\n");
      break;
    }
    buffer[bytes_read] = '\0';

    printf("[server]: \"%s\"\n", buffer);
  }
  shutdown(client_fd, SHUT_RDWR);
  shutdown(server_fd, SHUT_RDWR);
  close(client_fd);
  close(server_fd);
  return 0;
}