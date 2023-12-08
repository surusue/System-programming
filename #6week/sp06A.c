#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
/* perror() 를 사용하기 위함 fork 실패시 오류 처리에 모두 perror를 사용 */
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int status;
    /* son 프로세스 생성으로 구조 시작 */
    pid_t son = fork(); 

    if (son < 0) {
        perror("fork:");
        exit(1);
    /* 0이면 프로세스가 잘 생성됐음을 뜻함 */
    } else if (son == 0) {
        printf("I am son.\n");
        fflush(stdout);
        /* daughter가 아닌 son에서 프로세스 생성 (grand)*/
        pid_t grandson = fork(); 
        if (grandson < 0) {
            perror("fork");
            exit(1);
        } else if (grandson == 0) {
            printf("I am grandson.\n");
            fflush(stdout);
            sleep(1); 
            /* 실행 후 종료까지 */
            printf("grandson exits.\n");
            fflush(stdout);      
            exit(0);
        } else {
        /* 손녀 생성 (granddaughter) */
            pid_t granddaughter = fork(); 
            if (granddaughter < 0) {
                perror("fork");
                exit(1);
            } else if (granddaughter == 0) {
                printf("I am granddaughter.\n");
                fflush(stdout);
                sleep(1);
                /* 실행 후 종료까지 */
                printf("granddaughter exits.\n");      
                fflush(stdout);      
                exit(0);
            } else {
                /* son이 자식의 종료를 기다리는 부분 */
                waitpid(granddaughter, &status, 0);
                printf("son finished waiting for granddaughter.\n");
            }
            waitpid(grandson, &status, 0);
            printf("son finished waiting for grandson.\n");
        }
        printf("son exits.\n");
        exit(0);
    } else {
        printf("I am parent.\n");
        fflush(stdout);
        /* parent에서 daughter로 가지가 뻗어감 */
        pid_t daughter = fork();
        if (daughter < 0) {
            perror("fork");
            exit(1);
        } else if (daughter == 0) {
            printf("I am daughter.\n");
            sleep(1);
            printf("daughter exits.\n");      
            fflush(stdout);      
            exit(0);
        } else {
            /* parent가 자식의 종료를 기다리는 부분 */
            waitpid(daughter, &status, 0);
            printf("parent finished waiting for daughter.\n");
            fflush(stdout);
        }
        waitpid(son, &status, 0);
        printf("parent finished waiting for son.\n");
        fflush(stdout);
    }
    /* 마지막으로 parent까지 종료하고 모든 수행이 끝남 */
    printf("parent exits.\n");
    exit(0);
    return 0;
}