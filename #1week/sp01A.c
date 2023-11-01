#include <stdio.h>

// argc : 명령행 인자 수, argv : 명령행 인자 배열(공백 단위로 구별)
int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        // 첫 번째 문자열은 프로그램의 실행 경로로 고정이 되어 있음
        printf("argc[%d] = '%s'\n", i, argv[i]);
    }
    return 0;
}