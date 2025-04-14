#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(){
    int wr = open("sorted.txt" , O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(wr < 0){
        perror("Failed to create file");
        return 1;
    }
    if(dup2(wr, STDOUT_FILENO) < 0){
        perror("Failed to set descriptor for FILE as STDOUT");
        return 2;
    }
    pid_t pid = fork();
    if(pid == 0){
        execl("/bin/sort" , "sort", "./file.txt", NULL);
        perror("Failed to launch sort script");
        return 3;
    }
    close(wr);
    return 0;
}