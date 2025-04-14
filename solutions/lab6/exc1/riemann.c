#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

double func(float x){
    return 4 / (x*x + 1);
}

int main(int argc, char *argv[]){
    if(argc == 1){
        perror("The argument k is missing");
        return 1;
    }
    int k = atoi(argv[1]);

    double step = 1.0f / (double)k;
    double sum = 0;
    __pid_t pid;
    for(int i = 0; i < k; i++){
        int fd[2];
        if(pipe(fd) == -1){
            perror("Filed to create a pipe");
            return 2;
        }
        pid = fork();
        if(pid < 0){
            perror("Failed to fork");
            return 3;
        }
        // child
        if(pid == 0){
            close(fd[0]);
            double result = func(i * step) * step;
            write(fd[1] , &result , sizeof(double));
            close(fd[1]);
            return 0;
        }
        // parent
        else{
            close(fd[1]);
            double result;
            read(fd[0], &result, sizeof(double));
            sum += result;
        }
    }
    printf("Derivative [0 , 1] of func is : %f\n", sum);
    return 0;
}