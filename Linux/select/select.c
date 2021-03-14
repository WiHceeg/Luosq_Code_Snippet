#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/select.h>


int main() {

    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    int ret;
    
    bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));

    listen(lfd, 8);

    //创建一个fd_set集合
    fd_set rdset, tmp;
    FD_ZERO(&rdset);
    FD_SET(lfd, &rdset);
    int maxfd = lfd;

    while (1) {
        
        tmp = rdset;

        //调用select，让内核帮检测哪些文件描述符有数据
        ret = select(maxfd + 1, &tmp, NULL, NULL, NULL);
        if (ret == -1) {
            perror("select");
            exit(-1);
        } else if (ret == 0) {
            continue;
        } else if (ret > 0) {
            
            if (FD_ISSET(lfd, &tmp)) {
                //监听描述符对应的标志位是1, 表示有新的客户端连接进来了
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);

                // 将新的文件描述符加入到集合中
                FD_SET(cfd, &rdset);
                maxfd = maxfd > cfd ? maxfd : cfd;
            }

            for (int i = lfd + 1; i <= maxfd; i++) {
                if (FD_ISSET(i, &tmp)) {
                    //说明这个文件描述符对应的客户端发来了数据
                    char buf[1024];
                    int len = read(i, buf, sizeof(buf));
                    if (len == -1) {
                        perror("read");
                        exit(-1);
                    } else if (len == 0) {
                        printf("client closed...\n");
                        FD_CLR(i, &rdset);
                    } else if (len > 0) {
                        printf("read buf: %s\n", buf);
                        write(i, buf, strlen(buf) + 1);
                    }
                }
            }

        }



    }

    close(lfd);


    return 0;
}
