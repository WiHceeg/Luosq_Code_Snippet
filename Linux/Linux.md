# 系统编程入门

gcc问题。



gcc `-g`是什么意思？`-D`怎么用？ 

答：`-g`是给 GDB 调试用，`-D`是用来定义宏的，比如`-D DEBUG`



制作静态库用什么命令？制作动态库用什么命令？

答：静态库打包用`ar rcs libxxx.a xxx.o xxx.o`; 动态库打包`gcc -c -fpic a.c b.c`然后`gcc -shared a.o b.o -o libxxx.so`

r 将文件插入备存文件中

c 建立备份文件

s 索引



文件I/O

输入：文件里的信息加载到内存里

输出：内存里的数据写到文件里

```c
int open(const char *pathname, int flags);		//打开一个存在的文件
int open(const char *pathname, int flags, mode_t mode);	//创建一个新文件
//返回文件描述符
//flag O_RDONLY, OWRONLY, O_APPEND, O_CREAT等，用 | 表示都要
//mode八进制的数，表示创建的新文件的操作权限

int close(int fd);

void perror(const char *s);
//打印errno对应的错误描述

ssize_t read(int fd, void *buf, size_t count);
//fd：通过open得到的文件描述符
//buf：传出参数，需要读取数据存放的地方，数组的地址
//count：指定的数组的大小
//返回值：读取到的字节的数量，=0表示文件读取完，-1表示失败
ssize_t write(int fd, const void *buf, size_t count);
//fd：通过open得到的文件描述符
//buf：要往磁盘写入的数据
//count：要写的数据的实际的大小

off_t lseek(int fd, off_t offset, int whence);
//用于改变读写一个文件时读写指针位置
//fd 表示要操作的文件描述符
//offset是相对于whence（基准）的偏移量
//whence 可以是SEEK_SET（文件指针开始），SEEK_CUR（文件指针当前位置） ，SEEK_END为文件指针尾
//返回值：文件读写指针距文件开头的字节大小，出错，返回-1
//lseek 主要作用是移动文件读写指针，因此还有以下两个作用
//1.拓展文件，不过一定要一次写的操作。迅雷等下载工具在下载文件时候先扩展一个空间，然后再下载的。
//2.获取文件大小

int stat(const char *pathname, struct stat *statbuf);
int fstat(int fd, struct stat *statbuf);
int lstat(const char *pathname, struct stat *statbuf);
//statbuf是传出参数，stat结构体保存了很多文件的信息，文件类型，权限等

```



文件属性操作函数

```c
int access(const char *pathname, int mode);
//用来判断某个文件是否有某个权限，或者判断文件是否存在
//mode: R_OK读权限，W_OK写权限, X_OK执行权限, F_OK文件是否存在

int chmod(const char *pathname, mode_t mode); 
int fchmod(int fd, mode_t mode);
//用来修改文件的权限
//mode：需要修改的权限值，八进制的数

int chown(const char *pathname, uid_t owner, gid_t group);
int fchown(int fd, uid_t owner, gid_t group);
int lchown(const char *pathname, uid_t owner, gid_t group);
//修改文件所有者，所在组

int truncate(const char *path, off_t length);
int ftruncate(int fd, off_t length);
//缩减或扩展文件的尺寸至指定的大小
```



目录操作函数

```c
int mkdir(const char *pathname, mode_t mode);
//创建目录
//mode: 权限

int rmdir(const char *pathname);
//删除目录

int rename(const char *oldpath, const char *newpath);
//改目录名

int chdir(const char *path);
//修改进程的工作目录

char *getcwd(char *buf, size_t size);
//获取当前的工作目录
//buf: 用来保存路径，指向一个车数组
//size: 数组的大小
//返回值，指向一块内存，数据就是第一个参数
```



目录遍历函数

```c
DIR *opendir(const char *name);
//打开一个目录
//返回DIR*类型，目录流结构体，错误返回NULL

struct dirent *readdir(DIR *dirp);
//读取目录中的数据
//dirp是opendir返回的结果
//dirent里办函目录进入点、开头至进入点的位移、d_name长度、d_name文件类型、文件名

int closedir(DIR *dirp);
//关闭目录
```



dup、dup2函数

```c
int dup(int oldfd);
//复制一个新的文件描述符指向同一文件，从空闲的文件描述符表中找一个最小的

int dup2(int oldfd, int newfd);
//重定向文件描述符，可以用来重定向输入输出
//调用成功后，newfd指向oldfd指向的文件
//返回值是newfd
```



fcntl函数

```c
int fcntl(int fd, int cmd, ... /* arg */ );
//对打开的文件描述符执行cmd
/*
5个功能
1. 复制一个现有的描述符(cmd=F_DUPFD). 
2. 获得／设置文件描述符标记(cmd=F_GETFD或F_SETFD). 
3. 获得／设置文件状态标记(cmd=F_GETFL或F_SETFL). F_SETFL必选项设置读写，可选项有追加数据，设置成非阻塞
4. 获得／设置异步I/O所有权(cmd=F_GETOWN或F_SETOWN). 
5. 获得／设置记录锁(cmd=F_GETLK , F_SETLK或F_SETLKW).
*/
```

