#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

// 권한을 변경하는 함수 _ 디렉토리일 땐 재귀적으로 돌아가게 함
void chmod_recursive(const char *dirpath, mode_t mode) {
	struct dirent *entry;
	struct stat info;
	char path[PATH_MAX];
	DIR *dir = opendir(dirpath);
    
	if (dir == NULL) {
		perror("Error opening directory");
		return;
	}

	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
			continue;
		// 현재 파일의 경로
                snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);
		// 파일 정보가 있는지
            	if (stat(path, &info) == -1) {
			perror("stat");
			closedir(dir);
			return;
		}	
		// 디렉토리면 재귀적 호출!
		if (S_ISDIR(info.st_mode))
			chmod_recursive(path, mode);
		// 파일 권한 변경
           	if (chmod(path, mode) == -1) {
           	 	perror("chmod");
           	 	closedir(dir);
           	 	return;
    		}
    	}

	closedir(dir);
} 

int main(int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <mode> <file or directory>\n", argv[0]);
		exit(1);
	}
	// 숫자로 입력한 권한을 8진수로 변환
	mode_t mode = strtol(argv[1], NULL, 8);
	// 입력된 인자를 분석하여 파일이면 권한 변경, 디렉토리면 재귀 권한 변경 수행
	for (int i = 2; i < argc; i++) {
		struct stat info;
		if (stat(argv[i], &info) == -1) {
			perror(argv[i]);
			continue;
		}
		// 디렉토리라면 재귀적으로 동작하는 함수 호출
		if (S_ISDIR(info.st_mode)) {
			if (chmod(argv[i], mode) == -1) {
           	 		perror("chmod");
           	 		continue;
    			}
    			chmod_recursive(argv[i], mode);
    		}

		else { // 파일이라면 바로 권한 변경
			if (chmod(argv[i], mode) == -1) {
				perror("chmod");
				continue;
			}
		}
	}
	return 0;	
}