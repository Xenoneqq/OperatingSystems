#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void display(int status){
  printf("What a signal!\n");
}

int main(){
  signal(SIGUSR1 , display);
  raise(SIGUSR1);
  return 0;
}