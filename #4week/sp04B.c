#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

ino_t get_inode(char *);
void printpathto(ino_t);
void inum_to_name(ino_t, char *, int);

int main() {
   printpathto(get_inode("."));
   putchar('\n');
   return 0;
}

ino_t get_inode(char *fname) {
   struct stat info;
   
   if (stat(fname, &info) == -1) {
      fprintf(stderr, "Cannot stat ");
      perror(fname);
      exit(1);
   }
   return info.st_ino; // 입력으로 받은 파일의 inode 번호 반환
}

void printpathto(ino_t this_inode) {
   ino_t my_inode;
   char its_name[BUFSIZ];
   // 루트 디렉토리가 될 때까지 상위 디렉토리로 chdir(이동)
   if (get_inode("..") != this_inode) {
      chdir("..");
      
      inum_to_name(this_inode, its_name, BUFSIZ);
      // 상위 디렉토리의 inode 번호를 통해 재귀
      my_inode = get_inode(".");
      printpathto(my_inode);
      printf("/%s", its_name);
   }
}

void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen) {
   DIR *dir_ptr;
   struct dirent *direntp;
   
   dir_ptr = opendir(".");
   if (dir_ptr == NULL) {
      perror(".");
      exit(1);
   }
   
   while ((direntp = readdir(dir_ptr)) != NULL) {
   // 찾는 파일의 이름이 나오면 namebuf에 복사하고 종료
         if (direntp -> d_ino == inode_to_find) {
            strncpy(namebuf, direntp->d_name, buflen);
            namebuf[buflen-1] = '\0';
            closedir(dir_ptr);
            return;
         }
   }
   fprintf(stderr, "error looking for inum %ld\n", (long)inode_to_find);
   exit(1); // 못 찾으면 오류 메시지 출력 후 프로그램 종료
}