# 进程

5个状态：新建，就绪，运行，阻塞，终止

命令`ps aux`, `ps ajx`

a 显示终端上的所有进程，包括其他用户的 

u 详细信息

x 显示没有控制终端的进程

j 列出与作业控制相关的讯息



命令`top` 实时显示进程动态，`-d`设置时间间隔

获取进程id

```c
pid_t getpid(void);

pid_t getppid(void);
// 获取父进程的pid

pid_t getpgid(pid_t pid);
//获取进程pid所在的进程组的pgid,如果pid实参为0,则返回本进程所在的进程组的pgid。
//返回值:成功返回pgid,失败返回-1并设置errno。
```

进程创建

```c
pid_t fork(void);
//创建子进程
//返回两次。父进程中返回创建的直径的的PID
//        子进程中返回0
//用返回值区分是父进程还是子进程，父进程中返回-1表示子进程创建失败（可能是进程数到上限或者内存不足），并设置errno
```

fork之后，子进程用户区数据和父进程一样，内核区也会拷贝过来，但内核区的pid不同（废话）。

实际上，fork()是写时拷贝，读时共享



GDB多进程调试

默认调试父进程，子进程不会停，会一直往下执行

要改用`set follow-fork-mode parent/child`

查看当前调试哪个用`show`

设置调试模式`set detach-on-fork on/off`，默认是on表示脱离，子进程不会停。如果设置为off，就会挂起

`info inferiors`查看，*表示当前正在调试的进程

`inferiors 编号`切换当前正在调试的进程，切换之后先按一个c，就可以调试这个进程了

`detach inferiors id`使某个进程脱离GDB调试

`remove`移除

`kill`杀掉



exec函数族

```c
//有7个函数，作用是工具指定的文件名找到可执行文件，并用它来取代调用进程的内容
//执行成功后不会返回，因为进程内容大都被新内容取代（用户区会被换掉），只留下pid等表面信息保持原样。调用失败返回-1并从原程序调用点继续执行

int execl(const char *path, const char *arg, ...
                /* (char  *) NULL */);
//path: 需要制定的执行文件的路径或名称
//arg: 执行可执行文件的参数列表。
//		第一个参数没什么用，为了方便些可执行文件的名称，参数最后以NULL结束
//返回值：只有出错是返回-1，改errno。调用成功没有返回值

int execlp(const char *file, const char *arg, ...
                /* (char  *) NULL */);
//回到环境变量中查找指定的可执行文件，找到了就执行，找不到就执行不成功

int execle(const char *path, const char *arg, ...
                /*, (char *) NULL, char * const envp[] */);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[],
                char *const envp[]);

/*
函数族末尾含义
l (list)              命令行参数列表
p (path)              搜索file时使用path变量
v (vector)            使用命令行参数数组
e (environment)       使用环境变量数组,不使用进程原有的环境变量，设置新加载程序运行的环境变量
*/
```



进程退出

```c
void _exit(int status);		//Linux系统函数
void exit(int status);		//标准C库函数

//exit()函数与_exit()函数最大的区别就在于exit()函数在调用exit系统调用之前要检查文件的打开情况，把文件缓冲区中的内容写回文件，就是"清理I/O缓冲"。 
```



孤儿进程：父进程已结束的子进程，出现时内核把其父进程设置成init，init会循环地wait()它的已经退出的子进程。孤儿进程没什么危害。
僵尸进程：每个进程结束后，都会释放自己地址空间的用户区数据，内核区的PCB没办法释放掉，粗腰父进程释放掉。而父进程并没有调用wait或waitpid获取子进程的状态信息，那么子进程的进程描述符仍然保存在系统中。这种进程称之为僵死进程。如果进程不调用wait / waitpid的话， 那么保留的那段信息就不会释放，其进程号就会一直被占用，但是系统所能使用的进程号是有限的，如果大量的产生僵死进程，将因为没有可用的进程号而导致系统不能产生新的进程.



wait, waitpid

```c
pid_t wait(int *wstatus);
//wait()函数用于使父进程（也就是调用wait()的进程）阻塞，直到一个子进程结束或者该进程接收到了一个指定的信号为止。如果该父进程没有子进程或者它的子进程已经结束，则wait()函数就会立即返回。
//参数：输出型参数，获取子进程退出状态，不关心则可以设置为NULL
//    成功：返回被等待进程（子进程）pid
//    失败：返回-1(没子进程了，子进程都结束了)

//退出信息相关的宏函数
/*
            if(WIFEXITED(st)) {
                //正常退出
                printf("退出的状态码: %d\n", WEXITSTATUS(st));
            }
            if(WIFSIGNALED(st)) {
                //异常终止
                printf("被哪个信号干掉了: %d\n", WTERMSIG(st));
            }

*/

//waitpid()的作用和wait()一样，但它并不一定要等待第一个终止的子进程（它可以指定需要等待终止的子进程），它还有若干选项，如可提供一个非阻塞版本的 wait()功能，也能支持作业控制。实际上，wait()函数只是 waitpid()函数的一个特例，在Linux 内部实现 wait()函数时直接调用的就是waitpid()函数。

pid_t waitpid(pid_t pid, int *wstatus, int options);
//pid的4种情况
//1.pid==-1 等待任意子进程，相当于wait
//2.pid>0 等待进程ID与pid相等的子进程
//3.pid==0 等待组ID等于调用进程组ID的任意子进程
//4.pid<-1 等待组ID等于pid绝对值的任意子进程
//options: 0设置阻塞
       //WNOHANG     非阻塞
       //还有WUNTRACED WCONTINUED
//返回值：
//成功	成功结束运行的子进程的进程号
//失败	返回-1
//WNOHANG	没有子进程退出返回0
```



