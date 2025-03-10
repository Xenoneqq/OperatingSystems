#include <stdio.h>
int main(void){
  int n1, n2;
  printf("Prosze podac liczbe 1 : ");
  scanf("%d" , n1);
  printf("Prosze podac liczbe 2 : ");
  scanf("%d" , n2);
  int res = n1 * n2;
  printf("Wynik : %d" , res);
  return 0;
}