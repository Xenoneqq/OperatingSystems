#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    int global = 0;
    pid_t pid = vfork();

    if(pid == 0){
        while(global < 10){
            printf("%d\n" , global++);
        }
    }
    else
    {
        while(global < 10){
            printf("%d\n" , global++);
        }
    }

    return 0;
}