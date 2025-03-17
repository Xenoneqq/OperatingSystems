#define _POSIX_C_SOURCE 199309L  // Wymusza wsparcie POSIX
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

//compile with -lrt
int by_character(char* original, char* copy){
  FILE* original_file = fopen(original , "r");

  if(original_file == NULL){
    printf("Failed to open the file...\n");
    return -1;
  }

  
  FILE* result_file = fopen(copy , "w");
  if(result_file == NULL){
    printf("Failed to create the file...\n");
    return -1;
  }
  

  fseek(original_file , 0 , SEEK_END);
  long pos = ftell(original_file);
  int last_inline = 0;
  while(pos > 0){
    pos--;
    fseek(original_file , pos, SEEK_SET);
    int ch = fgetc(original_file);

    if(ch == '\n') last_inline = 1;
    else last_inline = 0;
    if(last_inline == 0) fputc(ch , result_file);
  }

  fclose(original_file);
  fclose(result_file);
  printf("Task Done!\n");
  return 0;
}

int main( int argc , char* argv[] ){
  printf("Reading file character by character.\n");
  if(argc < 3){
    printf("No files specified...\n");
    return -1;
  }
  char* original = argv[1];
  char* copy = argv[2];

  #ifndef _WIN32
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  #endif

  by_character(original , copy);
  #ifdef _WIN32
  printf("time in nanoseconds only aviable on linux version of the program!\n");
  #endif

  #ifndef _WIN32
  clock_gettime(CLOCK_MONOTONIC, &end);
  #endif

  #ifndef _WIN32
  long seconds = end.tv_sec - start.tv_sec;
  long nanoseconds = end.tv_nsec - start.tv_nsec;
  if (nanoseconds < 0) { 
      seconds--;
      nanoseconds += 1000000000;
  }
  double ms = seconds * 1000 + nanoseconds / 1000000.0;
  double us = seconds * 1000000 + nanoseconds / 1000.0;
  double ns = seconds * 1000000000 + nanoseconds;

  printf("Czas wykonania: %.3f ms (%.0f µs, %.0f ns)\n", ms, us, ns);
  #endif
  return 0;
}
