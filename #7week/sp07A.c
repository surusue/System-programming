#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

void sighandler(int signum); 
void chldhandler();

int main() {
    struct sigaction sa_usr, sa_chld;
    pid_t pid;
    int status;
    srand((unsigned int)time(NULL));

    /* SIGUSR1, SIGUSR2, SIGCHLD 시그널 핸들러 등록 */
    sa_usr.sa_handler = sighandler;
    sigemptyset(&sa_usr.sa_mask);
    sa_usr.sa_flags = 0;
    sigaction(SIGUSR1, &sa_usr, NULL);
    sigaction(SIGUSR2, &sa_usr, NULL);
    sa_chld.sa_handler = chldhandler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = 0;
    sigaction(SIGCHLD, &sa_chld, NULL);

    pid = fork();
    /* fork()로 자식 프로세스 생성 후 실패 시 오류 출력하기 */
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { /* 자식 프로세스의 공간 */
        /* 부모 프로세스에게 SIGUSR1 또는 SIGUSR2 시그널 전송*/
        for (int i = 0; i < 3; i++) {
            /* 과제 조건을 해결하기 위해 rand를 사용하여 매 초마다 무작위로 전송하도록 함 
            짝수, 홀수로 구분, 1초 간격 sleep(1) */
            kill(getppid(), rand() % 2 == 0 ? SIGUSR1 : SIGUSR2);
            sleep(1);
        }
        exit(0); /* 자식 프로세스 종료 */
    } else {
        while (1) { /* 부모 프로세스의 공간 */
            pause(); /* SIGUSR 1,2 시그널을 기다림 */

            /* 자식 프로세스 상태 확인 후 벗어남 */
            if (waitpid(pid, &status, WNOHANG) > 0) 
                break;
        }
    }
    waitpid(pid, &status, 0); 
    /* 자식 프로세스 종료 후 SIGCHLD 시그널 받았음을 출력하고, 부모 프로세스 종료 메시지 출력 */
    printf("parent is terminated.\n");
    exit(0);
    return 0;
}

void sighandler(int signum) {
    if (signum == SIGUSR1) {
        printf("SIGUSR1 is received.\n");
    } else if (signum == SIGUSR2) {
        printf("SIGUSR2 is received.\n");
    } 
}

void chldhandler() {
    printf("SIGCHLD is received.\n");
}
