#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define SEM_NAME "/example_sem"
#define SHM_NAME "/example_shm"

int main() {
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(int));
    int *counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    *counter = 0;

    if(fork() == 0) {
        for(int i = 0; i < 5; i++) {
            sem_wait(sem);
            (*counter)++;
            printf("Child: %d\n", *counter);
            sem_post(sem);
            sleep(1);
        }
        return 0;
    }

    for(int i = 0; i < 5; i++) {
        sem_wait(sem);
        (*counter)++;
        printf("Parent: %d\n", *counter);
        sem_post(sem);
        sleep(1);
    }

    wait(NULL);

    munmap(counter, sizeof(int));
    close(shm_fd);
    shm_unlink(SHM_NAME);
    sem_close(sem);
    sem_unlink(SEM_NAME);

    return 0;
}
