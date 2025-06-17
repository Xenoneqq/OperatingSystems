#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

int items = 5;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mutex_wakeup = PTHREAD_COND_INITIALIZER;

void handler(int signal) {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&mutex_wakeup);
    exit(0);
}

void *call_func(void *arg){
    while(1) {
        pthread_mutex_lock(&mutex);
        while(items > 0) pthread_cond_wait(&mutex_wakeup, &mutex);
        items = 10;
        pthread_mutex_unlock(&mutex);
    }
}

void *thread_func(void* arg) {
    int id = *(int *)arg;
    pthread_mutex_lock(&mutex);
    if(items <= 0) pthread_cond_signal(&mutex_wakeup);
    pthread_mutex_unlock(&mutex);
}

int main(){
  p_thread thread, call;
  pthread_create(&thread, NULL, thread_func, NULL); //&var
  pthread_create(&call, NULL, call_func, NULL); //&var
  pthread_join(call, NULL);
  pthread_join(thread, NULL);
}