进程间通信

同一主机：Unix进程间通信方式有匿名管道、有名管道、信号。systemV和POSIX进程间通信方式有消息队列、共享内存、信号量。

不同主机：socket



匿名管道

管道是内核内存中维护的缓冲器，大小有限。

管道有文件特质：读写操作。匿名管道无文件实体，有名管道有。

单向半双工。

匿名管道只能在有亲缘关系的进程间使用。

```c
int pipe(int pipefd[2]);
//创建一个匿名管道
//pipefd[2]是传出参数，pipefd[0]对应的是读端，pipefd[1]对应的是写端
//管道默认是阻塞的。如果管道中没数据，read阻塞；如果管道满了，write阻塞

>> ulimit -a看管道大小
>> ulimit -p可以修改
    
// 设置管道非阻塞
int flags = fcntl(pipefd[0], F_GETFL);  //获取原来的flag
flags |= O_NONBLOCK;                    //修改flag的值
fcntl(pipefd[0], F_SETFL, flags);       // 设置新的flag
```



有名管道（FIFO）

1. FIFO在文件系统中作为一个特殊文件存在，但FIFO的内容却放在内存里
2. 使用FIFO的进程退出后，FIFO文件继续保留在文件系统中以便以后使用
3. FIFO有名字，不相关的进程可以打开进行通信

```c
int mkfifo(const char *pathname, mode_t mode);
//pathname：管道文件的路径
//mode：文件的权限，和open是一样的，八进制数
//返回值：成功0，失败-1并设置errno
```

   

内存映射

将磁盘文件的数据映射到内存，可以只映射一部分

```c
void *mmap(void *addr, size_t length, int prot, int flags,
            int fd, off_t offset);
/*
addr
这个参数是建议地址（hint），没有特别需求一般设为0。这个函数会返回一个实际 map 的地址。

len
文件长度。

prot
表明对这块内存的保护方式，不可与文件访问方式冲突。权限应 <= open权限
PROT_NONE
无权限，基本没有用
PROT_READ
读权限, 必须要有
PROT_WRITE
写权限
PROT_EXEC
执行权限

flags
描述了映射的类型。
MAP_FIXED
开启这个选项，则 addr 参数指定的地址是作为必须而不是建议。如果由于空间不足等问题无法映射则调用失败。不建议使用。
MAP_PRIVATE
表明这个映射不是共享的。文件使用 copy on write 机制映射，任何内存中的改动并不反映到文件之中。也不反映到其他映射了这个文件的进程之中。如果只需要读取某个文件而不改变文件内容，可以使用这种模式。
MAP_SHARED
和其他进程共享这个文件。往内存中写入相当于往文件中写入。会影响映射了这个文件的其他进程。与 MAP_PRIVATE冲突。
...

fd
文件描述符。进行 map 之后，文件的引用计数会增加。因此，我们可以在 map 结束后关闭 fd，进程仍然可以访问它。当我们 unmap 或者结束进程，引用计数会减少。

offset
文件偏移，从文件起始算起。必须是4k的整数倍，不然会报错

如果失败，mmap 函数将返回 MAP_FAILED。
*/

int munmap(void *addr, size_t length);
//用来取消参数start 所指的映射内存起始地址，参数length 则是欲取消的内存大小。当进程结束或利用exec 相关函数来执行其他程序时，映射内存会自动解除，但关闭对应的文件描述词时不会解除映射。
```

使用内存映射实现进程间通信：

1. 有关系的进程（父子进程），可以使用匿名映射（不需要真实的文件，flag加上MAP_ANONYMOUS，fd设置为-1）。父进程创建内存映射区，再创建子进程。
2. 没有关系的进程间通信，要准备一个大小不为0的磁盘。两个进程都通过磁盘文件创建内存映射。
3. 通信是非阻塞的。



信号

信号是Unix、类Unix以及其他POSIX兼容的操作系统中进程间通讯的一种有限制的方式。它是一种异步的通知机制，用来提醒进程一个事件已经发生。有时也称为软件中断。

信号通常来源于内核，各类事件如下：

1. 前台进程，如暗下Ctrl+C
2. 硬件发生异常。
3. 系统状态变化，如alarm定时器到期将引起SIFALARM信号
4. 运行kill命令或调用kill函数

查看系统定义的信号命令:
`kill - l`
`man 7 signal`

不可靠信号： 也称为非实时信号，不支持排队，信号可能会丢失, 比如发送多次相同的信号, 进程只能收到一次. 信号值取值区间为1~31；
可靠信号： 也称为实时信号，支持排队, 信号不会丢失, 发多少次, 就可以收到多少次. 信号值取值区间为32~64

