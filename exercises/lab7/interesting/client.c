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
    while (1) {
        msgrcv(msgid, &message, sizeof(message), 0, 0);
        printf("Received: %s (type: %ld)\n", message.msg_text, message.msg_type);
        if (strcmp(message.msg_text, "Another message with type 1") == 0)
            break;
    }

    return 0;
}
