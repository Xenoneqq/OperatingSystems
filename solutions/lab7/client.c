#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

struct message {
    long mtype;
    pid_t sender_pid;
    char message[1024];
};

int main() {
    key_t key = ftok("./chat", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    struct message msg;
    pid_t my_pid = getpid();
    msg.sender_pid = my_pid;

    msg.mtype = 1;
    snprintf(msg.message, sizeof(msg.message), "Hello I just joined :D\n");
    msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);

    pid_t pid = fork();
    if (pid == 0) {
        while (1) {
            if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), my_pid, 0) > 0) {
                printf("User %d: %s", msg.sender_pid, msg.message);
                fflush(stdout);
            }
        }
    } else {
        while (1) {
            fgets(msg.message, sizeof(msg.message), stdin);
            msg.mtype = 1;
            msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);
        }
    }

    return 0;
}
