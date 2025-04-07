#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int task_changes = 0;
pid_t looppid;
int loop_active = 0;

void pressed(){
  printf("CTRL + C was pressed...\n");
}

void retrieve_task(int signo, siginfo_t *info, void *extra)
{
  task_changes++;
  if(loop_active){
    kill(looppid, SIGKILL);
    loop_active = 0;
  }
  int int_val = info->si_value.sival_int;
  if(int_val == 1){
    printf("Task retrieved so far : %d\n", task_changes);
  }
  else if(int_val == 2){
    int i = 1;
    looppid = fork();
    if(looppid > 0) loop_active = 1;
    if(looppid == 0){
      while(1){
        printf("count is : %d\n", i);
        sleep(1);
        i++; 
      }
    }
  }
  else if(int_val == 3){
    signal(SIGINT, SIG_IGN);
  }
  else if(int_val == 4){
    signal(SIGINT, pressed);
  }
  else if(int_val == 5){
    exit(0);
  }
}

int main(){
  printf("Started as process id : %d\n",getpid());
  struct sigaction action;
  action.sa_flags = SA_SIGINFO;
  action.sa_sigaction = &retrieve_task;
  sigaction(SIGUSR1, &action, NULL);
  while(1){
    pause();
  }
}