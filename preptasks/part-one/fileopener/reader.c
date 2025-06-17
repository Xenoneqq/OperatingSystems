#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int wr = open("file1.txt", O_RDONLY);
    if(wr < 0){
        perror("Failed to open FILE");
        return 1;
    }
    char message[1024];
    size_t bytesread;
    while((bytesread = read(wr, message, sizeof(message)) > 0)){
        printf("%s",message);
    }
    printf("\n");
    close(wr);
}