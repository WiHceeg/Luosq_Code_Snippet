#include <unistd.h>
#include <stdio.h>

int main() {

    int ret = chown("a.txt", 1000, 1000);
    if (ret == -1) {
        perror("chown");
        return -1;
    }

    return 0;
}