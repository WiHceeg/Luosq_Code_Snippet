#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//    int mkfifo(const char *pathname, mode_t mode);

int main() {

    // 判断管道文件是否存在
    int ret = access("fifo1", F_OK);
    
    if (ret == -1) {
        printf("管道不存在，创建管道\n");


        int ret = mkfifo("fifo1", 0664);

        if (ret == -1) {
            perror("mkfifo");
            exit(0);
        }

    }

    return 0;
}
