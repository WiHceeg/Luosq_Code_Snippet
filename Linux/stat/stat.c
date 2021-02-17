/*
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int stat(const char *pathname, struct stat *statbuf);

int lstat(const char *pathname, struct stat *statbuf);
*/
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {

    struct stat statbuf;
    
    int ret = stat("a.txt", &statbuf);

    if (ret == -1) {
        perror("stat");
        return -1;
    }

    printf("size: %ld\n", statbuf.st_size);

    return 0;
}