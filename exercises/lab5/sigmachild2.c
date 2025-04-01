#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void sighand(int signo, siginfo_t *info, void *extra)
{
       int int_val = info->si_value.sival_int;
       printf("Signal Recieved : [%d]\n", int_val);
}

int counter = 0;

int main(){
  struct sigaction action;
  action.sa_flags = SA_SIGINFO;
  action.sa_sigaction = &sighand;
  sigaction(SIGUSR1, &action, NULL);

  pid_t pid = fork();
  if(pid == 0){
    while(1) pause();
  }
  else{
    union sigval value;
    while(1){
      counter++;
      value.sival_int = counter;
      printf("signal sent : %d\n", counter);
      sigqueue(pid, SIGUSR1 , value);
      sleep(1);
    }
  }
  return 0;
}