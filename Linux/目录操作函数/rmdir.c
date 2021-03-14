#include <unistd.h>
#include <stdio.h>

    //    int rmdir(const char *pathname);

int main() {

    int ret = rmdir("aaa");

    if (ret == -1) {
        perror("rmdir");
        return -1;
    }


    return 0;
}