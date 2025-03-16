#include "collatz.h"

int collatz_conjecture(int input){
  if(input % 2 == 0) input /= 2;
    else input = input * 3 + 1;
  return input;
}

int test_collatz_convergence(int input, int max_iter, int *steps){
  if(input <= 0 || max_iter <= 0) return 0;

  int step = 0;
  while(input != 1 && step < max_iter){
    steps[step] = input;
    step++;
    input = collatz_conjecture(input);
  }
  if(input == 1){
    steps[step] = 1;
    step++;
  }
  else{
    return 0;
  }

  return step;
}