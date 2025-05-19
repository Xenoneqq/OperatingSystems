#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define START_POINT 0
#define END_POINT 1
#define CALC_THREADS 100000

float f(float x) {
    return 4 / (x * x + 1);
}

typedef struct {
    float step;
    float *result;
    int *ready;
    int index;
} ThreadArgs;

void *calculate_square(void *arg){
    ThreadArgs *args = (ThreadArgs *)arg;

    args->result[args->index] = f(args->index * args->step) * args->step;
    args->ready[args->index] = 1;

    pthread_exit(NULL);
}

int main(){
    int ready[CALC_THREADS] = {0};
    float results[CALC_THREADS] = {0.0};
    pthread_t threads[CALC_THREADS];
    ThreadArgs *args = malloc(sizeof(ThreadArgs) * CALC_THREADS);

    float step = (END_POINT - START_POINT) / (float)CALC_THREADS;

    for(int i = 0; i < CALC_THREADS; i++){
        args[i].step = step;
        args[i].result = results;
        args[i].ready = ready;
        args[i].index = i;

        if (pthread_create(&threads[i], NULL, calculate_square, (void *)&args[i]) != 0) {
            perror("pthread_create did not create thread");
            return 1;
        }
    }

    float result = 0;
    for(int i = 0; i < CALC_THREADS; i++){
        while(ready[i] == 0)
            usleep(100);
        result += results[i];
    }

    printf("Integral is %f\n", result);

    free(args);

    return 0;
}
