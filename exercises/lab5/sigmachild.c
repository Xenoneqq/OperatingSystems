#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int counter = 0;
void handler(int signal){
  counter++;
  printf("signal recieved : %d\n" , counter);
}

int main(){
  signal(SIGUSR1 , handler);
  pid_t pid = fork();
  if(pid == 0){
    while(1) pause();
  }
  else{
    while(1){
      printf("signal sent...\n");
      kill(pid, SIGUSR1);
      sleep(1);
    }
  }
  return 0;
}