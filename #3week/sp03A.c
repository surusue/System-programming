#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

void do_ls(char []);
void dostat(char *);
void show_file_info(char *, struct stat *);
void mode_to_letters(int, char[]);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);
void do_recursive(char []);

int main(int ac, char *av[]) {
    // 파일/디렉토리 이름 없이 명령어만 입력한 경우에는 “.”을 파일 이름으로 입력한 것과 동일
    if (ac == 1)
        do_ls(".");
    else {
        for (int i = 1; i < ac; i++) {
            struct stat info;
            // 파일 정보 획득 실패 시 에러 출력 후 다음으로
            if (stat(av[i], &info) == -1) {
                perror(av[i]);
                printf("\n");
                continue;
            }
            // 디렉토리인 경우
            if (S_ISDIR(info.st_mode)) {
                printf("%s:\n", av[i]);
                do_recursive(av[i]);
            } else {
                // 디렉토리가 아닌 경우 그 파일 정보만 출력
                show_file_info(av[i], &info);
                printf("\n");
            }
        }
    }
    return 0;
}

// 강의 자료의 ls –al 디렉토리 함수
void do_ls(char dirname[]) {
    DIR *dir_ptr;
    struct dirent *direntp;

    if ((dir_ptr = opendir(dirname)) == NULL)
        fprintf(stderr, "ls: cannot open %s\n", dirname);
    else {
        // 디렉토리의 각 항목에 대해 파일 정보 출력 함수 호출
        while ((direntp = readdir(dir_ptr)) != NULL)
            dostat(direntp->d_name);
        closedir(dir_ptr);
    }
}

// ls –alR 을 구현하기 위한 디렉토리 함수
void do_recursive(char dirname[]) {
    DIR *dir_ptr;
    struct dirent *direntp;
    struct stat info;

    if ((dir_ptr = opendir(dirname)) == NULL) {
        fprintf(stderr, "ls: cannot open %s\n", dirname);
        exit(EXIT_FAILURE);
    } else {
        while ((direntp = readdir(dir_ptr)) != NULL) {
            char path[1024];
            // 파일 경로 생성 (재귀 기록을 위함)
            snprintf(path, sizeof(path), "%s/%s", dirname, direntp->d_name);

            if (stat(path, &info) == -1) {
                perror(path);
                exit(EXIT_FAILURE);
            }

            show_file_info(path, &info);

            // 디렉토리이면서 현재 디렉토리(.) 또는 상위 디렉토리(..)가 아닌 경우 재귀 호출
            if (S_ISDIR(info.st_mode) && strcmp(direntp->d_name, ".") != 0 && strcmp(direntp->d_name, "..") != 0)
                do_recursive(path);
        }
    }
    closedir(dir_ptr);
    printf("\n");
}

void dostat(char *fname) {
    struct stat info;
    if (stat(fname, &info) == -1)
        perror(fname);
    else
        show_file_info(fname, &info);
}

void show_file_info(char *fname, struct stat *buf) {
    char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
    void mode_to_letters();
    char modestr[11];
    mode_to_letters(buf->st_mode, modestr);

    printf("%s", modestr);
    printf("%4d ", (int)buf->st_nlink);
    printf("%-s ", uid_to_name(buf->st_uid));
    printf("%-5s ", gid_to_name(buf->st_gid));
    printf("%4ld ", (long)buf->st_size);
    printf("%.12s", 4 + ctime(&buf->st_mtime));
    printf(" %s\n", fname);
}

void mode_to_letters(int mode, char str[]) {
    strcpy(str, "----------");

    if (S_ISDIR(mode)) str[0] = 'd';
    if (S_ISCHR(mode)) str[0] = 'c';
    if (S_ISCHR(mode)) str[0] = 'c';

    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';

    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';

    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
}

char *uid_to_name(uid_t uid) {
    struct passwd *getpwuid(), *pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL) {
        sprintf(numstr, "%d", uid);
        return numstr;
    } else
        return pw_ptr->pw_name;
}

char *gid_to_name(gid_t gid) {
    struct group *getgrgid(), *grp_ptr;
    static char numstr[10];

    if ((grp_ptr = getgrgid(gid)) == NULL) {
        sprintf(numstr, "%d", gid);
        return numstr;
    } else
        return grp_ptr->gr_name;
}
