#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

// int mkdir(const char *pathname, mode_t mode);

int main() {

    int ret = mkdir("aaa", 0777);

    if (ret == -1) {
        perror("mkdir");
        return -1;
    }

    return 0;
}
