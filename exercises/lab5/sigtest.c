#include <signal.h>
#include <stdio.h>

int main(){
  signal(SIGKILL , SIG_IGN);
  raise(SIGKILL);
  printf("tu sie proces dzieje...");
  return 0;
}
