#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int main() {
    int fd = -1;
    if ((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("error creating socket");
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "\0");

    if(connect(fd,(struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1) {
        perror("error connecting to server");
    }

    char buff[20];
    int to_send = sprintf(buff, "HELLO WORLD!");

    if(write(fd, buff, to_send+1) == -1) {
        perror("error sending data to server");
    }
}