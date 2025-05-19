#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int fdchat[2];
    pipe(fdchat);
    pid_t pid = fork();
    if(pid == 0){
        close(fdchat[1]);
        size_t bytesread;
        while(1){
            char message[1024];
            bytesread = read(fdchat[0], message, sizeof(message));
            if(bytesread < 0){
                perror("Failed to read from chat");
                return 1;
            }
            message[bytesread] = '\0';
            if(strcmp(message , "quit\n") == 0 || strcmp(message , "quit") == 0){
                close(fdchat[0]);
                return 0;
            }
            printf("Server : %s" , message);
        }
    }
    else{
        close(fdchat[0]);
        while(1){
            char message[1024];
            if(fgets(message, sizeof(message), stdin) == NULL){
                perror("fgets didin't work");
                return 2;
            }
            write(fdchat[1] , message, strlen(message));
            if(strcmp(message , "quit\n") == 0 || strcmp(message , "quit") == 0){
                close(fdchat[1]);
                return 0;
            }
        }
    }
    return 0;
}