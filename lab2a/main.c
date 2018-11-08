#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>

#define FILENAME "file.txt"
#define TARGETFILE "target.txt"
#define FILESIZE 1000 //amount of "data" default: 10 000 000

char * data = "0000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999";

int createFile(char * filename, struct stat * stats)
{
	int fd = open(filename, O_RDWR | O_APPEND | O_CREAT);
	if(fd <= 0){
		printf("File \"%s\" open't\n", FILENAME);
	}else{
		fstat(fd, stats);
		printf("File \"%s\" opened. Size: %jd bytes\n", filename, stats->st_size);
	}
	return fd;
}

int main()
{
	struct stat statsIn, statsTr;
	int fdIn = createFile(FILENAME, &statsIn);
	int fSizeIn = 0;
	if(fdIn >= 0){
		fSizeIn = statsIn.st_size;
		for (unsigned long long int i=fSizeIn/100; i<FILESIZE; i++){
			write(fdIn, data, 100*sizeof(char));
		}
		printf("File \"%s\" ready. Size: %jd bytes\n", FILENAME, statsIn.st_size);
	}

	int fdTr = createFile(TARGETFILE, &statsTr);
	
	return 0;
}
