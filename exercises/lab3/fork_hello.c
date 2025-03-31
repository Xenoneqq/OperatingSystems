#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
  for(int i = 0; i < 5; i++){
    pid_t pid = fork();
    if(pid == 0){
      printf("Process id : %d\n", getpid());
      exit(0);
    }
  }
  exit(0);
}