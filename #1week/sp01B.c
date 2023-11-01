#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	// 명령행 인자 수가 부족한 경우 종료
	if (argc < 2) {
		exit(1);
	}

	for (int i = 1; i < argc; i++) {
		// 받아온 명령행 인자 배열을 읽기 모드로 열기
		FILE* file = fopen(argv[i], "r");
		// 만약 파일이 없으면 perror를 사용해 구현한 오류메시지 출력
		if (file == NULL) {
			perror(argv[i]);
			continue;
		}

		char ch;
		// 파일로부터 한 자씩 읽어 출력
		while ((ch = fgetc(file)) != EOF) {
			putchar(ch);
		}
		fclose(file);
	}
	return 0;
}