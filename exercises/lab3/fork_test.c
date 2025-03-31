#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int counter = 0;

void fork_test(){
  printf("THIS IS FORK TEST\n");
  fork();
  int id = fork();
  if(id == 0) counter++;
  counter++;
  printf("%d\n", counter);
  return 0;
}

void v_fork_test(){
  printf("THIS IS V_FORK TEST\n");
  fork();
  int id = vfork();
  if(id == 0) counter++;
  counter++;
  printf("%d\n", counter);
  return 0;
}

int main(){
  fork_test();
  //v_fork_test();
  return 0;
}