// client.c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int sock_fd;

void *recv_thread(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        ssize_t n = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) {
            printf("Disconnected from server\n");
            exit(EXIT_FAILURE);
        }
        buffer[n] = 0;

        if (strncmp(buffer, "ALIVE", 5) == 0) {
            const char *alive_resp = "ALIVE\n";
            send(sock_fd, alive_resp, strlen(alive_resp), 0);
        } else {
            printf("%s", buffer);
            fflush(stdout);
        }
    }
    return NULL;
}

void sigint_handler(int signo) {
    const char *stop_msg = "STOP\n";
    send(sock_fd, stop_msg, strlen(stop_msg), 0);
    close(sock_fd);
    printf("\nClient exited correctly.\n");
    exit(EXIT_SUCCESS);
}

int main() {
    signal(SIGINT, sigint_handler);

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("failed to create a socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("failed to use inet_pton");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("failed to connect");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    pthread_t tid;
    if (pthread_create(&tid, NULL, recv_thread, NULL) != 0) {
        perror("failed to create a thread");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        strcat(buffer, "\n");

        if (send(sock_fd, buffer, strlen(buffer), 0) == -1) {
            perror("failed to send");
            break;
        }
    }

    pthread_cancel(tid);
    pthread_join(tid, NULL);
    close(sock_fd);
    return 0;
}
