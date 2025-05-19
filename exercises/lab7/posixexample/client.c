#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define QUEUE_NAME "/my_message_queue"
#define MAX_MSG_SIZE 1024
#define MSG_BUFFER_SIZE 256

typedef struct {
    long msg_priority;
    char msg_text[MSG_BUFFER_SIZE];
} message_t;

int main() {
    unsigned int prio = 2;

    mqd_t mq;
    struct mq_attr attr;
    message_t message;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(message_t);
    attr.mq_curmsgs = 0;

    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0664, &attr);
    if (mq == -1) {
        perror("mq_open");
        exit(1);
    }

    message.msg_priority = 1;
    snprintf(message.msg_text, sizeof(message.msg_text), "Message with priority 1");
    if (mq_send(mq, (const char *)&message, sizeof(message), message.msg_priority) == -1) {
        perror("mq_send");
        exit(1);
    }

    message.msg_priority = 2;
    snprintf(message.msg_text, sizeof(message.msg_text), "Message with priority 2");
    if (mq_send(mq, (const char *)&message, sizeof(message), message.msg_priority) == -1) {
        perror("mq_send");
        exit(1);
    }

    message.msg_priority = 1;
    snprintf(message.msg_text, sizeof(message.msg_text), "Another message with priority 1");
    if (mq_send(mq, (const char *)&message, sizeof(message), message.msg_priority) == -1) {
        perror("mq_send");
        exit(1);
    }

    message.msg_priority = 3;
    snprintf(message.msg_text, sizeof(message.msg_text), "Message with priority 3");
    if (mq_send(mq, (const char *)&message, sizeof(message), message.msg_priority) == -1) {
        perror("mq_send");
        exit(1);
    }

    for (int i = 0; i < 4; i++) {
        ssize_t bytes_read;
        bytes_read = mq_receive(mq, (char *)&message, sizeof(message), &prio);

        if (bytes_read >= 0) {
            printf("Received: %s with priority %ld\n", message.msg_text, message.msg_priority);
        } else {
            perror("mq_receive");
        }
    }

    if (mq_close(mq) == -1) {
        perror("mq_close");
        exit(1);
    }

    if (mq_unlink(QUEUE_NAME) == -1) {
        perror("mq_unlink");
        exit(1);
    }

    return 0;
}
