#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int count = 0;

void handler(int status, siginfo_t *siginf, void *extra){
    int val = siginf->si_value.sival_int;
    printf("value is %d\n", val);
}

int main(){
    pid_t pid = fork();
    struct sigaction action;
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = &handler;
    sigaction(SIGUSR1, &action, NULL);

    if(pid == 0){
        while(1) pause();
    }
    else{
        union sigval value;
        while(1){
            count++;
            value.sival_int = count;
            printf("Sending signal\n");
            sigqueue(pid, SIGUSR1, value);
            sleep(1);
        }
    }
    return 0;
}