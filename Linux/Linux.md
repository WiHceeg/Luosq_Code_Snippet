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

