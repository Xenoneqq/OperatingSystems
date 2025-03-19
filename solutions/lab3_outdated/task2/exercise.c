#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>


int get_directory_size(){
  DIR *dir;
  dir = opendir("./testdir");
  if(dir == NULL){
    printf("Failed to open directory...\n");
    return -1;
  }
  struct dirent *entry;
  struct stat statbuff;
  long long total_size = 0;

  while((entry = readdir(dir)) != NULL){
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    if (fstatat(dirfd(dir), entry->d_name, &statbuff, 0) == 0) {
      if(S_ISDIR(statbuff.st_mode)) continue;
      //printf("File: %s (size: %ld bytes)\n", entry->d_name, statbuff.st_size);
      total_size += statbuff.st_size;
    }
  }

  closedir(dir);

  return total_size;
}

int get_full_directory_size(char* path){
  DIR *dir;
  dir = opendir(path);
  if(dir == NULL){
    printf("Failed to open directory...\n");
    return 0;
  }
  struct dirent *entry;
  struct stat statbuff;
  long long total_size = 0;

  while((entry = readdir(dir)) != NULL){
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    if (fstatat(dirfd(dir), entry->d_name, &statbuff, 0) == 0) {
      if(S_ISDIR(statbuff.st_mode)){
        char sub_path[2048];
        snprintf(sub_path, sizeof(sub_path), "%s/%s", path, entry->d_name);
        total_size += get_full_directory_size(sub_path);
      }
      else{
        //printf("File: %s (size: %ld bytes)\n", entry->d_name, statbuff.st_size);
        total_size += statbuff.st_size;
      }
    }
  }
  closedir(dir);

  return total_size;
}

int main(){
  long long bytesize = get_directory_size();
  printf("Directory has a total of %lli bytes\n", bytesize);

  bytesize = get_full_directory_size("./testdir");
  printf("Full Directory has a size of %lli bytes\n", bytesize);
  return 0;
}