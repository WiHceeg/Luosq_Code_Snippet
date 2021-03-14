#include <stdio.h>
#include <unistd.h>

int main() {

    int seconds = alarm(10);
    printf("seconds = %d\n", seconds);  //0

    sleep(2);
    seconds = alarm(2);
    printf("seconds = %d\n", seconds);  //8

    while(1) {

    }

    return 0;
}
       
