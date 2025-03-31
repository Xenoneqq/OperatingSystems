#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv){
  if (argc < 2) {
    printf("Missing argument\n");
    return -1;
  }
  int n = atoi(argv[1]);
  pid_t pid;
  for(int i = 0; i < n; i++){
    pid = fork();
    if(pid == 0){
      printf("parent: %d\tchild: %d\n", getppid(), getpid());
      exit(0);
    }
  }

  while (wait(NULL) > 0);
  if(pid != 0){
    printf("%d\n", n);
  }

  return 0;
}