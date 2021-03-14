#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

int main() {


    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    int ret;
    
    bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));

    listen(lfd, 8);


    int epfd = epoll_create(1);

    //将要监听的文件描述符相关的检测信息添加到epoll实例中
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    struct epoll_event epevs[1024];

    while(1) {

        ret = epoll_wait(epfd, epevs, 1024, -1);
        if (ret == -1) {
            perror("epoll_wait");
            exit(-1);
        }

        // printf("ret = %d\n", ret);

        for (int i = 0; i < ret; i++) {

            int curfd = epevs[i].data.fd;
            
            if (curfd == lfd) {
                //监听的文件描述符有数据到达，有客户端连接进来

                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);

                //设置cfd属性非阻塞
                int flag = fcntl(cfd, F_GETFL);
                flag |= O_NONBLOCK;
                fcntl(cfd, F_SETFL, flag);

                epev.events = EPOLLIN | EPOLLET;      //重用epev, 设置边沿触发, 没读完就不会通知了，所以需要一次性读完
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            } else {
                //有数据到达，需要通信
                //这里需要注意的是，如果关注了很多事件（比如EPOLLIN | EPOLLOUT），那么需要对每一种分情况处理
                

                //循环读取所有数据
                char buf[5];
                int len = 0;
                while ( (len = read(curfd, buf, sizeof(buf))) > 0) {
                    //打印数据
                    // printf("recv data: %s\n", buf);
                    write(STDOUT_FILENO, buf, len);
                    write(curfd, buf, len);
                }
                if (len == 0) {
                    printf("client closed...\n");
                    // epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    // close(curfd);
                } else if (len == -1) {
                    if (errno == EAGAIN) {
                        printf("data over...\n");
                    } else {
                        perror("read");
                        exit(-1);
                    }
                    
                }

            }
        }

    }

    close(lfd);
    close(epfd);

    return 0;

}
