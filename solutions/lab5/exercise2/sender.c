#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main(){
  union sigval value;
  int action;

  pid_t pid;
  printf("Write the pid of catcher process : ");
  scanf("%d",&pid);
  printf("The selected process is : %d\n\n", pid);

  while(1){
    printf("Choose an action : ");
    scanf("%d",&action);
    if(action < 1 || action > 5){
      printf("Please choose correct action 1- 5\n");
      continue;
    }

    value.sival_int = action;
    printf("signal sent : %d\n", action);
    sigqueue(pid, SIGUSR1 , value);
    sleep(1);
  }
}