#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
// 파일 정보 출력 함수
void filedata(char *fname, struct stat *buf) ;

int main(int argc, char * argv[]) {
	for (int i = 1; i < argc; i++) {
		// sys/stat.h_ 파일 정보를 저장할 구조체
		struct stat info;
		// 지금 처리할 인자를 설정
		char *fname = argv[i];
		if (argc > 1) {
			// stat 함수로 파일 정보 획득
			if (stat(fname, &info) != -1) 
				filedata(fname, &info);
			else // 파일 정보가 없다면 오류
				perror(argv[i]);
		}
	}
	return 0;
}

void filedata(char *fname, struct stat *buf) {
	char *type;
	
	// 파일의 종류에 따라 type 설정
	if (S_ISREG(buf->st_mode)) type = "regular file";
	else if (S_ISDIR(buf->st_mode)) type = "directory";
	else type = "special file";

	printf("pathname: \"%s\"\n", fname);
	printf("type: %s\n", type);
	printf("mode: %o\n", (unsigned int)buf->st_mode & 07777);
	printf("inode #: %d\n", (int)buf->st_ino);
	printf("number of linked = %d\n", (int)buf->st_nlink);
	printf("uid: %u\n", buf->st_uid);
	printf("gid = %u\n", buf->st_gid);
	printf("size = %d\n", (int)buf->st_size);
	printf("preferred I/O block size = %d\n", (int)buf->st_blksize);
	printf("number of 512-byte blocks = %d\n", (int)buf->st_blocks);
	printf("----------------------------------------\n");
}