信号的5种默认处理动作：

Term: 终止进程
Ign: 当前进程忽略掉这个信号
Core: 终止进程，并生成一个Core文件
Stop:暂停当前进程
Cont:继续执行当前被暂停的进程

信号的3种状态：产生，未决，递达

core文件作用
写一个访问野指针的程序
`ulimit -c 1024`设置core文件的大小（默认是不会生成）
编译运行就会生成包含错误信息的core文件
`gdb a.out`
`core-file core`就可以看错误信息



kill, raise, abort函数

```c
int kill(pid_t pid, int sig);/*功能：给任何进程(组)发送任何信号sig1. pid大于零时，pid是信号欲送往的进程的标识。2. pid等于零时，信号将送往所有与调用kill()的那个进程属同一个使用组的进程。3. pid等于-1时，信号将送往所有调用进程有权给其发送信号的进程，除了进程1(init)。4. pid小于-1时，信号将送往以-pid为组标识的进程。sig：准备发送的信号代码，假如其值为零则没有任何信号送出，但是系统会执行错误检查，通常会利用sig值为零来检验某个进程是否仍在执行。*/int raise(int sig);//给当前进程发送信号void abort(void);//给当前进程发送SIGABRT信号，杀死当前进程unsigned int alarm(unsigned int seconds);//设置定时器，函数调用，倒计时为0时，函数会给当前的进程发送一个SIGALARM信号。//返回值，倒计时剩余的时间，之前没有定时器返回0//SIGALARM信号默认终止当前进程，每一个进程都有且只有唯一的定时器//该函数时不阻塞的，时间到了会终止当前的进程//后面的调用会把前面的取消//实际的时间 = 内核时间 + 用户时间 + 消耗的时间int setitimer(int which, const struct itimerval *new_value,                struct itimerval *old_value);/*功能：设置定时器，精度微秒μ，可以实现周期性定时其中which参数表示类型，可选的值有：ITIMER_REAL：以系统真实的时间来计算，它送出SIGALRM信号。ITIMER_VIRTUAL：以该进程在用户态下花费的时间来计算，它送出SIGVTALRM信号。ITIMER_PROF：以该进程在用户态下和内核态下所费的时间来计算，它送出SIGPROF信号。*///紧接着的new_value和old_value均为itimerval结构体，先看一下itimerval结构体定义：struct itimerval {    struct timeval it_interval; /* next value */    struct timeval it_value;    /* current value */}; struct timeval {    time_t      tv_sec;         /* seconds */    suseconds_t tv_usec;        /* microseconds */};/*itimeval又是由两个timeval结构体组成，timeval包含tv_sec和tv_usec两部分，其中tv_se为秒，tv_usec为微秒(即1/1000000秒)其中的new_value参数用来对计时器进行设置，it_interval为计时间隔，it_value为延时时长，下面例子中表示的是在setitimer方法调用成功后，延时1微秒便触发一次SIGALRM信号，以后每隔200毫秒触发一次SIGALRM信号。settimer工作机制是，先对it_value倒计时，当it_value为零时触发信号，然后重置为it_interval，继续对it_value倒计时，一直这样循环下去。基于此机制，setitimer既可以用来延时执行，也可定时执行。假如it_value为0是不会触发信号的，所以要能触发信号，it_value得大于0；如果it_interval为零，只会延时，不会定时（也就是说只会触发一次信号)。old_value参数，通常用不上，设置为NULL，它是用来存储上一次setitimer调用时设置的new_value值。*/
```



signal信号捕获函数

```c
sighandler_t signal(int signum, sighandler_t handler);//设置某个信号的捕获行为//signal：需要捕获的信号//handler：捕获后如何处理.		//-SIG_IGN：忽略信号		//SIG_DFL：使用信号默认的行为		//回调函数：内核调用，程序员只负责写捕捉到信号后如何处理//返回先前的信号处理函数指针，如果有错误则返回SIG_ERR(-1)。 　　
```



信号集

信号集被定义为一种数据类型：

```
typedef struct {    unsigned long sig[_NSIG_WORDS]；} sigset_t
```

信号集用来描述信号的集合，每个信号占用一位（64位）。Linux所支持的所有信号可以全部或部分的出现在信号集中，主要与信号阻塞相关函数配合使用。下面是为信号集操作定义的相关函数：

```c
#include <signal.h>int sigemptyset(sigset_t *set)；int sigfillset(sigset_t *set)；int sigaddset(sigset_t *set, int signum)int sigdelset(sigset_t *set, int signum)；int sigismember(const sigset_t *set, int signum)；/*sigemptyset(sigset_t *set)初始化由set指定的信号集，信号集里面的所有信号被清空，相当于64为置0；sigfillset(sigset_t *set)调用该函数后，set指向的信号集中将包含linux支持的64种信号，相当于64为都置1；sigaddset(sigset_t *set, int signum)在set指向的信号集中加入signum信号，相当于将给定信号所对应的位置1；sigdelset(sigset_t *set, int signum)在set指向的信号集中删除signum信号，相当于将给定信号所对应的位置0；sigismember(const sigset_t *set, int signum)判定信号signum是否在set指向的信号集中，相当于检查给定信号所对应的位是0还是1。*/
```



