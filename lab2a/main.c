#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>

#define FILENAME "file.txt"

int main()
{
	char * data = "1234567890111111111122222222223333333333444444444455555555556666666666777777777788888888889999999999";
	int sizeF = 0;
	int fd = open(FILENAME, O_WRONLY | O_APPEND | O_CREAT);
	if(fd <= 0){
		printf("File open't\n");
		return 0;
	}else{
		struct stat stats;
		fstat(fd, &stats);
		sizeF = stats.st_size;
		printf("File \"%s\" opened. Size: %jd bytes\n", FILENAME, stats.st_size);
	}
	
	for (unsigned long long int i=sizeF/100; i<10000000; i++){
		write(fd, data, 100*sizeof(char) );
	}
	printf("File \"%s\" generated\n", FILENAME);
	
	return 0;
}
