#include <stdio.h>
#include <unistd.h>

//1秒钟能数多少个数

//直接运行和重定向到txt文件，能数的数差距很大

int main() {


    alarm(1);

    int i = 0;
    while(1) {
        printf("%d\n", ++i);
    }
    return 0;
}
