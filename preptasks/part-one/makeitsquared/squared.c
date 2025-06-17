#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if(pid == 0){
        close(fd[0]);
        int val = 5;
        if(write(fd[1], &val, sizeof(int)) < 0){
            perror("Failed to send value with PIPE");
            return 1;
        }
        close(fd[1]);
        return 0;
    } else {
        close(fd[1]);
        size_t bytesread;
        int val;
        bytesread = read(fd[0], &val, sizeof(int));
        if(bytesread <= 0){
            perror("Failed to read value stored in PIPE");
            return 2;
        }
        printf("Recieved number %d square of it is %d\n", val, val * val);
        close(fd[0]);
        return 0;
    }
}