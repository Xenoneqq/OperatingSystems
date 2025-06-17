#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <error.h>
#include <errno.h>
#include <string.h>

int main() {
    pid_t pid;
    time_t t;
    int status;

    if ((pid = fork()) < 0)
        perror("fork");
    else if (pid == 0) {
        sleep(5);
        exit(5);
    } else do 
    {
        pid = waitpid(pid, &status, WNOHANG);
        if(pid == -1){
            perror("waitpid");
            return errno;
        } else if (pid == 0){
            time_t date = time(NULL);
            char* finaltime = ctime(&date);
            if(finaltime){
                finaltime[strcspn(finaltime, "\n")] = '\0';
                printf("Process still works : %s\n", finaltime);
            }
            sleep(1);
        } else {
            if (WIFEXITED(status)){
                printf("Process ended in statys %d\n", WEXITSTATUS(status));
            }
            else{
                puts("Process has been shut down incorrectly");
            }
        }
    } while (pid == 0);
    puts("KONIEC");
    return 0;
} 
