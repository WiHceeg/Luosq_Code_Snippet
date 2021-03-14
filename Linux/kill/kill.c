#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// int kill(pid_t pid, int sig);
// int raise(int sig);
// void abort(void);

int main() {

    pid_t pid = fork();

    if (pid == 0) {

        for (int i = 0; i < 5; i++) {
            printf("child process\n");
            sleep(1);
        }

    } else if (pid > 0) {

        printf("parent process\n");
        sleep(2);
        printf("kill child process now\n");
        kill(pid, SIGINT);
    }

}

