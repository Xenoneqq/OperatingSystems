#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#define MAX_ITEMS 10
#define PLAYERS 3

int items = 5;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t shopkepper_wakeup = PTHREAD_COND_INITIALIZER;

void handler(int signal) {
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&shopkepper_wakeup);
  printf("Program exited safely!");
  exit(0);
}

void *player(void* arg){
  int id = *(int *)arg;
  while(1){
    sleep(rand()%4 + 2);
    pthread_mutex_lock(&mutex);
    if(items > 0) {
      printf("[Player %d]: Purchased an item! Items left %d\n", id, items);
      items--;
    }
    else {
      printf("[Player %d]: No items left waking up the shopkepper...\n", id);
      pthread_cond_signal(&shopkepper_wakeup);
    }
    pthread_mutex_unlock(&mutex);
  }
}

void *shopkeeper(void* arg) {
  while(1) {
    pthread_mutex_lock(&mutex);
    while(items > 0){
      pthread_cond_wait(&shopkepper_wakeup, &mutex);
    }
    printf("[Shop]: I woke up! Restocking...\n");
    sleep(3);
    items = 10;
    printf("[Shop]: Items restocked! Going back to sleep...\n");
    pthread_mutex_unlock(&mutex);
  }
}

int main() {
  srand(time(NULL));
  pthread_t players_threads[PLAYERS];
  int player_ids[PLAYERS];
  pthread_t shopkeeper_thread;

  signal(SIGINT, handler);

  pthread_create(&shopkeeper_thread, NULL, shopkeeper, NULL);
  for(int i = 0; i < PLAYERS; i++){
    player_ids[i] = i;
    pthread_create(&players_threads[i], NULL, player, &player_ids[i]);
  }

  for(int i = 0; i < PLAYERS; i++){
    pthread_join(players_threads[i], NULL);
  }
  pthread_join(shopkeeper_thread, NULL);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&shopkepper_wakeup);
  return 0;
}