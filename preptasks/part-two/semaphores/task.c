#include <stdio.h>
#include <time.h>
#include <semaphore.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define ITEM_SEM "/items_semaphore"
#define ITEM_SHM "/item_shm"

#define PLAYERS 4
#define MAX_ITEMS 6
#define REFILL_TIME 8

sem_t *item_sem;
int item_shm;

int *items;

void cleanup(int signal) {
  sem_unlink(ITEM_SEM);
  sem_unlink(ITEM_SHM);
  exit(0);
}

void player(int id){
  while(1){
    sleep(1 + rand() % 400 / 100.0);
    sem_wait(item_sem);
    if(*items > 0) {
      (*items)--;
      printf("[Player %d]: Picked up an item! Items left %d\n", id, *items);
      fflush(stdout);
    }
    else {
      printf("[Player %d]: No more items!\n", id);
      fflush(stdout);
    }
    sem_post(item_sem);
  }
}

void shop() {
  while(1){
    sleep(REFILL_TIME);
    sem_wait(item_sem);
    if(*items <= 0){
      printf("[Shop]: No more items! Restocking\n");
      fflush(stdout);
      *items = 10;
    }
    else{
      printf("[Shop]: There are items in the shop!\n");
      fflush(stdout);
    }
    sem_post(item_sem);
  }
}

int main() {
  signal(SIGINT, cleanup);

  item_shm = shm_open(ITEM_SHM, O_CREAT | O_RDWR, 0666);
  if(item_shm == -1){
    perror("shm_open failed");
    exit(1);
  }

  if(ftruncate(item_shm, sizeof(int)) == -1){
    perror("ftruncate failed");
    exit(1);
  }

  items = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, item_shm, 0);
  if(items == MAP_FAILED){
    perror("mmap failed");
    exit(1);
  }
  *items = 10;

  item_sem = sem_open(ITEM_SEM, O_CREAT, 0666, 1);

  if(item_sem == SEM_FAILED){
    perror("sem_open failed");
    exit(1);
  }

  if(fork() == 0){
    shop();
    exit(0);
  }

  for(int id = 0; id < PLAYERS; id++){
    if(fork() == 0){
      srand(time(NULL) + 10 * id);
      player(id);
      exit(0);
    }
  }

  while(wait(NULL) > 0);
  return 0;
}