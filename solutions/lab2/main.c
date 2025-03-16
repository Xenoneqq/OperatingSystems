#include "collatz.h"
#include "stdlib.h"
#include "stdio.h"

#ifdef USE_DYNAMIC
#include <dlfcn.h>
#endif

int static_test(){
  #ifndef USE_DYNAMIC
  int input;
  printf("Write number to test: ");
  scanf("%d", &input);
  int max_iter = 1000;
  int *steps = malloc(max_iter * sizeof(int));
  if(steps == NULL){
    printf("memory allocation error...");
    return -1;
  }
  int length = test_collatz_convergence(input, max_iter, steps);
  
  printf("Length is : %d\n", length);
  if (length > 0) {
    printf("Collatz sequence:\n");
    for (int i = 0; i < length; i++) {
        printf("%d ", steps[i]);
    }
    printf("\n");
  }

  free(steps);
  return 0;
  #else
  printf("no static library found...");
  #endif
}

int dynamic_test(){
  #ifdef USE_DYNAMIC
  void *handle = dlopen("libcollatz.so" , RTLD_LAZY);
  if(!handle){
    printf("error importing library...\n");
    return -1;
  }

  int (*test_collatz_convergence)(int, int, int *);
  test_collatz_convergence = dlsym(handle, "test_collatz_convergence");
  if(dlerror() != NULL) {
    printf("error importing function");
    dlclose(handle);
    return -1;
  }

  int input;
  printf("Write number to test: ");
  scanf("%d", &input);
  int max_iter = 1000;
  int *steps = malloc(max_iter * sizeof(int));
  if(steps == NULL){
    printf("memory allocation error...");
    return -1;
  }
  int length = test_collatz_convergence(input, max_iter, steps);
  
  printf("Length is : %d\n", length);
  if (length > 0) {
    printf("Collatz sequence:\n");
    for (int i = 0; i < length; i++) {
        printf("%d ", steps[i]);
    }
    printf("\n");
  }

  free(steps);
  dlclose(handle);
  return 0;
  #else
  printf("No dynamic libraries!");
  return -1;
  #endif
}

int main(){
  #ifdef USE_DYNAMIC
  dynamic_test();
  #else
  static_test();
  #endif
  return 0;
}