sigprocmask函数使用

阻塞信号集通过sigprocmask修改

```c
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);//功能：将自定义信号集中的数据设置到内核中（设置阻塞，解除阻塞，替换）/*how：用于指定信号修改的方式，可能选择有三种SIG_BLOCK//将set所指向的信号集中包含的信号加到当前的信号掩码中。即信号掩码和set信号集进行或操作。SIG_UNBLOCK//将set所指向的信号集中包含的信号从当前的信号掩码中删除。即信号掩码和set进行与操作。SIG_SETMASK //将set的值设定为新的进程信号掩码。即set对信号掩码进行了赋值操作。set：为指向信号集的指针，在此专指新设的信号集，如果仅想读取现在的屏蔽值，可将其置为NULL。oldset：也是指向信号集的指针，在此存放原来的信号集。可用来检测信号掩码中存在什么信号。                       */int sigpending(sigset_t *set);//功能：获取内核中的未决信号集到set里
```



sigaction信号捕获函数

```c
int sigaction(int signum, const struct sigaction *act,                struct sigaction *oldact);//功能：检查或改变信号的处理。信号捕捉。//signo : 指定操作的信号//act : 新修改的sigaction//oldact : 保存该函数原有的sigaction。/*sigaction结构体定义struct sigaction{    void (*sa_handler)(int);    void (*sa_sigaction)(int, siginfo_t*, void*);    sigset_t sa_mask;    int sa_flags;};sa_handler：信号处理器函数的地址，亦或是常量SIG_IGN、SIG_DFL之一。仅当sa_handler是信号处理程序的地址时，亦即sa_handler的取值在SIG_IGN和SIG_DFL之外，才会对sa_mask和sa_flags字段加以处理。sa_sigaction：如果设置了SA_SIGINFO标志位，则会使用sa_sigaction处理函数，否则使用sa_handler处理函数。sa_mask：定义一组信号，在调用由sa_handler所定义的处理器程序时将阻塞该组信号，不允许它们中断此处理器程序的执行。sa_flags：位掩码，指定用于控制信号处理过程的各种选项。SA_NODEFER：捕获该信号时，不会在执行处理器程序时将该信号自动添加到进程掩码中。SA_ONSTACK：针对此信号调用处理器函数时，使用了由sigaltstack()安装的备选栈。SA_RESETHAND：当捕获该信号时，会在调用处理器函数之前将信号处置重置为默认值(即SIG_IGN)。SA_SIGINFO：调用信号处理器程序时携带了额外参数，其中提供了关于信号的深入信息*/
```

建议使用sigaction(POSIX标准)代替signal(美国国家标准)



SIGCHLD信号

产生的条件：

1. 子进程终止时
2. 子进程接收到SIGSTOP时
3. 子进程处在停止态，接收到SIGCOUNT后唤醒时

以上三个条件都会给父进程发送SIGCHLD信号，父进程默认忽略



共享内存

```c
int shmget(key_t key, size_t size, int shmflg);//功能：找见或找到一个共享内存段的标识/* * key：SHM 标识，找到或创建 * size：SHM 大小 * shmflg：创建或得到的属性，例如 IPC_CREAT创建，IPC_EXCL判断 * return：成功返回 shmid，失败返回 -1，并设置 erron*/void *shmat(int shmid, const void *shmaddr, int shmflg);//功能：和单签进程进行关联/* * shmid：SHM ID * shmaddr：SHM 内存地址，填NULL由内核指定 * shmflg：SHM 权限。SHM_RDONLY读权限必须有，0读写 * return：成功返回 SHM 的地址，失败返回 (void *) -1，并设置 erron*/int shmdt(const void *shmaddr);/* * shmaddr：已经映射的 SHM 地址 * return：成功返回 0，失败返回 -1，并设置 erron*/int shmctl(int shmid, int cmd, struct shmid_ds *buf);/*shm_id是shmget()函数返回的共享内存标识符。command是要采取的操作，它可以取下面的三个值 ：    IPC_STAT：把shmid_ds结构中的数据设置为共享内存的当前关联值，即用共享内存的当前关联值覆盖shmid_ds的值。    IPC_SET：如果进程有足够的权限，就把共享内存的当前关联值设置为shmid_ds结构中给出的值    IPC_RMID：删除共享内存段    buf是一个结构指针，它指向共享内存模式和访问权限的结构。shmid_ds结构 至少包括以下成员：struct shmid_ds{    uid_t shm_perm.uid;    uid_t shm_perm.gid;    mode_t shm_perm.mode;};*/key_t ftok(const char *pathname, int proj_id);//功能：根据指定的路径名和int值，生成一个共享内存的key
```

```c 
ipcs -a   -m      -q     -s 可以打印	 所有 共享内存 消息队列 信号 进行进程间通信的消息ipcrm -同上可以用来移除用api创建的进程通信的工具
```

共享内存核内存映射的区别。



守护进程

