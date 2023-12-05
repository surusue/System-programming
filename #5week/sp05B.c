#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#define MAXARGS 20
#define ARGLEN 100

char *makestring(char *buf);
int execute(char *argliar[]);

int main(int argc, char *argv[]) {
	// if조건이 없어도 잘 동작은 하지만 첫줄에 인자를 여러개 입력해도 문제없이 엔터 이후로 그대로 동작하기 때문에 나만의 조건을 추가해 보았습니다.
    if (argc > 1) {
        fprintf(stderr, "실행파일만 입력하고 엔터 이후로 수행할 명령어를 입력하시오.\n");
        exit(1);
    }
    char argbuf[ARGLEN];

    while (1) {
        fgets(argbuf, ARGLEN, stdin);

        argbuf[strcspn(argbuf, "\n")] = '\0';
        // 과제 예시에서 'exit'로 마무리가 되었으므로 'exit'를 입력하면 프로그램을 즉시 종료하는 조건을 추가하였습니다.
        if (strcmp(argbuf, "exit") == 0) {
            break;
        }
        char *arglist[MAXARGS + 1];
        int numargs = 0;
// 명령어 문자열을 공백과 탭을 기준으로 토큰으로 분리합니다.
        char *token = strtok(argbuf, " \t\n");
        while (token != NULL && numargs < MAXARGS) {
            arglist[numargs++] = token;
            token = strtok(NULL, " \t\n");
        }
        arglist[numargs] = NULL;
// 전달할 배열의 끝을 나타냅니다.
        if (numargs > 0) {
            execute(arglist);
        }
    }

    return 0;
}

int execute(char *arglist[]) {
// 자식 프로세스를 생성합니다.
    pid_t pid = fork();
    int exitstatus;

    switch (pid) {
        case -1:
// fork 실패의 경우입니다.
            perror("fork failed"); 
            exit(1);
        case 0:
// 명령어가 존재하지 않는 경우입니다.
            execvp(arglist[0], arglist);
            perror("execvp failed");
            exit(1);
        default:
                while (wait(&exitstatus) != pid)
                    ;
    }

    return 0;
}
// 문자열을 동적으로 할당하고 반환하는 함수입니다. 1번 코드를 기준으로 수정하여서 같습니다.
char *makestring(char *buf) {
    char *cp = malloc(strlen(buf) + 1);

    if (cp == NULL) {
        fprintf(stderr, "no memory\n");
        exit(1);
    }

    strcpy(cp, buf);
    return cp;
}