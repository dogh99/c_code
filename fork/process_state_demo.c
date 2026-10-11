#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int zombie_demo(void)
{
    pid_t child = fork();
    if (child < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child == 0) {
        printf("[子进程 %ld] 现在退出。\n", (long)getpid());
        _exit(EXIT_SUCCESS);
    }

    /* 子进程已退出，但父进程还没调用 waitpid()，退出记录暂时保留。 */
    sleep(1);
    printf("[父进程 %ld] 子进程 PID=%ld；15 秒后调用 waitpid()。\n",
           (long)getpid(), (long)child);
    printf("现在另开终端运行：ps -o pid,ppid,stat,cmd -p %ld\n", (long)child);
    sleep(15);

    int status;
    if (waitpid(child, &status, 0) == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }
    printf("[父进程] 已回收子进程 %ld；再次运行 ps 将找不到它。\n", (long)child);
    return EXIT_SUCCESS;
}

static int orphan_demo(void)
{
    int ready[2];
    if (pipe(ready) == -1) {//管道
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t child = fork();
    if (child < 0) {
        perror("fork");
        close(ready[0]);
        close(ready[1]);
        return EXIT_FAILURE;
    }

    if (child == 0) {
        close(ready[0]);
        pid_t original_parent = getppid();
        printf("[子进程 %ld] 父进程退出前，PPID=%ld\n",
               (long)getpid(), (long)original_parent);
        /* 告诉父进程：已经记录了原父进程的 PID。 */
        if (write(ready[1], "x", 1) != 1) {
            perror("write");
            _exit(EXIT_FAILURE);
        }
        close(ready[1]);
        /* 等到父进程真正退出，再观察新的 PPID。 */
        while (getppid() == original_parent) sleep(1);
        printf("[子进程 %ld] 父进程退出后，PPID=%ld\n",
               (long)getpid(), (long)getppid());
        printf("孤儿进程仍可继续运行，并由系统中的收养进程负责回收。\n");
        _exit(EXIT_SUCCESS);
    }

    close(ready[1]);
    char signal;
    ssize_t n;
    do {
        n = read(ready[0], &signal, 1);
    } while (n == -1 && errno == EINTR);
    close(ready[0]);
    if (n != 1) {
        if (n == -1) perror("read");
        else fprintf(stderr, "子进程未发送就绪信号\n");
        return EXIT_FAILURE;
    }
    printf("[父进程 %ld] 现在退出，子进程 PID=%ld。\n",
           (long)getpid(), (long)child);
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    /* 父进程退出后，仍能看到子进程输出。 */
    setbuf(stdout, NULL);

    if (argc != 2) {
        fprintf(stderr, "用法：%s zombie|orphan\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "zombie") == 0) return zombie_demo();
    if (strcmp(argv[1], "orphan") == 0) return orphan_demo();

    fprintf(stderr, "未知模式：%s（应为 zombie 或 orphan）\n", argv[1]);
    return EXIT_FAILURE;
}
