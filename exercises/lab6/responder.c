#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main(){
    int fd[2];
    int readSize = 1024;
    pipe(fd);
    pid_t pid = fork();
    // child
    if(pid == 0){
        close(fd[1]);
        char buffer[readSize];
        size_t bytesRead;
        while((bytesRead = read(fd[0], buffer, sizeof(buffer) - 1)) > 0){
            buffer[bytesRead] = '\0';
            printf("Received message : %s\n" , buffer);
        }
        close(fd[0]);
        printf("Done responding...\n");
    }
    // parent
    else{
        close(fd[0]);
        char message[readSize];
        while(1){
            if(fgets(message , sizeof(message), stdin) == NULL){
                break;
            }

            message[strcspn(message , "\n")] = '\0';
            if(strlen(message) == 0) break;
            write(fd[1] , message, strlen(message));
        }

        close(fd[1]);
        printf("Done sending...\n");
    }

    return 0;
}