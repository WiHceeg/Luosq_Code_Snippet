 
/*
       uint32_t htonl(uint32_t hostlong);

       uint16_t htons(uint16_t hostshort);

       uint32_t ntohl(uint32_t netlong);

       uint16_t ntohs(uint16_t netshort);
*/

#include <stdio.h>
#include <arpa/inet.h>


int main() {

    //htons
    unsigned short a = 0x0102;
    printf("a: %x\n", a);
    unsigned short b = htons(a);
    printf("b: %x\n", b);


    //htonl, 转换ip
    char buf[4] = {192, 168, 1, 100};
    int num = *(int *)buf;          //先转换成地址，再取值
    unsigned int res = htonl(num);
    unsigned char *p = (char *)&res;
    printf("%d %d %d %d\n", *p, *(p+1), *(p+2), *(p+3));


    //ntohl
    unsigned char buf1[4] = {1, 1, 168, 192};
    int num1 = *(int *)buf1;
    int res1 = ntohl(num1);
    unsigned char *p1 = (unsigned char *)&res1;
    printf("%d %d %d %d\n", *p1, *(p1+1), *(p1+2), *(p1+3));



    //ntohs


    return 0;
}
