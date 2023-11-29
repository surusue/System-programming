#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXARGS 20

char *makestring(char *buf);
int execute(char *argbuf[]);

int main(int argc, char *argv[]) {
// 과제 조건에서 명령어 줄에는 1개의 인자만 입력받을 수 있다고 하였으므로 2개보다 적어도 실행 조건에 부합하지 않고, 2개보다 많아도 안 되므로 조건 처리를 하였습니다.
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(1);
    }
    execute(argv);

    return 0;
}

int execute(char *argbuf[]) {
// 강의 자료에선 main에 있던 부분을 함수 안으로 옮겨 수정하였습니다.
    char *command = makestring(argbuf[1]);
    char *token;
    char *arglist[MAXARGS + 1];  
    int numargs = 0;
	// 명령어 문자열을 공백과 탭을 기준으로 토큰으로 분리합니다.
    token = strtok(command, " \t\n");
    while (token != NULL) {
        arglist[numargs++] = token;
        token = strtok(NULL, " \t\n");
    }
    arglist[numargs] = NULL; // 전달할 배열의 끝을 나타냅니다.

    execvp(arglist[0], arglist);
	// 명령어가 존재하지 않는 경우 사진의 첫 번째 오류 경우를 출력하도록 합니다.
    perror("execvp failed");
    exit(1);
}

// 문자열을 동적으로 할당하고 반환하는 함수입니다.
char* makestring(char* buf) {
    char* cp = malloc(strlen(buf) + 1);

    if (cp == NULL) {
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf);
    return cp;
}