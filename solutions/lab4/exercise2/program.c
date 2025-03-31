#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int global = 0;
int code;

int main(int argc, char **argv){
  if (argc < 2) {
    printf("Missing argument\n");
    return -1;
  }
  char* path = argv[1];

  printf("%s\n" , argv[0]);
  int local = 0;

  pid_t pid = fork();
  if(pid < 0){
    perror("fork failed");
    exit(1);
  }
  // child process
  if(pid == 0){
    printf("child process\n");
    global++; local++;
    printf("child pid = %d, parent pid = %d\n", getpid(), getppid());
    printf("child's local = %d, child's global = %d\n", local, global);
    execl("/bin/ls" ,"ls", path, NULL);
    perror("execl failed");
    exit(2);
  }
  // parent process
  else{
    int status;
    if(wait(&status) == -1){
      perror("wait failed : no child process to wait for...");
      exit(3);
    }
    printf("parent process\n");
    printf("parent pid = %d, child pid = %d\n", getpid(), pid);
    if (WIFEXITED(status)) {
      printf("Child exit code: %d\n", WEXITSTATUS(status));
    } else {
        printf("Child did not exit normally\n");
    }
  }

  return 0;
}