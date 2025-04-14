#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){
  int fd[2];
  pipe(fd);
  pid_t pid = fork();
  if(pid == 0){
    close(fd[1]);
    char c;
    while(read(fd[0], &c, 1) > 0){
      write(STDOUT_FILENO, &c, 1);
    }
    printf("E\n");
  }
  else{
    close(fd[0]);
    write(fd[1], "ABC", 3);
    close(fd[1]);
    waitpid(pid, NULL, 0);
  }
}
