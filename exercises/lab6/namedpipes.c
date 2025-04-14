#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(){
    char* path = "myfifo";
    if (mkfifo(path, 0666) == -1) {
        perror("mkfifo");
    }

    pid_t pid = fork();

    if(pid == 0){
        FILE* rd = fopen(path, "r");
        if (rd == NULL) {
            perror("fopen (read)");
            exit(1);
        }

        char message[100];
        fgets(message, sizeof(message), rd);
        printf("Odebrano: %s\n", message);
        fclose(rd);
    }
    else{
        sleep(1);
        FILE* wr = fopen(path, "w");
        if (wr == NULL) {
            perror("fopen (write)");
            exit(1);
        }

        fputs("To jest wiadomość...\n", wr);
        fclose(wr);
    }

    unlink(path);

    return 0;
}