`echo $$`查看当前终端的PID

默认情况下，每个进程的输入、输出、错误都指向控制终端
进程组是一组相关进程的集合
会话是一组相关进程组的集合

进程组由多个共享同一PGID的进程组成，PGID就是首进程ID，新进程会继承其父进程的所属的进程组ID

会话首进程是创建改新会话的进程。一个会话中的所有进程共享单个控制终端，一个终端最多成为一个会话的控制终端。任一时刻，会话中的一个进程组成为该会话的前台进程组。



进程组、会话操作函数

```c
int setpgid(pid_t pid, pid_t pgid);pid_t getpgid(pid_t pid);pid_t getpgrp(void);                 /* POSIX.1 version */pid_t getpgrp(pid_t pid);            /* BSD version */int setpgrp(void);                   /* System V version */int setpgrp(pid_t pid, pid_t pgid);  /* BSD version */pid_t getsid(pid_t pid);pid_t setsid(void);
```



守护进程

守护进程是Linux的后台服务进程，一般以d结尾的名字。

特征：生命周期很长（从开机到关机）。后台运行并不拥有控制终端。

Linux大多数服务器就是用守护进程实现的，比如httpd，sshd等

守护进程的创建步骤：

（1）在父进程中执行fork并exit推出；
（2）在子进程中调用setsid函数创建新的会话；
（3）在子进程中调用chdir函数，让根目录 ”/” 成为子进程的工作目录；
（4）在子进程中调用umask函数，设置进程的umask为0；
（5）在子进程中关闭任何不需要的文件描述符



# 线程

线程只需将数据复制到共享(全局或堆)变量中即可

栈空间，.text空间都分为多份给每个线程

```c
//编译的时候需要带有 -pthread

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                    void *(*start_routine) (void *), void *arg);
//功能：创建一个子线程
//thread是传出参数，负责向调用者传递子线程的线程号
//attr设置线程的属性，可以设置成NULL
/*
typedef struct
{
       int                               detachstate;   //线程的分离状态
       int                               schedpolicy;  //线程调度策略
       structsched_param              schedparam;  //线程的调度参数
       int                               inheritsched;  //线程的继承性
       int                                scope;       //线程的作用域
       size_t                           guardsize;   //线程栈末尾的警戒缓冲区大小
       int                                stackaddr_set;   //线程堆栈的地址集
       void*                          stackaddr;   //线程栈的位置
       size_t                           stacksize;    //线程栈的大小
}pthread_attr_t;
*/
//第三个参数是函数指针，子线程处理的回调代码
//arg给第三个参数使用，传参
//失败错误号和errno不一样，不能再用perror了，用char* strerror(int errnum);

void pthread_exit(void *retval);
//功能：终止一个线程
//提供指针变量*value_ptr给pthread_join()调用
//线程的终止可以是调用了pthread_exit或者该线程的例程结束。也就是说，一个线程可以隐式的退出，也可以显式的调用pthread_exit函数来退出。
//pthread_exit函数唯一的参数value_ptr是函数的返回代码，只要pthread_join中的第二个参数value_ptr不是NULL，这个值将被传递给value_ptr。
    
pthread_t pthread_self(void);
//功能：获取当前的线程的线程ID

int pthread_equal(pthread_t t1, pthread_t t2);
//功能：比较两个线程ID是否相等

int pthread_join(pthread_t thread, void **retval);
/*
函数pthread_join的作用是，和终止的线程进行连接，等待一个线程终止。它是阻塞函数，调用一次只能回收一个子线程
调用pthread_join的线程将被挂起直到参数thread所代表的线程终止时为止。pthread_join是一个线程阻塞函数，调用它的函数将一直等到被等待的线程结束为止。
如果retval不为NULL，那么线程thread的返回值存储在该指针指向的位置。该返回值可以是由pthread_exit给出的值，或者该线程被取消而返回PTHREAD_CANCELED。
*/
```



线程的分离

```c
int pthread_detach(pthread_t thread);
//主线程与子线程分离，子线程结束后，资源自动回收。
```



线程取消

```c
int pthread_cancel(pthread_t thread);
//功能：取消线程（让线程终止）
//不是立即终止，而是当子线程运行到一个取消点线程才会终止。取消点是系统规定好的一些系统调用，我们可以粗略理解为从用户区到内核区的切换
```



线程属性

```c
int pthread_attr_init(pthread_attr_t *attr);
//初始化线程属性变量

int pthread_attr_destroy(pthread_attr_t *attr);
//释放线程属性的资源

int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
//设置线程分离的状态属性

int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
//获取线程分离的状态属性 
```



线程同步



互斥锁。用互斥量(mutex是mutual exclusion的缩写)

互斥量类型`pthread_mutex_t`

```c
int pthread_mutex_init(pthread_mutex_t *restrict mutex,
    const pthread_mutexattr_t *restrict attr);
//初始化互斥量

int pthread_mutex_destroy(pthread_mutex_t *mutex);
//释放互斥量的资源

int pthread_mutex_lock(pthread_mutex_t *mutex);
//加锁，阻塞的，如果一个线程加锁了，其他线程只能阻塞等待

int pthread_mutex_trylock(pthread_mutex_t *mutex);
//尝试加锁，如果加锁失败，不会阻塞，会直接返回

int pthread_mutex_unlock(pthread_mutex_t *mutex);
//解锁
```



