#include <stdio.h>

int main(){
    FILE* f = fopen("./file1.txt" , "r");
    if(f == NULL){
        perror("Failed to open the file");
        return 1;
    }
    char buffer[1024];
    while(fgets(buffer, sizeof(buffer), f)){
        printf("%s", buffer);
    }
    fclose(f);
    printf("\n");
    return 0;
}