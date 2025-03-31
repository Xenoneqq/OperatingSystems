#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
  pid_t pid = fork();
  if(pid == 0){
    execl("/bin/ls", "ls", "-ld", NULL);
  }

  pid = fork();
  if(pid == 0){
    char* const av[]={"ls", "-ld", NULL};
    execv("/bin/ls", av);
  }

  return 0;
}