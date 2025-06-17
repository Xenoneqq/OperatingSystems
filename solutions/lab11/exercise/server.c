// server.c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define PORT 12345
#define BUFFER_SIZE 1024
#define MAX_EVENTS 10
#define MAX_CLIENTS 100

typedef struct {
    int fd;
    int id;
    time_t last_alive;
} client_t;

client_t clients[MAX_CLIENTS];
int clients_count = 0;
int next_client_id = 1;

int set_fd_noblock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl get");
        return -1;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl set");
        return -1;
    }
    return 0;
}

void remove_client(int epoll_fd, int idx) {
    client_t *cl = &clients[idx];
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, cl->fd, NULL);
    close(cl->fd);
    printf("Client %d disconnected\n", cl->id);

    for (int i = idx; i < clients_count - 1; i++) {
        clients[i] = clients[i + 1];
    }
    clients_count--;
}

void send_to_client(int fd, const char *msg) {
    send(fd, msg, strlen(msg), 0);
}

void broadcast_message(int sender_fd, const char *msg) {
    for (int i = 0; i < clients_count; i++) {
        if (clients[i].fd != sender_fd) {
            send_to_client(clients[i].fd, msg);
        }
    }
}

int find_client_index_by_fd(int fd) {
    for (int i = 0; i < clients_count; i++) {
        if (clients[i].fd == fd) return i;
    }
    return -1;
}

int find_client_index_by_id(int id) {
    for (int i = 0; i < clients_count; i++) {
        if (clients[i].id == id) return i;
    }
    return -1;
}

void send_client_list(int fd) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "ACTIVE CLIENTS (%d): ", clients_count);
    for (int i = 0; i < clients_count; i++) {
        char idbuf[32];
        snprintf(idbuf, sizeof(idbuf), "%d ", clients[i].id);
        strncat(buffer, idbuf, sizeof(buffer) - strlen(buffer) - 1);
    }
    strncat(buffer, "\n", sizeof(buffer) - strlen(buffer) - 1);
    send_to_client(fd, buffer);
}

void handle_message(int epoll_fd, int idx, char *msg) {
    client_t *cl = &clients[idx];
    msg[strcspn(msg, "\r\n")] = 0;

    if (strncmp(msg, "LIST", 4) == 0) {
        send_client_list(cl->fd);
    }
    else if (strncmp(msg, "2ALL ", 5) == 0) {
        char *text = msg + 5;

        char buffer[BUFFER_SIZE];
        time_t now = time(NULL);
        char timestr[64];
        strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", localtime(&now));

        snprintf(buffer, sizeof(buffer), "[%s] Client %d to ALL: %s\n", timestr, cl->id, text);
        broadcast_message(cl->fd, buffer);
    }
    else if (strncmp(msg, "2ONE ", 5) == 0) {
        char *space = strchr(msg + 5, ' ');
        if (!space) {
            send_to_client(cl->fd, "Usage: 2ONE <id> <message>\n");
            return;
        }
        *space = 0;
        int target_id = atoi(msg + 5);
        char *text = space + 1;

        int target_idx = find_client_index_by_id(target_id);
        if (target_idx == -1) {
            send_to_client(cl->fd, "Client not found\n");
            return;
        }

        char buffer[BUFFER_SIZE];
        time_t now = time(NULL);
        char timestr[64];
        strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", localtime(&now));

        snprintf(buffer, sizeof(buffer), "[%s] Client %d to Client %d: %s\n", timestr, cl->id, target_id, text);
        send_to_client(clients[target_idx].fd, buffer);
    }
    else if (strncmp(msg, "STOP", 4) == 0) {
        remove_client(epoll_fd, idx);
    }
    else if (strncmp(msg, "ALIVE", 5) == 0) {
        cl->last_alive = time(NULL);
    }
    else {
        send_to_client(cl->fd, "Unknown command\n");
    }
}

void ping_clients(int epoll_fd) {
    time_t now = time(NULL);
    char ping_msg[] = "ALIVE\n";

    for (int i = clients_count - 1; i >= 0; i--) {
        if (difftime(now, clients[i].last_alive) > 10) {
            printf("Client %d timed out\n", clients[i].id);
            remove_client(epoll_fd, i);
        } else {
            send_to_client(clients[i].fd, ping_msg);
        }
    }
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("failed to create the socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("failed to bind the socket with the address");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, SOMAXCONN) == -1) {
        perror("failed to listen out for connections");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (set_fd_noblock(server_fd) == -1) {
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("failed to create epoll");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("failed to add server_fd to epoll");
        close(server_fd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    struct epoll_event events[MAX_EVENTS];

    time_t last_ping = time(NULL);

    while (1) {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, 1000);
        if (n == -1) {
            if (errno == EINTR) continue;
            perror("failed to wait for epoll interaction");
            break;
        }

        time_t now = time(NULL);
        if (difftime(now, last_ping) >= 5) {
            ping_clients(epoll_fd);
            last_ping = now;
        }

        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == server_fd) {
                // Nowy klient
                int client_fd = accept(server_fd, NULL, NULL);
                if (client_fd == -1) {
                    perror("failed to accept connection");
                    continue;
                }

                if (set_fd_noblock(client_fd) == -1) {
                    close(client_fd);
                    continue;
                }

                if (clients_count >= MAX_CLIENTS) {
                    printf("Max clients reached, rejecting connection\n");
                    close(client_fd);
                    continue;
                }

                clients[clients_count].fd = client_fd;
                clients[clients_count].id = next_client_id++;
                clients[clients_count].last_alive = time(NULL);
                clients_count++;

                struct epoll_event client_event;
                client_event.events = EPOLLIN | EPOLLET;
                client_event.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event) == -1) {
                    perror("failed to add client to epoll");
                    close(client_fd);
                    continue;
                }

                char welcome[BUFFER_SIZE];
                snprintf(welcome, sizeof(welcome), "Welcome! Your client ID is %d\n", clients[clients_count - 1].id);
                send_to_client(client_fd, welcome);

                printf("New client connected: id=%d fd=%d\n", clients[clients_count - 1].id, client_fd);
            } else {
                int client_fd = events[i].data.fd;
                int idx = find_client_index_by_fd(client_fd);
                if (idx == -1) {
                    close(client_fd);
                    continue;
                }

                char buf[BUFFER_SIZE];
                ssize_t count = read(client_fd, buf, sizeof(buf) - 1);
                if (count <= 0) {
                    remove_client(epoll_fd, idx);
                    continue;
                }
                buf[count] = 0;

                handle_message(epoll_fd, idx, buf);
            }
        }
    }

    close(server_fd);
    close(epoll_fd);
    return 0;
}
