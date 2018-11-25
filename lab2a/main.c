#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>

#define FILENAME "file.txt"
#define TARGETFILE "target.txt"
#define FILESIZE 1000000 //amount of "data" default: 10 000 000 (GIGABYTE)

char * data = "0000000000111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999";

void copyFile(int fdSr, int fdTr, int blocksize)
{
	lseek(fdSr, SEEK_SET, 0);
	char * buf = calloc(blocksize, sizeof(char));
	ssize_t bufAm = read(fdSr, buf, blocksize);
	do{
		write(fdTr, buf, bufAm);
		bufAm = read(fdSr, buf, blocksize);
	}while(bufAm);
}

int main()
{
	long int ttime;
	long int st;// = time(NULL);
	struct stat statsIn, statsTr;
	int fdIn = open(FILENAME, O_RDWR | O_APPEND | O_CREAT);
	if(fdIn <= 0){
		printf("File \"%s\" open't\n", FILENAME);
	}else{
		fstat(fdIn, &statsIn);
		printf("File \"%s\" opened. Size: %jd bytes\n", FILENAME, statsIn.st_size);
		fchmod(fdIn, S_IRUSR | S_IWUSR);
	}

	if (fdIn < 0){
		return 0;
	}

	int fSizeIn = statsIn.st_size;
	if(fSizeIn < FILESIZE*100){
		for (unsigned long long int i=fSizeIn/100; i<FILESIZE; i++){
			write(fdIn, data, 100*sizeof(char));
		}
		fstat(fdIn, &statsIn);
		lseek(fdIn,0,0);
		printf("File \"%s\" ready. Size: %jd bytes\n", FILENAME, statsIn.st_size);
	}

	int fdTr = open(TARGETFILE, O_RDWR | O_CREAT | O_TRUNC);
	fchmod(fdTr, S_IRUSR | S_IWUSR);
	if(fdTr < 0){
		close(fdIn);
		return 0;
	}
	fstat(fdTr, &statsTr);
	int fSizeTr = statsTr.st_size;
	printf("\"%s\" size: %d\n", TARGETFILE, fSizeTr );
	
	printf("Start copying\n");
	st = time(NULL);
	copyFile(fdIn, fdTr, 100);
	fstat(fdTr, &statsTr);
	printf("Copying \"%s\" to \"%s\" completed.\n\"%s\" size: %jd\nTime: %ld sec", FILENAME, TARGETFILE, TARGETFILE, statsTr.st_size, time(&ttime)-st);

	close(fdIn);
	close(fdTr);
	return 0;
}
