#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char* s1, char* s2);

int main(int ac, char* av[]) {
	int in_fd, out_fd, n_chars;
	char buf[BUFFERSIZE];

	if (ac < 3) {
		fprintf(stderr, "usage:%s source destination\n", *av);
		exit(1);
	}

	for (int i = 1; i < ac - 1; i++) {
		// 입력 파일 열기
		if ((in_fd = open(av[i], O_RDONLY)) == -1)
			oops("Cannot open", av[i]);

		// create는 출력 파일이 기존에 있으면 열고 없으면 생성
		if ((out_fd = creat(av[ac - 1], COPYMODE)) == -1)
			oops("Cannot creat", av[ac - 1]);

		while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0) {
			// 파일 내용을 버퍼에서 읽은 뒤 출력 파일에 씀
			if (write(out_fd, buf, n_chars) != n_chars)
				oops("write error to", av[ac - 1]);
		}

		if (n_chars == -1)
			oops("read error from", av[i]);

		if (close(in_fd) == -1 || close(out_fd) == -1)
			oops("error closing file", "");
	}
	return 0;
}

// perror를 사용하여 오류메시지 구현
void oops(char* s1, char* s2) {
	fprintf(stderr, "error:%s", s1);
	perror(s2);
	exit(1);
}