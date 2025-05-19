#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <semaphore.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <mqueue.h>

#define USERWRITE "/user_write"
#define PRINTERREAD "/printer_read"
#define QUEUEFREE "/queue_status"
#define QUEUEREQUESTS "/queue_request_count"
#define SHM_NAME "/print_queue_shm"

#define PRINTSIZE 10
#define QUEUESIZE 10
#define MAX_USERS 5
#define MAX_PRINTERS 3

typedef struct {
    char requests[QUEUESIZE][PRINTSIZE];
    int head;
    int tail;
} PrintQueue;

sem_t *sem_user, *sem_printer, *sem_queue, *sem_requests;

PrintQueue* queue = NULL;

void random_string(char* str, int len) {
    for (int i = 0; i < len; ++i)
        str[i] = 'a' + rand() % 26;
}

void user(int id) {
    srand(getpid());
    while(1){
        sleep(1 + rand() % 5);
        char request[PRINTSIZE];
        random_string(request, PRINTSIZE);

        sem_wait(sem_queue);
        sem_wait(sem_user);

        memcpy(queue->requests[queue->tail], request, PRINTSIZE);
        printf("Print request : User %d", id);
        printf("\n");

        queue->tail = (queue->tail + 1) % QUEUESIZE;

        sem_post(sem_user);
        sem_post(sem_requests);
    }
}

void printer(int id) {
    while(1) {
        sem_wait(sem_requests);
        sem_wait(sem_printer);

        char request[PRINTSIZE];
        memcpy(request, queue->requests[queue->head], PRINTSIZE);
        queue->head = (queue->head + 1) % QUEUESIZE;

        sem_post(sem_printer);
        sem_post(sem_queue);

        printf("[PRINTER %d] Began printing:\n", id);
        for (int i = 0; i < PRINTSIZE; ++i) {
            printf("%c", request[i]);
            fflush(stdout);
            sleep(1);
        }
        printf("\n[PRINTER %d] Finished.\n", id);
    }
}

void cleanup(int sig) {
    sem_unlink(USERWRITE);
    sem_unlink(PRINTERREAD);
    sem_unlink(QUEUEFREE);
    sem_unlink(QUEUEREQUESTS);
    shm_unlink(SHM_NAME);
    printf("Removed POSIX IPC...\n");
    exit(0);
}

int main() {
    signal(SIGINT, cleanup);

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(PrintQueue));
    queue = mmap(NULL, sizeof(PrintQueue), PRO`T_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    queue->head = queue->tail = 0;

    sem_user = sem_open(USERWRITE, O_CREAT, 0666, 1);
    sem_printer = sem_open(PRINTERREAD, O_CREAT, 0666, 1);
    sem_queue  = sem_open(QUEUEFREE,  O_CREAT, 0666, QUEUESIZE);
    sem_requests = sem_open(QUEUEREQUESTS, O_CREAT, 0666, 0);

    for (int i = 0; i < MAX_USERS; ++i) {
        if (fork() == 0) {
            user(i);
            exit(0);
        }
    }

    for (int i = 0; i < MAX_PRINTERS; ++i) {
        if (fork() == 0) {
            printer(i);
            exit(0);
        }
    }

    while (wait(NULL) > 0);
    return 0;
}