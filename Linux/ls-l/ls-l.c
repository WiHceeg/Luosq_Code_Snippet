#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <pwd.h>
#include <grp.h>
#include <time.h>

#include <string.h>

//模拟实现ls -l指令
// ls -l a.txt
// -rw-r--r-- 1 root root 12 Feb 16 20:25 a.txt


int main(int argc, char *argv[]) {
    
    //判断输入的参数是否正确
    if (argc < 2) {
        printf("正确用法：\n%s filename\n", argv[0]);
        return -1;
    }

    struct stat st;
    int ret = stat(argv[1], &st);
    if (ret == -1) {
        perror("stat");
        return -1;
    }
    
    //获取文件类型和权限
    char perms[11] = {0};   //保存文件类型和权限
    switch(st.st_mode & S_IFMT) {
        case S_IFLNK:       //符号链接
            perms[0] = '1';
            break;
        case S_IFDIR:       //目录
            perms[0] = 'd';
            break;
        case S_IFREG:       //普通文件
            perms[0] = '-';
            break;
        case S_IFBLK:       //块设备
            perms[0] = 'b';
            break;
        case S_IFCHR:       //字符设备
            perms[0] = 'c';
            break;
        case S_IFSOCK:       //套接字文件
            perms[0] = 's';
            break;
        case S_IFIFO:       //管道文件
            perms[0] = 'p';
            break;
        default:            //未知文件
            perms[0] = '?';
            break;
    }

    //判断文件的访问权限
    
    //文件所有者
    perms[1] = st.st_mode & S_IRUSR ? 'r' : '-';
    perms[2] = st.st_mode & S_IWUSR ? 'w' : '-';
    perms[3] = st.st_mode & S_IXUSR ? 'x' : '-';

    //文件所在组
    perms[4] = st.st_mode & S_IRGRP ? 'r' : '-';
    perms[5] = st.st_mode & S_IWGRP ? 'w' : '-';
    perms[6] = st.st_mode & S_IXGRP ? 'x' : '-';

    //其他人
    perms[7] = st.st_mode & S_IROTH ? 'r' : '-';
    perms[8] = st.st_mode & S_IWOTH ? 'w' : '-';
    perms[9] = st.st_mode & S_IXOTH ? 'x' : '-';

    //硬连接数
    int linkNum = st.st_nlink;

    //文件所有者
    char *fileUser = getpwuid(st.st_uid)->pw_name;

    //文件所在组
    char *fileGrp = getgrgid(st.st_gid)->gr_name;

    //文件大小
    long int fileSize = st.st_size;

    //获取修改的时间
    char *time = ctime(&st.st_mtime);

    //去掉换行
    char mtime[512] = {0};
    strncpy(mtime, time, strlen(time) - 1);

    char buf[1024];

    // ls -l a.txt
    // -rw-r--r-- 1 root root 12 Feb 16 20:25 a.txt

    sprintf(buf, "%s %d %s %s %ld %s %s", perms, linkNum, fileUser, fileGrp, fileSize, mtime, argv[1]);

    printf("%s\n", buf);

    return 0;
}