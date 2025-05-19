#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int global = 0;

void handler(int status){
    global++;
    printf("Alerted : %d\n", global);
    return;
}

int main(){
    signal(SIGUSR1, handler);
    pid_t pid = fork();
    if(pid == 0){
        while(1){
            printf("awaiting signal...\n");
            pause();
        }
    }
    else{
        while(1){
            sleep(1);
            kill(pid , SIGUSR1);
        }
    }
}