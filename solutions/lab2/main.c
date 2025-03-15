#include "collatz.h"
#include "stdlib.h"
#include "stdio.h"

int static_test(){
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
}

int main(){
  static_test();
  return 0;
}