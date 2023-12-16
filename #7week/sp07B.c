#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define BUF_SIZE 100

void fororder() {
        /* write와 read 순서가 꼬이지 않게 시그널을 주기 위한 함수 */
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(1);
    }

    int fd[2];
    char buf[BUF_SIZE];
    pid_t pid;
    char* str1 = argv[1];
    char* str2;
    struct sigaction sa;

    /* SIGUSR1 시그널 핸들러 등록 */
    sa.sa_handler = fororder;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) { /* 자식 프로세스의 공간 */
        /* 자식 프로세스가 파이프에 문자열 쓰는 것으로 시작 */
        write(fd[1], str1, strlen(str1) + 1);
        /* 부모 프로세스에게 썼음을 알리는 신호를 주고 부모가 쓸 때까지 대기 */
        kill(getppid(), SIGUSR1);
        pause();
        /* 신호를 받으면 잘 써졌다는 뜻이니 읽고나서 출력 */
        read(fd[0], buf, BUF_SIZE);
        printf("child: %s\n", buf);
        /* 아스키코드 +2를 위한 부분 */
        for (int i = 0; i < strlen(buf); i++) {
                buf[i] = buf[i] + 2;
        }
        str1 = buf;
        /* 쓰고나서 아까처럼 또 신호를 줌 */
        write(fd[1], str1, strlen(str1) + 1);
        kill(getppid(), SIGUSR1);
        /* 자식 프로세스 종료 */
        exit(0);
    } else {
        /* 부모는 읽을 것이 생길 때까지 대기, 읽고나서 출력 */
        pause();
        read(fd[0], buf, BUF_SIZE);
        printf("parent: %s\n", buf);
        /* 아스키코드 +1를 위한 부분 */
        for (int i = 0; i < strlen(buf); i++) {
                buf[i] = buf[i] + 1;
        }

        str2 = buf;
        /* 자식에게 넘겨주고 잘 썼음을 알리는 신호 전달 */
        write(fd[1], str2, strlen(str2) + 1);
        kill(pid, SIGUSR1);
        /* 다시 받을 때까지 대기 */
        pause();
        read(fd[0], buf, BUF_SIZE);   

        printf("parent: %s\n", buf);
        /* 부모 프로세스 종료 */
        exit(0);
    }

    return 0;
}