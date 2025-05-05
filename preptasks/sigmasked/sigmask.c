#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main(){
    sigset_t sigset, pending;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
    raise(SIGUSR1);
    sigpending(&pending);
    raise(SIGUSR1);
    if(sigismember(&pending, SIGUSR1)){
        printf("USR1 has been signaled!\n");
    }
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);
    return 0;
}