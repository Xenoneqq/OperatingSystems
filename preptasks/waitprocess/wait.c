#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();
    if(pid == 0){
        sleep(3);
        exit(10);
    } else {
        int status;
        while(1){
            if(waitpid(pid, &status, WNOHANG) != 0) break;
        }
        if(WIFEXITED(status)){
            if( WEXITSTATUS(status) >= 0){
                printf("The program finished with status %d\n", WEXITSTATUS(status));
            }
            else{
                printf("Program was interrupted\n");
            }
        }
        else if(WIFSIGNALED(status)){
            printf("Program finished with signal");
        }
    }
}