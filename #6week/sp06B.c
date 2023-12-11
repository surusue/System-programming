#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

void inthandler() {
    printf("user interrupt\n");
    exit(0);
}

int main(int argc, char *argv[]) {
/* argc가 입력되지 않았을 때 기본 시간 제한을 3으로 설정 */
    int limitime = 3;
/* 만약 입력된 값이 있다면 그 값을 시간 제한으로 사용하되! 음수가 입력되면 기본 시간 제한인 3으로 설정 */
    if (argc > 1) {
        int input = atoi(argv[1]);
        limitime = input;
        if (input < 0)
            limitime = 3;
    }
/* 컨트롤 + c를 누르면 user interrupt를 출력하고 종료하도록 함 (inthandler 함수) */
    signal(SIGINT, inthandler);
    clock_t startime = clock(); 

    for (int i = 1; i <= limitime; i++) {
        clock_t currentime = clock(); 
        double elapsedtime = (double)(currentime - startime) / CLOCKS_PER_SEC;

        printf("after %ds\n", i);
/* sleep()대신 1초 동안 대기하기 위한 처리 */
        while (((double)(clock() - currentime) / CLOCKS_PER_SEC) < 1);

/* 설정된 제한 시간 초과시 시간이 모두 경과했기 때문에 탈출하고 time out 출력 */
        if (elapsedtime >= limitime) {
            break;
        }
    }
    printf("time out\n");
    return 0;
}
