#include <unistd.h>
#include <stdio.h>

int main() {
    
    int ret = access("a.txt", F_OK);
    
    if (ret == -1) {
        perror("access");
        return -1;
    }

    printf("文件存在！\n");

    return 0;
}