死锁的场景：

1. 忘记释放锁
2. 重复加锁
3. 多线程多锁，抢占锁资源



读写锁

满足能够允许多个读出，但只允许一个写入的需求。

当读写锁被加了写锁时，其他线程对该锁加读锁或者写锁都会阻塞（不是失败）。 

当读写锁被加了读锁时，其他线程对该锁加写锁会阻塞，加读锁会成功。

```c
int pthread_rwlock_init(pthread_rwlock_t *rwptr, const pthread_rwlockattr_t *attr)
int pthread_rwlock_destroy(pthread_rwlock_t *rwptr);
//都返回：成功时为0，出错时为正的Exxx值

int pthread_rwlock_rdlock(pthread_rwlock_t *rwptr); 
//获取一个读出锁

int pthread_rwlock_wrlock(pthread_rwlock_t *rwptr); 
//获取一个写入锁

int pthread_rwlock_unlock(pthread_rwlock_t *rwptr); //释放一个写入锁或者读出锁
//都返回：成功时为0，出错时为正的Exxx值

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwptr);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwptr);
//都返回：成功时为0，出错时为正的Exxx值
```

有些同学认为读锁没有用，他们的理由是：读操作又不会修改数据，想读就读呗，无论读的是就值还是新值，反正能读到。

任何锁表面上是互斥，但本质是都是为了避免原子性问题。

所以读锁是防止读到写的中间值。



条件变量

条件变量是用来等待线程而不是上锁的，条件变量通常和互斥锁一起使用。条件变量之所以要和互斥锁一起使用，主要是因为互斥锁的一个明显的特点就是它只有两种状态：锁定和非锁定，而条件变量可以通过允许线程阻塞和等待另一个线程发送信号来弥补互斥锁的不足，所以互斥锁和条件变量通常一起使用。

当条件满足的时候，线程通常解锁并等待该条件发生变化，一旦另一个线程修改了环境变量，就会通知相应的环境变量唤醒一个或者多个被这个条件变量阻塞的线程。这些被唤醒的线程将重新上锁，并测试条件是否满足。一般来说条件变量被用于线程间的同步；当条件不满足的时候，允许其中的一个执行流挂起和等待。

简而言之，条件变量本身不是锁，但它也可以造成线程阻塞，通常与互斥锁配合使用，给多线程提供一个会合的场所。

**条件变量的优点：**

相较于mutex而言，条件变量可以减少竞争。如果仅仅是mutex，那么，不管共享资源里有没数据，生产者及所有消费都全一窝蜂的去抢锁，会造成资源的浪费。

如直接使用mutex，除了生产者、消费者之间要竞争互斥量以外，消费者之间也需要竞争互斥量，但如果汇聚（链表）中没有数据，消费者之间竞争互斥锁是无意义的。有了条件变量机制以后，只有生产者完成生产，才会引起消费者之间的竞争。提高了程序效率。

主要应用函数：

pthread_cond_init函数
pthread_cond_destroy函数
pthread_cond_wait函数
pthread_cond_timedwait函数
pthread_cond_signal函数
pthread_cond_broadcast函数

以上6 个函数的返回值都是：成功返回0， 失败直接返回错误号。

pthread_cond_t类型：用于定义条件变量，比如：pthread_cond_t cond;

```c
int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
初始化一个条件变量
cond：条件变量，调用时应传&cond给该函数
attr：条件变量属性，通常传NULL，表示使用默认属性
也可以使用静态初始化的方法，初始化条件变量：
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_destroy函数

int pthread_cond_destroy(pthread_cond_t *cond);
销毁一个条件变量

int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
阻塞等待一个条件变量。具体而言有以下三个作用：
1. 阻塞等待条件变量cond（参1）满足；
2. 释放已掌握的互斥锁mutex（解锁互斥量）相当于pthread_mutex_unlock(&mutex);
3. 当被唤醒，pthread_cond_wait函数返回时，解除阻塞并重新申请获取互斥锁
其中1、2.两步为一个原子操作。

int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);
限时等待一个条件变量
前两个比较好理解，重点说明第三个参数。
这里有个struct timespec结构体，可以在man sem_timedwait中查看。结构体原型如下：
struct timespec {
    time_t tv_sec; /* seconds */ 秒
    long tv_nsec; /* nanosecondes*/ 纳秒
}
struct timespec定义的形参abstime是个绝对时间。注意，是绝对时间，不是相对时间。什么是绝对时间？2018年10月1日10:10:00，这就是一个绝对时间。什么是相对时间？给洗衣机定时30分钟洗衣服，就是一个相对时间，也就是说从当时时间开始计算30分钟，诸如此类。
如：time(NULL)返回的就是绝对时间。而alarm(1)是相对时间，相对当前时间定时1秒钟。
adstime所相对的时间是相对于1970年1月1日00:00:00，也就是UNIX计时元年。
下面给出一个错误用法：
struct timespec t = {1, 0};
pthread_cond_timedwait (&cond, &mutex, &t);
这种用法只能定时到 1970年1月1日 00:00:01秒，想必这个时间大家都还没出生。
正确用法：
time_t cur = time(NULL); 获取当前时间。
struct timespec t; 定义timespec 结构体变量t
t.tv_sec = cur+1; 定时1秒
pthread_cond_timedwait (&cond, &mutex, &t); 传参

int pthread_cond_signal(pthread_cond_t *cond);
唤醒至少一个阻塞在条件变量上的线程

int pthread_cond_broadcast(pthread_cond_t *cond);
唤醒全部阻塞在条件变量上的线程
```



