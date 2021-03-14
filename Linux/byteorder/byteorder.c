/*
    小端：数据高位在内存高位
    大端：数据地位在内存高位
*/
#include <stdio.h>

// 当前计算机是哪种字节序

int main() {

    union {
        short value;
        char bytes[sizeof(short)];
    } test;
    
    test.value =0x0102;
    if ((test.bytes[0] == 1) && (test.bytes[1] == 2)) {
        printf("大端字节序\n");
    } else if ((test.bytes[0] == 2) && (test.bytes[1] == 1)) {
        printf("小端字节序\n");
    } else {
        printf("未知\n");
    }


    return 0;
} 
