#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

char* PIPE = "localPipe";

int main(){
    double start, end;
    printf("Enter the start point : ");
    scanf("%lf", &start);
    printf("Enter the end point : ");
    scanf("%lf", &end);

    if(mkfifo(PIPE , 0666) == -1){
        perror("Failed to create FIFO");
        return 1;
    }

    pid_t pid = fork();
    if(pid == 0){
        execl("./riemann.o", "riemann" , NULL);
        perror("Riemann program was not executed");
        return 2;
    }

    int wr = open(PIPE , O_WRONLY);
    if(wr < 0){
        perror("Failed to open FIFO (write)");
        return 3;
    }
    if(write(wr , &start , sizeof(double)) == -1){
        perror("Failed to write to FIFO");
        return 4;
    }
    if(write(wr , &end , sizeof(double)) == -1){
        perror("Failed to write to FIFO");
        return 5;
    }
    close(wr);

    int rd = open(PIPE, O_RDONLY);
    if(rd < 0){
        perror("Failed to open FIFO");
        return 6;
    }
    size_t bytesread;
    double res;
    bytesread = read(rd , &res, sizeof(double));
    if(bytesread < 0){
        perror("Failed to read FIFO");
        return 7;
    }
    close(rd);
    printf("The integral is : %f\n", res);
    unlink(PIPE);
    return 0;
}