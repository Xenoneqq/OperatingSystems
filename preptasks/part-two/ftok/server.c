#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


struct message {
  long mtype;
  pid_t sender;
  char mtext[1024];
};

int main(){
  key_t key = ftok("./key", 65);
  int msgid = msgget(key, 0666 | IPC_CREAT);
  if (msgid == -1) {
    perror("msgget");
    exit(1);
  }

  struct message msg;
  while(1){
    if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 1, 0) > 0) {
      printf("[%d]: %s\n", msg.sender, msg.mtext);
    }
  }

  return 0;
}