#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

char* PIPE = "localPipe";

double func(float x){
    return 4 / (x*x + 1);
}

double calculate(double start , double end){
    if(start > end){
        double t = start;
        start = end;
        end = t;
    }
    double width = end - start;
    double step = width / 10000;
    double sum = 0;
    for(int i = 0; i < 10000; i++){
        sum += func(i * step) * step;
    }
    return sum;
}

int main(){
    int rd = open(PIPE , O_RDONLY);
    if(rd < 0){
        perror("Failed to open FIFO (read)");
        return 1;
    }

    size_t bytesize;
    double start, end;

    bytesize = read(rd, &start, sizeof(double));
    if(bytesize < 0){
        perror("Failed to read FIFO (start)");
        return 2;
    }
    bytesize = read(rd, &end, sizeof(double));
    if(bytesize < 0){
        perror("Failed to read FIFO (end)");
        return 3;
    }

    close(rd);
    double res = calculate(start, end);

    int wr = open(PIPE , O_WRONLY);
    if(wr < 0){
        perror("Failed to open FIFO (write)");
        return 4;
    }

    if(write(wr , &res, sizeof(double)) == -1){
        perror("Failed to write to FIFO");
        return 5;
    }
    close(wr);
    return 0;
}