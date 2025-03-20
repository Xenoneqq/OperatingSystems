#include<stdio.h>
#include <stdlib.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>

void make_path(char* out, char* path , char* file){
  int i = 0;
  int pathlen = strlen(path);
  int namelen = strlen(file);
  while(i < pathlen){
    out[i] = path[i];
    i++;
  }
  out[i] = '/'; i++;
  int i2 = 0;
  while(i2 < namelen){
    out[i] = file[i2];
    i2++; i++;
  }
  out[i] = '\0';
}

int file_read(char* original, char* copy){
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
  
  int length, i;
  char block[1024];
  while(fgets(block , sizeof(block), original_file) != NULL){
    length = strlen(block);
    for(i = length - 1; i >= 0; i--){
      if(block[i] == '\n') continue;
      fputc(block[i] , result_file);
    }
    fputc('\n', result_file);
  }

  fclose(original_file);
  fclose(result_file);
  printf("Task Done!\n");
  return 0;
}

int read_directory(char* open, char* clone){
  DIR *dir;
  dir = opendir(open);
  if(dir == NULL){
    printf("Failed to open directory...\n");
    return -1;
  }

  #ifndef _WIN32
  mkdir(clone, 0777 );
  #else
  mkdir(clone);
  #endif

  struct dirent *entry;
  struct stat statbuff;

  while((entry = readdir(dir)) != NULL){
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    if (fstatat(dirfd(dir), entry->d_name, &statbuff, 0) == 0) {
      char* name = entry->d_name;
      printf("%s\n",name);
      int length = strlen(name);
      char type[5];
      for(int i = 0; i < 4; i++){
        type[3-i] = name[length - i - 1];
        type[4] = '\0';
      }
      if(strcmp(type , ".txt") == 0){
        char* og_path = malloc(sizeof(char) * (strlen(open) + length + 2));
        make_path(og_path , open , name);

        char* clone_path = malloc(sizeof(char) * (strlen(clone) + length + 2));
        make_path(clone_path , clone , name);
        file_read(og_path , clone_path);
        printf("%s\n" , og_path);
      }
    }
  }

  closedir(dir);
  return 0;
}

int main(){
  read_directory("./art" , "./res");
  return 0;
}