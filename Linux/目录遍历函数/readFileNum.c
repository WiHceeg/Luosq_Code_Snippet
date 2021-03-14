#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
DIR *opendir(const char *name);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);
*/



//递归函数，用于获取目录下所有普通文件的个数
int getFileNum(const char *path);

// 读取某个目录下所有普通文件的个数
int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("正确用法: %s path\n", argv[0]);
        return -1;
    }

    int num = getFileNum(argv[1]);
    printf("普通文件的个数为: %d\n", num);

    return 0;
}

int getFileNum(const char *path) {

    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(0);
    }

    struct dirent *ptr;

    //记录普通文件的个数，最后返回
    int total = 0;

    while ((ptr = readdir(dir)) != NULL) {

        //获取名称
        char *dname = ptr->d_name;

        //要忽略. ..
        if (strcmp(dname, ".") == 0 || strcmp(dname, "..") == 0) {
            continue;
        }

        //判断是普通文件还是目录
        if (ptr->d_type == DT_DIR) {
            //目录，需要继续读取这个目录
            char newpath[256];
            sprintf(newpath, "%s/%s", path, dname);
            total += getFileNum(newpath);
        }

        if (ptr->d_type == DT_REG) {
            //普通文件，统计个数
            total++;
        }

    }
    
    closedir(dir);

    return total;
}