信号量

信号量是包含一个非负整数型的变量，并且带有两个原子操作wait和signal。Wait还可以被称为down、P或lock，signal还可以被称为up、V、unlock或post。在UNIX的API中（POSIX标准）用的是wait和post。

对于wait操作，如果信号量的非负整形变量S大于0，wait就将其减1，如果S等于0，wait就将调用线程阻塞；对于post操作，如果有线程在信号量上阻塞（此时S等于0），post就会解除对某个等待线程的阻塞，使其从wait中返回，如果没有线程阻塞在信号量上，post就将S加1.

由此可见，S可以被理解为一种资源的数量，信号量即是通过控制这种资源的分配来实现互斥和同步的。如果把S设为1，那么信号量即可使多线程并发运行。另外，信号量不仅允许使用者申请和释放资源，而且还允许使用者创造资源，这就赋予了信号量实现同步的功能。可见信号量的功能要比互斥量丰富许多。

POSIX信号量是一个sem_t类型的变量，但POSIX有两种信号量的实现机制：**无名信号量**和**命名信号量**。无名信号量只可以在共享内存的情况下，比如实现进程中各个线程之间的互斥和同步，因此无名信号量也被称作基于内存的信号量；命名信号量通常用于不共享内存的情况下，比如进程间通信。

同时，在创建信号量时，根据信号量取值的不同，POSIX信号量还可以分为：

- 二值信号量：信号量的值只有0和1，这和互斥量很类似，若资源被锁住，信号量的值为0，若资源可用，则信号量的值为1；
- 计数信号量：信号量的值在0到一个大于1的限制值之间，该计数表示可用的资源的个数。

| 有名信号量   |                | 无名信号量    |
| ------------ | -------------- | ------------- |
| sem_open()   |                | sem_init()    |
|              | sem_wait()     |               |
|              | sem_trywait()  |               |
|              | sem_post()     |               |
|              | sem_getvalue() |               |
| sem_close()  |                | sem_destroy() |
| sem_unlink() |                |               |

无名信号量

```c
【sem_init函数】：
该函数用于创建信号量，原型如下：
int sem_init(sem_t *sem, int pshared, unsigned int value);
该函数初始化由sem指向的信号对象，设置它的共享选项，并给它一个初始的整数值。pshared控制信号量的类型，如果其值为0，就表示信号量是当前进程的局部信号量，否则信号量就可以在多个进程间共享，value为sem的初始值。

该函数调用成功返回0，失败返回-1。

【sem_destroy函数】：
该函数用于对用完的信号量进行清理，其原型如下：
int sem_destroy(sem_t *sem);
成功返回0，失败返回-1。

【sem_wait函数】：
该函数用于以原子操作的方式将信号量的值减1。原子操作就是，如果两个线程企图同时给一个信号量加1或减1，它们之间不会互相干扰。其原型如下：
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
sem指向的对象是sem_init调用初始化的信号量。调用成功返回0，失败返回-1。

sem_trywait()则是sem_wait()的非阻塞版本，当条件不满足时（信号量为0时），该函数直接返回EAGAIN错误而不会阻塞等待。

sem_timedwait()功能与sem_wait()类似，只是在指定的abs_timeout时间内等待，超过时间则直接返回ETIMEDOUT错误。

【sem_post函数】：
该函数用于以原子操作的方式将信号量的值加1，其原型如下：
int sem_post(sem_t *sem);
与sem_wait一样，sem指向的对象是由sem_init调用初始化的信号量。调用成功时返回0，失败返回-1。

【sem_getvalue函数】：
该函数返回当前信号量的值，通过restrict输出参数返回。如果当前信号量已经上锁（即同步对象不可用），那么返回值为0，或为负数，其绝对值就是等待该信号量解锁的线程数。
int sem_getvalue(sem_t *restrict, int *restrict);
```



信号量-互斥量-条件变量三者的差异

很多时候信号量、互斥量和条件变量都可以在某种应用中使用，那这三者的差异有哪些呢？下面列出了这三者之间的差异：
- 互斥量必须由给它上锁的线程解锁；而信号量不需要由等待它的线程进行挂出，可以在其他进程进行挂出操作；
- 互斥量要么被锁住，要么被解开，只有这两种状态；而信号量的值可以支持多个进程/线程成功的进行wait操作；
- 信号量的挂出操作总是被记住，因为信号量有一个计数值，挂出操作总会将该计数值加1，然而当条件变量发送一个信号时，如果没有线程等待在条件变量，那么该信号就会丢失。

