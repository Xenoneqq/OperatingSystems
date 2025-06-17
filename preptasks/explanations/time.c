#include <time.h>

int main(){
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char buf[100];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local);
}