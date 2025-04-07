#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void handler(int status){
  printf("Signal recieved : %d\n", status);
}

int main(int argc, char **argv){
  if(argc == 1){
    printf("No argument passed...\n");
    return 0;
  }

  if(strcmp(argv[1] , "none") == 0){
    printf("Prepearing SIGUSR1 signal...\n");
    raise(SIGUSR1);
    printf("Signal was ignored...\n");
  }
  else if(strcmp(argv[1] , "ignore") == 0){
    signal(SIGUSR1 , SIG_IGN);
    printf("Prepearing SIGUSR1 signal...\n");
    raise(SIGUSR1);
    printf("Signal was ignored...\n");
  }
  else if(strcmp(argv[1] , "handle") == 0){
    signal(SIGUSR1, handler);
    printf("Prepearing SIGUSR1 signal...\n");
    raise(SIGUSR1);
  }
  else if(strcmp(argv[1] , "mask") == 0){
    sigset_t sigset, pendingset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
    raise(SIGUSR1);
    sigpending(&pendingset);
    if (sigismember(&pendingset, SIGUSR1)) {
      printf("SIGUSR1 is awaiting...\n");
    } 
    else {
      printf("SIGUSR1 is not awaiting...\n");
    }
  }
  else{
    printf("unknown command...\n");
  }
  return 0;
}