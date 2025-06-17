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

int main() {
  key_t key = ftok("./key", 65);
  int msgid = msgget(key, 0666 | IPC_CREAT);
  struct message msg;
  msg.mtype = 1;
  msg.sender = getpid();

  while(1){
    fgets(msg.mtext, 1024, stdin);
    msg.mtype = 1;
    msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);
  }

  return 0;
}