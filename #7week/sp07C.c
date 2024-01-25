#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;
    int status;
    /* 파이프 생성 */
    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }
    /* 자식 프로세스 생성 */
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { /* 첫 번째 자식 프로세스의 공간 */
        close(pipefd[0]); 
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[1]);
        execlp("ls", "ls", NULL); /* ls 명령어 실행 */
        perror("execlp ls failed");
        exit(EXIT_FAILURE);
    } else { /* 부모 프로세스의 공간 */
        pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { /* 두 번째 자식 프로세스의 공간 */ 
            close(pipefd[1]); 
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            execlp("wc", "wc", NULL); /* wc 명령어 실행 */
            perror("execlp wc failed");
            exit(EXIT_FAILURE);
        } else {
            close(pipefd[0]); 
            close(pipefd[1]); 
            /* 자식 프로세스들의 종료 대기 */
            waitpid(pid, &status, 0); 
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}
