#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

#define UNIX_PATH_MAX 100

int main() {
    int fd = -1;
    if ((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("error creating socket");
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    addr.sun_path[0] = '\0';
    if(bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1) {
        perror("error binding");
    }

    char buff[20];
    if(read(fd, buff, 20) == -1) {
        perror("error recieving message");
    }

    printf("Message from client: \n%s\n", buff);

    shutdown(fd, SHUT_RDWR);
    close(fd);
}