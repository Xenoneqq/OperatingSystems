#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MAX_TEXT 512

struct msg_buffer {
    long msg_type;
    char msg_text[MAX_TEXT];
};

int main() {
    key_t key = ftok("progfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    struct msg_buffer message;
    message.msg_type = 1;
    strcpy(message.msg_text, "Hello, this is message with type 1");

    msgsnd(msgid, &message, sizeof(message), 0);
    message.msg_type = 2;
    strcpy(message.msg_text, "Hello, this is message with type 2");

    msgsnd(msgid, &message, sizeof(message), 0);
    message.msg_type = 1;
    strcpy(message.msg_text, "Another message with type 1");

    msgsnd(msgid, &message, sizeof(message), 0);

    return 0;
}
