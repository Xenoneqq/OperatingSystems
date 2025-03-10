#include <stdio.h>
int main(void){
  int n1, n2;
  printf("Input the first number : ");
  scanf("%d" , n1);
  printf("Input the second number : ");
  scanf("%d" , n2);
  int res = n1 * n2;
  printf("Result : %d" , res);
  return 0;
}