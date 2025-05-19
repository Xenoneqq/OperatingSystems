#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MAX_CLIENTS 10

struct message {
    long mtype;
    pid_t sender_pid;
    char message[1024];
};

pid_t clients[MAX_CLIENTS];
int client_count = 0;

void register_client(pid_t pid) {
    for (int i = 0; i < client_count; ++i)
        if (clients[i] == pid) return;

    if (client_count < MAX_CLIENTS) {
        clients[client_count++] = pid;
        printf("New Client: %d\n", pid);
    }
}

void broadcast(int msgid, struct message* msg) {
    for (int i = 0; i < client_count; ++i) {
        if (clients[i] != msg->sender_pid) {
            struct message copy = *msg;
            copy.mtype = clients[i];
            msgsnd(msgid, &copy, sizeof(*msg) - sizeof(long), 0);
        }
    }
}

int main() {
    key_t key = ftok("./chat", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    struct message msg;

    while (1) {
        if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 1, 0) > 0) {
            register_client(msg.sender_pid);
            printf("Message from %d: %s", msg.sender_pid, msg.message);
            broadcast(msgid, &msg);
        }
    }

    return 0;
}
